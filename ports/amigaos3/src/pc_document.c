#include "pc_document.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>

/* PF-SP-002: independent, bounded tree subset of Rust doc::LayerContent.
 * Each child array owns its elements; raster surfaces are owned and cloned
 * through PcRaster's COW interface. Groups are containers, not raster layers.
 */
typedef struct PcLayer {
    uint64_t id;
    char *name;
    PcRaster *surface;
    PcRaster *mask;
    int mask_enabled,mask_linked;
    int visible;
    float opacity;
    float fill_opacity;
    int is_group;
    struct PcLayer *children;
    size_t count,capacity;
} PcLayer;
struct PcDocument {
    char *name;
    uint32_t width,height;
    PcLayer *layers;
    size_t count,capacity;
};

static char *copy_name(const char *s)
{
    size_t n;
    char *p;
    if(!s)return NULL;
    n=strlen(s);
    if(n==SIZE_MAX)return NULL;
    p=(char *)malloc(n+1);
    if(p)memcpy(p,s,n+1);
    return p;
}
static void pc_layer_release(PcLayer *layer)
{
    size_t i;
    if(!layer)return;
    for(i=0;i<layer->count;++i)pc_layer_release(&layer->children[i]);
    free(layer->children);
    pc_raster_destroy(layer->surface);
    pc_raster_destroy(layer->mask);
    free(layer->name);
    memset(layer,0,sizeof(*layer));
}
static PcLayer *pc_find_layer(PcLayer *items,size_t count,uint64_t id,
                              size_t depth,size_t *found_depth)
{
    size_t i;
    PcLayer *hit;
    for(i=0;i<count;++i){
        if(items[i].id==id){
            if(found_depth)*found_depth=depth;
            return &items[i];
        }
        if(items[i].is_group && depth<=PC_DOCUMENT_MAX_GROUP_DEPTH){
            hit=pc_find_layer(items[i].children,items[i].count,id,
                              depth+1,found_depth);
            if(hit)return hit;
        }
    }
    return NULL;
}
/* A document owns each PcRaster pointer at most once. Otherwise
 * cloning or destruction could double-free an aliased mask/surface. */
static int pc_owns_surface(const PcLayer *items,size_t count,
                            const PcRaster *surface)
{
    size_t i;
    for(i=0;i<count;++i){
        if(items[i].surface==surface || items[i].mask==surface)return 1;
        if(items[i].is_group &&
           pc_owns_surface(items[i].children,items[i].count,surface))
            return 1;
    }
    return 0;
}

static PcLayer *pc_group(PcDocument *d,uint64_t id,size_t *depth)
{
    PcLayer *layer;
    if(!d||id==0)return NULL;
    layer=pc_find_layer(d->layers,d->count,id,1,depth);
    return layer && layer->is_group?layer:NULL;
}
/* Commit the new child only after label copy and vector growth succeed.
 * On failure, the caller still owns 'surface'. */
static int pc_append(PcLayer **items,size_t *count,size_t *capacity,
                     uint64_t id,const char *name,PcRaster *surface,
                     int is_group)
{
    PcLayer *array,*slot;
    size_t n;
    char *label=copy_name(name);
    if(!label)return 0;
    if(*count==*capacity){
        n=*capacity?*capacity*2:4;
        if(n<*capacity||n>SIZE_MAX/sizeof(**items)){
            free(label);return 0;
        }
        array=(PcLayer *)realloc(*items,n*sizeof(**items));
        if(!array){free(label);return 0;}
        *items=array;*capacity=n;
    }
    slot=&(*items)[*count];
    memset(slot,0,sizeof(*slot));
    slot->id=id;
    slot->name=label;
    slot->surface=surface;
    slot->is_group=is_group;
    slot->visible=1;
    slot->opacity=1.0f;
    slot->fill_opacity=1.0f;
    ++*count;
    return 1;
}
static int pc_document_append(PcDocument *d,uint64_t parent_id,
                              uint64_t id,const char *name,
                              PcRaster *surface,int is_group)
{
    PcLayer *parent;
    size_t depth=0;
    if(!d||!name||id==0||(!surface&&!is_group)||(surface&&is_group))
        return 0;
    if(pc_find_layer(d->layers,d->count,id,1,NULL))return 0;
    if(surface && pc_owns_surface(d->layers,d->count,surface))return 0;
    if(parent_id==0){
        return pc_append(&d->layers,&d->count,&d->capacity,
                         id,name,surface,is_group);
    }
    parent=pc_group(d,parent_id,&depth);
    if(!parent)return 0;
    /* 100 nested groups is the original Rust MAX_GROUP_DEPTH. A raster
     * may live inside the hundredth group but another group may not. */
    if(is_group && depth>=PC_DOCUMENT_MAX_GROUP_DEPTH)return 0;
    return pc_append(&parent->children,&parent->count,&parent->capacity,
                     id,name,surface,is_group);
}

