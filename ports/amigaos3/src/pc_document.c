#include "pc_document.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <float.h>
typedef struct PcLayer {
    uint64_t id;
    char *name;
    PcRaster *surface;
    int visible;
    float opacity;
    float fill_opacity;
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
PcDocument *pc_document_new(const char *name,uint32_t width,uint32_t height)
{
    PcDocument *d;
    if(!name||!width||!height)return NULL;
    d=(PcDocument *)calloc(1,sizeof(*d));
    if(!d)return NULL;
    d->name=copy_name(name);
    if(!d->name){free(d);return NULL;}
    d->width=width;d->height=height;return d;
}
void pc_document_destroy(PcDocument *d)
{
    size_t i;
    if(!d)return;
    for(i=0;i<d->count;++i){
        free(d->layers[i].name);
        pc_raster_destroy(d->layers[i].surface);
    }
    free(d->layers);free(d->name);free(d);
}
size_t pc_document_layer_count(const PcDocument *d){return d?d->count:0;}
uint32_t pc_document_width(const PcDocument *d){return d?d->width:0;}
uint32_t pc_document_height(const PcDocument *d){return d?d->height:0;}
int pc_document_append_raster(PcDocument *d,uint64_t id,
                              const char *name,PcRaster *owned)
{
    size_t i,n;
    char *label;
    PcLayer *array;
    if(!d||!name||!owned||id==0)return 0;
    for(i=0;i<d->count;++i)if(d->layers[i].id==id)return 0;
    label=copy_name(name);
    if(!label)return 0;
    if(d->count==d->capacity){
        n=d->capacity?d->capacity*2:4;
        if(n<d->capacity||n>SIZE_MAX/sizeof(*array)){free(label);return 0;}
        array=(PcLayer *)realloc(d->layers,n*sizeof(*array));
        if(!array){free(label);return 0;}
        d->layers=array;d->capacity=n;
    }
    d->layers[d->count].id=id;
    d->layers[d->count].name=label;
    d->layers[d->count].visible=1;
    d->layers[d->count].opacity=1.0f;
    d->layers[d->count].fill_opacity=1.0f;
    d->layers[d->count].surface=owned;
    ++d->count;
    return 1;
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
    d->layers[i].visible=(visible!=0);return 1;
}
/* The indices and ordering mirror Rust doc::Document::shift:
 * 0 is bottom; a positive delta raises a layer. The range checks avoid
 * signed overflow (including INT_MIN) and forbid any partial mutation. */
int pc_document_shift_layer(PcDocument *d,uint64_t id,int delta)
{
    size_t from,to,i,steps;
    PcLayer moved;
    if(!d || id==0)return 0;
    for(from=0;from<d->count && d->layers[from].id!=id;++from){}
    if(from==d->count)return 0;
    if(delta>=0){
        steps=(size_t)delta;
        if(steps>=d->count-from)return 0;
        to=from+steps;
    }else{
        steps=(size_t)(-(int64_t)delta);
        if(steps>from)return 0;
        to=from-steps;
    }
    if(from==to)return 1;
    moved=d->layers[from];
    if(to>from){
        for(i=from;i<to;++i)d->layers[i]=d->layers[i+1];
    }else{
        for(i=from;i>to;--i)d->layers[i]=d->layers[i-1];
    }
    d->layers[to]=moved;
    return 1;
}
/* The frozen PF-SP-002 raw metadata profile uses IEEE-754 binary32
 * floats (as do the original Rust f32 fields). Inspect bits rather than
 * using floating-point comparisons: GCC13/m68k -msoft-float under the
 * tested -noixemul target does not link __lesf2/__gesf2 helpers.
 * Both +0 and -0 are valid, nonfinite and values outside [0,1] fail.
 * Byte copying keeps this independent of native endianness. */
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
    d->layers[i].opacity=opacity;
    return 1;
}
float pc_document_layer_fill_opacity(const PcDocument *d,size_t i)
{
    return d&&i<d->count?d->layers[i].fill_opacity:-1.0f;
}
int pc_document_set_layer_fill_opacity(PcDocument *d,size_t i,float opacity)
{
    if(!d||i>=d->count||!pc_valid_opacity(opacity))return 0;
    d->layers[i].fill_opacity=opacity;
    return 1;
}

PcDocument *pc_document_clone(const PcDocument *orig)
{
    PcDocument *d;
    size_t i;
    if(!orig)return NULL;
    d=pc_document_new(orig->name,orig->width,orig->height);
    if(!d)return NULL;
    for(i=0;i<orig->count;++i){
        PcRaster *copy=pc_raster_clone(orig->layers[i].surface);
        if(!copy){pc_document_destroy(d);return NULL;}
        if(!pc_document_append_raster(d,orig->layers[i].id,orig->layers[i].name,copy)){
            pc_raster_destroy(copy);pc_document_destroy(d);return NULL;
        }
        d->layers[i].visible=orig->layers[i].visible;
        d->layers[i].opacity=orig->layers[i].opacity;
        d->layers[i].fill_opacity=orig->layers[i].fill_opacity;
    }
    return d;
}
