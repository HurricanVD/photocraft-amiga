#include "pc_raster.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PC_PIXEL_MAX 20u
#define PC_TILE_AREA ((size_t)PC_CORE_TILE_SIZE * (size_t)PC_CORE_TILE_SIZE)

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
    PcRasterEntry *entries;
    size_t count, capacity;
};

static size_t pc_find(const PcRaster *r, PcTileCoord c)
{
    size_t i;
    for (i=0; i<r->count; ++i)
        if (pc_tile_equal(r->entries[i].coord,c)) return i;
    return r->count;
}
static void pc_tile_release(PcRasterTile *t)
{
    if (t && --t->refs == 0) free(t);
}
static PcRasterTile *pc_tile_alloc(const PcRaster *r)
{
    PcRasterTile *tile;
    size_t i;
    if (r->tile_bytes > SIZE_MAX - sizeof(*tile)) return NULL;
    tile = (PcRasterTile *)malloc(sizeof(*tile)+r->tile_bytes);
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
    p=(PcRasterEntry *)realloc(r->entries,n*sizeof(*p));
    if(!p)return 0;
    r->entries=p;r->capacity=n;return 1;
}
PcRaster *pc_raster_new(PcPixelFormat fmt,const uint8_t *defaults)
{
    PcRaster *r;
    size_t bpp=pc_format_bytes_per_pixel(fmt);
    if(bpp==0 || bpp>PC_PIXEL_MAX || bpp>SIZE_MAX/PC_TILE_AREA ||
       (fmt.alpha!=0 && fmt.alpha!=1))return NULL;
    r=(PcRaster *)calloc(1,sizeof(*r));
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
        if(r->count>SIZE_MAX/sizeof(*copy->entries)){free(copy);return NULL;}
        copy->entries=(PcRasterEntry *)malloc(r->count*sizeof(*copy->entries));
        if(!copy->entries){free(copy);return NULL;}
        memcpy(copy->entries,r->entries,r->count*sizeof(*copy->entries));
        for(i=0;i<r->count;++i){
            if(copy->entries[i].tile->refs==SIZE_MAX){
                while(i) --copy->entries[--i].tile->refs;
                free(copy->entries);free(copy);return NULL;
            }
            ++copy->entries[i].tile->refs;
        }
        copy->count=copy->capacity=r->count;
    }
    return copy;
}
void pc_raster_destroy(PcRaster *r)
{
    size_t i;
    if(!r)return;
    for(i=0;i<r->count;++i)pc_tile_release(r->entries[i].tile);
    free(r->entries);free(r);
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
    PcRasterTile *tile;
    if(!r||!bytes||len!=r->bpp)return 0;
    c=pc_tile_containing(x,y);
    i=pc_find(r,c);
    if(i==r->count){
        /* Allocate tile before reserving/committing an entry. */
        tile=pc_tile_alloc(r);
        if(!tile)return 0;
        if(!pc_grow(r)){pc_tile_release(tile);return 0;}
        r->entries[i].coord=c;
        r->entries[i].tile=tile;
        ++r->count;
    }else{
        tile=r->entries[i].tile;
        if(tile->refs>1){
            PcRasterTile *new_tile;
            if(r->tile_bytes>SIZE_MAX-sizeof(*new_tile))return 0;
            new_tile=(PcRasterTile *)malloc(sizeof(*new_tile)+r->tile_bytes);
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
    if(!r||!bytes)return 0;
    need=pc_raster_region_bytes(r,rect);
    if(!need||need!=len)return 0;
    w=(size_t)pc_rect_width(rect);
    h=(size_t)pc_rect_height(rect);
    row_bytes=w*r->bpp;
    for(y=0;y<h;++y){
        int32_t gy=(int32_t)((int64_t)rect.y0+(int64_t)y);
        for(x=0;x<w;++x){
            int32_t gx=(int32_t)((int64_t)rect.x0+(int64_t)x);
            if(!pc_raster_write_pixel(r,gx,gy,
                 bytes+y*row_bytes+x*r->bpp,r->bpp))return 0;
        }
    }
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
