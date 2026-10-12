#include "pc_raster.h"
#include "pc_raster_owner_internal.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PC_PIXEL_MAX 20u
#define PC_TILE_AREA ((size_t)PC_CORE_TILE_SIZE * (size_t)PC_CORE_TILE_SIZE)

/* Optional per-raster allocator accounting. C99-compatible header alignment. */
#ifdef PC_RASTER_TEST_ALLOC
typedef union PcAllocHeader {
    struct { size_t size; } meta;
    long double alignment;
    void *pointer_alignment;
} PcAllocHeader;
static PcMemoryStats pc_mem_stats;
static long pc_alloc_remaining=-1;
void pc_raster_test_fail_after(long n){pc_alloc_remaining=n;}
void pc_raster_test_stats(PcMemoryStats *out){if(out)*out=pc_mem_stats;}
static int pc_allowed(void)
{
    if(pc_alloc_remaining<0)return 1;
    if(pc_alloc_remaining==0){++pc_mem_stats.failed_allocations;return 0;}
    --pc_alloc_remaining;return 1;
}
static void *pc_alloc(size_t size)
{
    PcAllocHeader *h;
    if(!pc_allowed())return NULL;
    if(size>SIZE_MAX-sizeof(*h)){++pc_mem_stats.failed_allocations;return NULL;}
    h=(PcAllocHeader *)malloc(sizeof(*h)+size);
    if(!h){++pc_mem_stats.failed_allocations;return NULL;}
    h->meta.size=size;
    pc_mem_stats.current_bytes+=size+sizeof(*h);
    if(pc_mem_stats.current_bytes>pc_mem_stats.peak_bytes)
        pc_mem_stats.peak_bytes=pc_mem_stats.current_bytes;
    ++pc_mem_stats.allocation_count;
    return h+1;
}
static void pc_free(void *p)
{
    PcAllocHeader *h;
    if(!p)return;
    h=((PcAllocHeader *)p)-1;
    pc_mem_stats.current_bytes-=h->meta.size+sizeof(*h);
    free(h);
}
static void *pc_calloc(size_t n,size_t size)
{
    void *p;
    if(size && n>SIZE_MAX/size){++pc_mem_stats.failed_allocations;return NULL;}
    p=pc_alloc(n*size);
    if(p)memset(p,0,n*size);
    return p;
}
static void *pc_realloc(void *p,size_t size)
{
    PcAllocHeader *h;
    void *next;
    if(!p)return pc_alloc(size);
    h=((PcAllocHeader *)p)-1;
    next=pc_alloc(size);
    if(!next)return NULL;
    memcpy(next,p,h->meta.size<size?h->meta.size:size);
    pc_PC_FREE(p);
    return next;
}
#define PC_MALLOC(n) pc_alloc(n)
#define PC_CALLOC(n,s) pc_calloc((n),(s))
#define PC_REALLOC(p,n) pc_realloc((p),(n))
#define PC_FREE(p) pc_free(p)
#else
#define PC_MALLOC(n) malloc(n)
#define PC_CALLOC(n,s) calloc((n),(s))
#define PC_REALLOC(p,n) realloc((p),(n))
#define PC_FREE(p) free(p)
#endif

typedef struct PcRasterTile {
    size_t refs;
    uint8_t bytes[]; /* C99 flexible array: allocation follows raster bpp */
} PcRasterTile;
typedef struct PcRasterEntry {
    PcTileCoord coord;
    PcRasterTile *tile;
} PcRasterEntry;
struct PcRaster {
    PcPixelFormat format;
    size_t bpp;
    size_t tile_bytes;
    uint8_t default_bytes[PC_PIXEL_MAX];
    int document_owned; /* one document owns this PcRaster; COW tiles separate */
    PcRasterEntry *entries;
    size_t count, capacity;
};

/* Entries stay ordered by (y,x), allowing O(log N) lookup.
 * Return lower bound for insertion, with optional exact-match flag. */