PcDocument *pc_document_new(const char *name,uint32_t width,uint32_t height)
{
    PcDocument *d;
    if(!name||!width||!height)return NULL;
    d=(PcDocument *)calloc(1,sizeof(*d));
    if(!d)return NULL;
    d->name=copy_name(name);
    if(!d->name){free(d);return NULL;}
    d->width=width;d->height=height;
    return d;
}
void pc_document_destroy(PcDocument *d)
{
    size_t i;
    if(!d)return;
    for(i=0;i<d->count;++i)pc_layer_release(&d->layers[i]);
    free(d->layers);free(d->name);free(d);
}
/* Root index APIs still address only root siblings. The legacy
 * root-only count is retained explicitly; Rust Document::layer_count
 * includes the root and all nested descendants. */
size_t pc_document_root_layer_count(const PcDocument *d){return d?d->count:0;}
static size_t pc_count_tree(const PcLayer *items,size_t count)
{
    size_t i,total=count;
    for(i=0;i<count;++i){
        if(items[i].is_group){
            size_t kids=pc_count_tree(items[i].children,items[i].count);
            if(kids>SIZE_MAX-total)return SIZE_MAX;
            total+=kids;
        }
    }
    return total;
}
size_t pc_document_layer_count(const PcDocument *d)
{
    return d?pc_count_tree(d->layers,d->count):0;
}
uint32_t pc_document_width(const PcDocument *d){return d?d->width:0;}
uint32_t pc_document_height(const PcDocument *d){return d?d->height:0;}

int pc_document_append_raster(PcDocument *d,uint64_t id,
                              const char *name,PcRaster *surface)
{
    return pc_document_append(d,0,id,name,surface,0);
}
int pc_document_append_group(PcDocument *d,uint64_t id,const char *name)
{
    return pc_document_append(d,0,id,name,NULL,1);
}
int pc_document_group_append_raster(PcDocument *d,uint64_t parent_id,
                                    uint64_t id,const char *name,
                                    PcRaster *surface)
{
    if(parent_id==0)return 0;
    return pc_document_append(d,parent_id,id,name,surface,0);
}
int pc_document_group_append_group(PcDocument *d,uint64_t parent_id,
                                   uint64_t id,const char *name)
{
    if(parent_id==0)return 0;
    return pc_document_append(d,parent_id,id,name,NULL,1);
}
size_t pc_document_group_child_count(const PcDocument *d,uint64_t parent_id)
{
    PcLayer *group=pc_group((PcDocument *)d,parent_id,NULL);
    return group?group->count:0;
}
uint64_t pc_document_group_child_id(const PcDocument *d,uint64_t parent_id,
                                    size_t index)
{
    PcLayer *group=pc_group((PcDocument *)d,parent_id,NULL);
    return group && index<group->count?group->children[index].id:0;
}
int pc_document_group_child_is_group(const PcDocument *d,uint64_t parent_id,
                                     size_t index)
{
    PcLayer *group=pc_group((PcDocument *)d,parent_id,NULL);
    return group && index<group->count?group->children[index].is_group:-1;
}
PcRaster *pc_document_group_child_raster(PcDocument *d,uint64_t parent_id,
                                         size_t index)
{
    PcLayer *group=pc_group(d,parent_id,NULL);
    return group && index<group->count?group->children[index].surface:NULL;
}
const char *pc_document_group_child_name(const PcDocument *d,
                                          uint64_t parent_id,size_t index)
{
    PcLayer *group=pc_group((PcDocument *)d,parent_id,NULL);
    return group && index<group->count?group->children[index].name:NULL;
}
uint64_t pc_document_layer_id(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].id:0;
}
const char *pc_document_layer_name(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].name:NULL;
}
PcRaster *pc_document_layer_raster(PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].surface:NULL;
}
int pc_document_layer_visible(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].visible:-1;
}
int pc_document_set_layer_visible(PcDocument *d,size_t i,int visible)
{
    if(!d||i>=d->count)return 0;
    d->layers[i].visible=(visible!=0);
    return 1;
}
static int pc_shift(PcLayer *items,size_t count,uint64_t id,int delta)
{
    size_t from,to,i,steps;
    PcLayer moved;
    for(from=0;from<count && items[from].id!=id;++from){}
    if(from==count)return 0;
    if(delta>=0){
        steps=(size_t)delta;
        if(steps>=count-from)return 0;
        to=from+steps;
    }else{
        steps=(size_t)(-(int64_t)delta);
        if(steps>from)return 0;
        to=from-steps;
    }
    if(from==to)return 1;
    moved=items[from];
    if(to>from){
        for(i=from;i<to;++i)items[i]=items[i+1];
    }else{
        for(i=from;i>to;--i)items[i]=items[i-1];
    }
    items[to]=moved;
    return 1;
}
/* Only reorders siblings; no cross-parent ownership transfer. */
/* Locate the actual sibling vector of a node, as original
 * Document::shift(LayerId) does; never change parent ownership. */