static size_t pc_lower_bound(const PcRaster *r, PcTileCoord c, int *found)
{
    size_t lo=0,hi=r->count;
    while(lo<hi){
        size_t mid=lo+(hi-lo)/2;
        PcTileCoord m=r->entries[mid].coord;
        if(m.ty<c.ty || (m.ty==c.ty && m.tx<c.tx))lo=mid+1;
        else hi=mid;
    }
    if(found)*found=(lo<r->count &&
        pc_tile_equal(r->entries[lo].coord,c));
    return lo;
}
static size_t pc_find(const PcRaster *r, PcTileCoord c)
{
    int found;
    size_t pos=pc_lower_bound(r,c,&found);
    return found?pos:r->count;
}
static void pc_tile_release(PcRasterTile *t)
{
    if (t && --t->refs == 0) PC_FREE(t);
}
static PcRasterTile *pc_tile_alloc(const PcRaster *r)
{
    PcRasterTile *tile;
    size_t i;
    if (r->tile_bytes > SIZE_MAX - sizeof(*tile)) return NULL;
    tile = (PcRasterTile *)PC_MALLOC(sizeof(*tile)+r->tile_bytes);
    if (!tile) return NULL;
    tile->refs=1;
    for(i=0;i<r->tile_bytes;i+=r->bpp)
        memcpy(tile->bytes+i,r->default_bytes,r->bpp);
    return tile;
}
static size_t pc_offset(const PcRaster *r, int32_t x, int32_t y)
{
    int32_t local_x=x%PC_CORE_TILE_SIZE, local_y=y%PC_CORE_TILE_SIZE;
    if(local_x<0)local_x+=PC_CORE_TILE_SIZE;
    if(local_y<0)local_y+=PC_CORE_TILE_SIZE;
    return ((size_t)local_y*PC_CORE_TILE_SIZE+(size_t)local_x)*r->bpp;
}
static int pc_grow(PcRaster *r)
{
    size_t n;
    PcRasterEntry *p;
    if(r->count<r->capacity)return 1;
    n=r->capacity ? r->capacity*2 : 4;
    if(n<r->capacity||n>SIZE_MAX/sizeof(*p))return 0;
    p=(PcRasterEntry *)PC_REALLOC(r->entries,n*sizeof(*p));
    if(!p)return 0;
    r->entries=p;r->capacity=n;return 1;
}
PcRaster *pc_raster_new(PcPixelFormat fmt,const uint8_t *defaults)
{
    PcRaster *r;
    size_t bpp=pc_format_bytes_per_pixel(fmt);
    if(bpp==0 || bpp>PC_PIXEL_MAX || bpp>SIZE_MAX/PC_TILE_AREA ||
       (fmt.alpha!=0 && fmt.alpha!=1))return NULL;
    r=(PcRaster *)PC_CALLOC(1,sizeof(*r));
    if(!r)return NULL;
    r->format=fmt;
    r->bpp=bpp;
    r->tile_bytes=bpp*PC_TILE_AREA;
    if(defaults)memcpy(r->default_bytes,defaults,bpp);
    return r;
}
PcRaster *pc_raster_clone(const PcRaster *r)
{
    PcRaster *copy;
    size_t i;
    if(!r)return NULL;
    copy=pc_raster_new(r->format,r->default_bytes);
    if(!copy)return NULL;
    if(r->count){
        if(r->count>SIZE_MAX/sizeof(*copy->entries)){PC_FREE(copy);return NULL;}
        copy->entries=(PcRasterEntry *)PC_MALLOC(r->count*sizeof(*copy->entries));
        if(!copy->entries){PC_FREE(copy);return NULL;}
        memcpy(copy->entries,r->entries,r->count*sizeof(*copy->entries));
        for(i=0;i<r->count;++i){
            if(copy->entries[i].tile->refs==SIZE_MAX){
                while(i) --copy->entries[--i].tile->refs;
                PC_FREE(copy->entries);PC_FREE(copy);return NULL;
            }
            ++copy->entries[i].tile->refs;
        }
        copy->count=copy->capacity=r->count;
    }
    return copy;
}
/* Object ownership is different from tile COW sharing.
 * Keep the claim internal so ordinary raster callers can still own their
 * freshly created or cloned raster until a successful document append.
 * Document mutations in this experimental port are single-threaded.
 */
int pc_raster_claim_document_ownership(PcRaster *r)
{
    if (!r || r->document_owned) return 0;
    r->document_owned = 1;
    return 1;
}
/* Detach transfers lifetime ownership out of the document; no second
 * document may claim this object before successful detach. */
int pc_raster_release_document_ownership(PcRaster *r)
{
    if(!r || !r->document_owned)return 0;
    r->document_owned=0;
    return 1;
}
void pc_raster_destroy(PcRaster *r)
{
    size_t i;
    if(!r)return;
    for(i=0;i<r->count;++i)pc_tile_release(r->entries[i].tile);
    PC_FREE(r->entries);PC_FREE(r);
}
size_t pc_raster_tile_count(const PcRaster *r){return r?r->count:0;}
size_t pc_raster_bytes_per_pixel(const PcRaster *r){return r?r->bpp:0;}
int pc_raster_get_format(const PcRaster *r,PcPixelFormat *out)
{
    if(!r||!out)return 0;
    *out=r->format;
    return 1;
}
int pc_raster_read_pixel(const PcRaster *r,int32_t x,int32_t y,
                         uint8_t *out,size_t len)
{
    size_t i;
    if(!r||!out||len<r->bpp)return 0;
    i=pc_find(r,pc_tile_containing(x,y));
    memcpy(out,i==r->count?r->default_bytes:
        r->entries[i].tile->bytes+pc_offset(r,x,y),r->bpp);
    return 1;
}
int pc_raster_write_pixel(PcRaster *r,int32_t x,int32_t y,
                          const uint8_t *bytes,size_t len)
{
    PcTileCoord c;
    size_t i;
    int found;
    PcRasterTile *tile;
    if(!r||!bytes||len!=r->bpp)return 0;
    c=pc_tile_containing(x,y);
    i=pc_lower_bound(r,c,&found);
    if(!found){
        /* Allocate tile before reserving/committing an entry. */
        tile=pc_tile_alloc(r);
        if(!tile)return 0;
        if(!pc_grow(r)){pc_tile_release(tile);return 0;}
        memmove(&r->entries[i+1],&r->entries[i],
                (r->count-i)*sizeof(*r->entries));
        r->entries[i].coord=c;
        r->entries[i].tile=tile;
        ++r->count;
    }else{
        tile=r->entries[i].tile;
        if(tile->refs>1){
            PcRasterTile *new_tile;
            if(r->tile_bytes>SIZE_MAX-sizeof(*new_tile))return 0;
            new_tile=(PcRasterTile *)PC_MALLOC(sizeof(*new_tile)+r->tile_bytes);
            if(!new_tile)return 0;
            new_tile->refs=1;
            memcpy(new_tile->bytes,tile->bytes,r->tile_bytes);
            pc_tile_release(tile);
            r->entries[i].tile=tile=new_tile;
        }
    }
    memcpy(tile->bytes+pc_offset(r,x,y),bytes,r->bpp);
    return 1;
}
size_t pc_raster_region_bytes(const PcRaster *r,PcRect rect)
{
    size_t w,h;
    if(!r||pc_rect_empty(rect))return 0;
    w=(size_t)pc_rect_width(rect);
    h=(size_t)pc_rect_height(rect);
    if(w>SIZE_MAX/r->bpp)return 0;
    w*=r->bpp;
    if(h>SIZE_MAX/w)return 0;
    return w*h;
}
int pc_raster_read_region(const PcRaster *r,PcRect rect,
                          uint8_t *out,size_t out_len)
{
    size_t need,row_bytes,w,h,y,x;
    if(!r||!out)return 0;
    need=pc_raster_region_bytes(r,rect);
    if(!need||need!=out_len)return 0;
    w=(size_t)pc_rect_width(rect);
    h=(size_t)pc_rect_height(rect);
    row_bytes=w*r->bpp;
    for(y=0;y<h;++y){
        int32_t gy=(int32_t)((int64_t)rect.y0+(int64_t)y);
        for(x=0;x<w;++x){
            int32_t gx=(int32_t)((int64_t)rect.x0+(int64_t)x);
            if(!pc_raster_read_pixel(r,gx,gy,out+y*row_bytes+x*r->bpp,
                                      r->bpp))return 0;
        }
    }
    return 1;
}
int pc_raster_write_region(PcRaster *r,PcRect rect,
                           const uint8_t *bytes,size_t len)
{
    size_t need,row_bytes,w,h,y,x;
    PcRaster *pending;
    PcRasterEntry *old_entries;
    size_t old_count,old_capacity;
    if(!r||!bytes)return 0;
    need=pc_raster_region_bytes(r,rect);
    if(!need||need!=len)return 0;
    w=(size_t)pc_rect_width(rect);
    h=(size_t)pc_rect_height(rect);
    row_bytes=w*r->bpp;
    /* Prepare all changes in a COW snapshot. Failure leaves original
     * entries, pixels and ownership intact, including later tile OOM. */
    pending=pc_raster_clone(r);
    if(!pending)return 0;
    for(y=0;y<h;++y){
        int32_t gy=(int32_t)((int64_t)rect.y0+(int64_t)y);
        for(x=0;x<w;++x){
            int32_t gx=(int32_t)((int64_t)rect.x0+(int64_t)x);
            if(!pc_raster_write_pixel(pending,gx,gy,
                 bytes+y*row_bytes+x*r->bpp,r->bpp)){
                pc_raster_destroy(pending);
                return 0;
            }
        }
    }
    /* No allocation after this point: commit exclusively the tile vector.
     * Keep the original object identity/document claim. */
    old_entries=r->entries;
    old_count=r->count;
    old_capacity=r->capacity;
    r->entries=pending->entries;
    r->count=pending->count;
    r->capacity=pending->capacity;
    pending->entries=old_entries;
    pending->count=old_count;
    pending->capacity=old_capacity;
    pc_raster_destroy(pending);
    return 1;
}
void pc_raster_prune(PcRaster *r)
{
    size_t i,j,keep=0;
    if(!r)return;
    for(i=0;i<r->count;++i){
        const uint8_t *b=r->entries[i].tile->bytes;
        int differs=0;
        for(j=0;j<r->tile_bytes;j+=r->bpp){
            if(memcmp(b+j,r->default_bytes,r->bpp)!=0){differs=1;break;}
        }
        if(differs)r->entries[keep++]=r->entries[i];
        else pc_tile_release(r->entries[i].tile);
    }
    r->count=keep;
}