static int pc_find_siblings(PcLayer *items,size_t count,uint64_t id,
                             size_t depth,PcLayer **siblings,size_t *length)
{
    size_t i;
    for(i=0;i<count;++i){
        if(items[i].id==id){
            *siblings=items;*length=count;return 1;
        }
        if(items[i].is_group && depth<=PC_DOCUMENT_MAX_GROUP_DEPTH &&
           pc_find_siblings(items[i].children,items[i].count,id,
                             depth+1,siblings,length))return 1;
    }
    return 0;
}
int pc_document_shift_layer(PcDocument *d,uint64_t id,int delta)
{
    PcLayer *siblings=NULL;
    size_t count=0;
    if(!d||id==0 ||
       !pc_find_siblings(d->layers,d->count,id,1,&siblings,&count))return 0;
    return pc_shift(siblings,count,id,delta);
}
int pc_document_group_shift_child(PcDocument *d,uint64_t parent_id,
                                  uint64_t child_id,int delta)
{
    PcLayer *group=pc_group(d,parent_id,NULL);
    if(!group||child_id==0)return 0;
    return pc_shift(group->children,group->count,child_id,delta);
}
/* The original Rust f32 metadata contract is IEEE-754 binary32. A bit check
 * avoids unavailable __lesf2/__gesf2 soft-float helpers on GCC13 m68k.
 * Plus and minus zero are legal; negative, >1, NaN and infinity are not. */
static int pc_valid_opacity(float value)
{
    uint32_t bits;
    if(sizeof(value)!=sizeof(bits) || FLT_RADIX!=2 ||
       FLT_MANT_DIG!=24)return 0;
    memcpy(&bits,&value,sizeof(bits));
    if((bits & UINT32_C(0x7fffffff))==0)return 1;
    return (bits & UINT32_C(0x80000000))==0 &&
           bits<=UINT32_C(0x3f800000);
}
float pc_document_layer_opacity(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].opacity:-1.0f;
}
int pc_document_set_layer_opacity(PcDocument *d,size_t i,float opacity)
{
    if(!d||i>=d->count||!pc_valid_opacity(opacity))return 0;
    d->layers[i].opacity=opacity;return 1;
}
float pc_document_layer_fill_opacity(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].fill_opacity:-1.0f;
}
int pc_document_set_layer_fill_opacity(PcDocument *d,size_t i,float opacity)
{
    if(!d||i>=d->count||!pc_valid_opacity(opacity))return 0;
    d->layers[i].fill_opacity=opacity;return 1;
}

/* LayerMask::reveal_all/hide_all in the upstream Rust model are
 * Gray8 sparse surfaces with default 255/0 and no per-tile allocation. */
static PcRaster *pc_document_mask_default(uint8_t value)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    return pc_raster_new(fmt,&value);
}
PcRaster *pc_document_mask_reveal_all(void)
{
    return pc_document_mask_default(255);
}
PcRaster *pc_document_mask_hide_all(void)
{
    return pc_document_mask_default(0);
}
int pc_document_attach_mask(PcDocument *d,uint64_t id,PcRaster *mask,
                            int enabled,int linked)
{
    PcPixelFormat fmt;
    PcLayer *layer;
    if(!d||!mask||id==0)return 0;
    if(!pc_raster_get_format(mask,&fmt) ||
       fmt.mode!=PC_COLOR_GRAY || fmt.sample!=PC_SAMPLE_U8 ||
       fmt.alpha!=0)return 0;
    layer=pc_find_layer(d->layers,d->count,id,1,NULL);
    if(!layer||layer->mask || pc_owns_surface(d->layers,d->count,mask))
        return 0;
    /* No operation after this point can fail. */
    layer->mask=mask;
    layer->mask_enabled=(enabled!=0);
    layer->mask_linked=(linked!=0);
    return 1;
}
PcRaster *pc_document_layer_mask(PcDocument *d,uint64_t id)
{
    PcLayer *layer;
    if(!d||id==0)return NULL;
    layer=pc_find_layer(d->layers,d->count,id,1,NULL);
    return layer?layer->mask:NULL;
}
PcRaster *pc_document_detach_mask(PcDocument *d,uint64_t id)
{
    PcLayer *layer;
    PcRaster *mask;
    if(!d||id==0)return NULL;
    layer=pc_find_layer(d->layers,d->count,id,1,NULL);
    if(!layer)return NULL;
    mask=layer->mask;
    layer->mask=NULL;
    layer->mask_enabled=0;
    layer->mask_linked=0;
    return mask;
}
int pc_document_mask_enabled(const PcDocument *d,uint64_t id)
{
    PcLayer *layer;
    if(!d||id==0)return -1;
    layer=pc_find_layer(((PcDocument *)d)->layers,d->count,id,1,NULL);
    return layer&&layer->mask?layer->mask_enabled:-1;
}
int pc_document_set_mask_enabled(PcDocument *d,uint64_t id,int enabled)
{
    PcLayer *layer;
    if(!d||id==0)return 0;
    layer=pc_find_layer(d->layers,d->count,id,1,NULL);
    if(!layer||!layer->mask)return 0;
    layer->mask_enabled=(enabled!=0);
    return 1;
}
int pc_document_mask_linked(const PcDocument *d,uint64_t id)
{
    PcLayer *layer;
    if(!d||id==0)return -1;
    layer=pc_find_layer(((PcDocument *)d)->layers,d->count,id,1,NULL);
    return layer&&layer->mask?layer->mask_linked:-1;
}
int pc_document_set_mask_linked(PcDocument *d,uint64_t id,int linked)
{
    PcLayer *layer;
    if(!d||id==0)return 0;
    layer=pc_find_layer(d->layers,d->count,id,1,NULL);
    if(!layer||!layer->mask)return 0;
    layer->mask_linked=(linked!=0);
    return 1;
}
int pc_document_mask_value_u8(const PcDocument *d,uint64_t id,
                              int32_t x,int32_t y,uint8_t *out)
{
    PcLayer *layer;
    if(!d||id==0||!out)return 0;
    layer=pc_find_layer(((PcDocument *)d)->layers,d->count,id,1,NULL);
    if(!layer||!layer->mask)return 0;
    if(!layer->mask_enabled){*out=255;return 1;}
    return pc_raster_read_pixel(layer->mask,x,y,out,1);
}

/* Recursive clone terminates at PC_DOCUMENT_MAX_GROUP_DEPTH, preserving
 * independent child arrays/names and refcounted COW raster snapshots. */
static int pc_layer_clone(PcLayer *out,const PcLayer *src)
{
    size_t i;
    memset(out,0,sizeof(*out));
    out->id=src->id;
    out->name=copy_name(src->name);
    if(!out->name)return 0;
    out->visible=src->visible;
    out->opacity=src->opacity;
    out->fill_opacity=src->fill_opacity;
    out->is_group=src->is_group;
    out->mask_enabled=src->mask_enabled;
    out->mask_linked=src->mask_linked;
    if(src->surface){
        out->surface=pc_raster_clone(src->surface);
        if(!out->surface)goto fail;
    }
    if(src->mask){
        out->mask=pc_raster_clone(src->mask);
        if(!out->mask)goto fail;
    }
    if(src->count){
        if(src->count>SIZE_MAX/sizeof(*out->children))goto fail;
        out->children=(PcLayer *)calloc(src->count,sizeof(*out->children));
        if(!out->children)goto fail;
        out->capacity=src->count;
        for(i=0;i<src->count;++i){
            if(!pc_layer_clone(&out->children[i],&src->children[i]))goto fail;
            ++out->count;
        }
    }
    return 1;
fail:
    pc_layer_release(out);
    return 0;
}
PcDocument *pc_document_clone(const PcDocument *orig)
{
    PcDocument *d;
    size_t i;
    if(!orig)return NULL;
    d=pc_document_new(orig->name,orig->width,orig->height);
    if(!d)return NULL;
    if(orig->count){
        if(orig->count>SIZE_MAX/sizeof(*d->layers)){
            pc_document_destroy(d);return NULL;
        }
        d->layers=(PcLayer *)calloc(orig->count,sizeof(*d->layers));
        if(!d->layers){pc_document_destroy(d);return NULL;}
        d->capacity=orig->count;
        for(i=0;i<orig->count;++i){
            if(!pc_layer_clone(&d->layers[i],&orig->layers[i])){
                pc_document_destroy(d);return NULL;
            }
            ++d->count;
        }
    }
    return d;
}
