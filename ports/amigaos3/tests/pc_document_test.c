#include "pc_document.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>

static void pix(const PcRaster *r,const uint8_t expected[4])
{
    uint8_t actual[4]={0,0,0,0};
    assert(pc_raster_read_pixel(r,2,3,actual,4));
    assert(memcmp(actual,expected,4)==0);
}
/* Original Rust doc::Document::shift direction, Layer defaults and clone
 * metadata. This is a flat-raster subset only, not full Document parity. */
static int same_float(float a,float b)
{
    uint32_t aa,bb;
    if(sizeof(a)!=sizeof(aa))return 0;
    memcpy(&aa,&a,sizeof(aa));
    memcpy(&bb,&b,sizeof(bb));
    return aa==bb;
}

static void test_layer_contract(void)
{
    PcPixelFormat f={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const uint8_t a[4]={3,5,7,255};
    PcDocument *d=pc_document_new("Three layers",20,30),*snap;
    PcRaster *r0=pc_raster_new(f,NULL),*r1=pc_raster_new(f,NULL);
    PcRaster *r2=pc_raster_new(f,NULL);
    assert(d&&r0&&r1&&r2);
    assert(pc_document_append_raster(d,11,"Bottom",r0));
    assert(pc_document_append_raster(d,22,"Middle",r1));
    assert(pc_document_append_raster(d,33,"Top",r2));

    /* Rust's +1 moves toward the visible top; -1 toward the bottom. */
    assert(pc_document_shift_layer(d,11,2));
    assert(pc_document_layer_id(d,0)==22);
    assert(pc_document_layer_id(d,1)==33);
    assert(pc_document_layer_id(d,2)==11);
    assert(pc_document_layer_raster(d,2)==r0);
    assert(strcmp(pc_document_layer_name(d,2),"Bottom")==0);
    assert(pc_document_shift_layer(d,11,-1));
    assert(pc_document_layer_id(d,0)==22);
    assert(pc_document_layer_id(d,1)==11);
    assert(pc_document_layer_id(d,2)==33);
    assert(pc_document_shift_layer(d,11,0));
    assert(!pc_document_shift_layer(d,11,2));
    assert(!pc_document_shift_layer(d,22,-1));
    assert(!pc_document_shift_layer(d,0,1));
    assert(!pc_document_shift_layer(d,444,0));
    assert(!pc_document_shift_layer(d,22,INT_MIN));
    assert(!pc_document_shift_layer(d,33,INT_MAX));
    assert(pc_document_layer_id(d,0)==22 && pc_document_layer_id(d,1)==11);
    assert(pc_document_layer_id(d,2)==33);
    assert(pc_document_layer_raster(d,0)==r1);
    assert(pc_document_layer_raster(d,1)==r0);
    assert(pc_document_layer_raster(d,2)==r2);

    assert(same_float(pc_document_layer_opacity(d,0),1.0f));
    assert(same_float(pc_document_layer_fill_opacity(d,0),1.0f));
    assert(same_float(pc_document_layer_opacity(NULL,0),-1.0f));
    assert(same_float(pc_document_layer_fill_opacity(d,3),-1.0f));
    assert(pc_document_set_layer_opacity(d,0,0.0f));
    assert(pc_document_set_layer_fill_opacity(d,0,0.75f));
    assert(pc_document_set_layer_opacity(d,1,0.5f));
    assert(pc_document_set_layer_fill_opacity(d,1,0.25f));
    assert(!pc_document_set_layer_opacity(d,0,-0.1f));
    assert(!pc_document_set_layer_opacity(d,0,1.01f));
    assert(!pc_document_set_layer_opacity(d,0,NAN));
    assert(!pc_document_set_layer_fill_opacity(d,0,INFINITY));
    assert(!pc_document_set_layer_fill_opacity(d,0,-0.01f));
    assert(!pc_document_set_layer_opacity(NULL,0,0.5f));
    assert(!pc_document_set_layer_fill_opacity(d,SIZE_MAX,0.5f));
    assert(same_float(pc_document_layer_opacity(d,0),0.0f));
    assert(same_float(pc_document_layer_fill_opacity(d,0),0.75f));

    assert(pc_raster_write_pixel(pc_document_layer_raster(d,1),2,3,a,4));
    snap=pc_document_clone(d);
    assert(snap && pc_document_layer_count(snap)==3);
    assert(pc_document_layer_id(snap,1)==11);
    assert(same_float(pc_document_layer_opacity(snap,0),0.0f));
    assert(same_float(pc_document_layer_fill_opacity(snap,1),0.25f));
    assert(pc_document_shift_layer(snap,33,-2));
    assert(pc_document_set_layer_visible(snap,2,0));
    assert(pc_document_set_layer_opacity(snap,1,1.0f));
    assert(pc_document_set_layer_fill_opacity(snap,1,1.0f));
    assert(pc_document_layer_id(snap,0)==33);
    assert(pc_document_layer_id(d,0)==22);
    assert(pc_document_layer_visible(d,2)==1);
    assert(same_float(pc_document_layer_opacity(d,1),0.5f));
    assert(same_float(pc_document_layer_fill_opacity(d,1),0.25f));
    pix(pc_document_layer_raster(snap,2),a);
    pc_document_destroy(d);
    pix(pc_document_layer_raster(snap,2),a);
    pc_document_destroy(snap);
}

/* PF-BG-001: lifetime ownership belongs to one document, not one layer ID.
 * Regression covers same-document and cross-document double-free vectors.
 */
static void check_exclusive_raster_owner(void)
{
    PcPixelFormat f={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const uint8_t red[4]={255,0,0,255};
    const uint8_t blue[4]={0,0,255,255};
    PcDocument *a=pc_document_new("Owner A",16,16);
    PcDocument *b=pc_document_new("Owner B",16,16);
    PcDocument *snapshot;
    PcRaster *original=pc_raster_new(f,NULL);
    PcRaster *other=pc_raster_new(f,NULL);
    PcRaster *separate;
    assert(a && b && original && other);
    assert(!pc_document_append_raster(a,0,"invalid ID",original));
    assert(!pc_document_append_raster(NULL,1,"invalid doc",original));
    assert(!pc_document_append_raster(a,1,NULL,original));
    assert(pc_document_append_raster(a,1,"Original",original));

    /* Each alias must fail while its first owner remains usable. */
    assert(!pc_document_append_raster(a,2,"duplicate pointer",original));
    assert(!pc_document_append_raster(b,3,"cross-document",original));
    assert(pc_document_layer_count(a)==1);
    assert(pc_document_layer_count(b)==0);
    assert(pc_document_layer_id(a,0)==1);
    assert(strcmp(pc_document_layer_name(a,0),"Original")==0);
    assert(pc_raster_write_pixel(pc_document_layer_raster(a,0),2,3,red,4));

    /* Duplicate ID rejected before ownership transfer; caller still owns other. */
    assert(!pc_document_append_raster(a,1,"duplicate ID",other));
    assert(pc_document_append_raster(b,4,"Different raster",other));
    assert(!pc_document_append_raster(a,5,"already in B",other));
    assert(pc_document_layer_count(b)==1);

    /* Clones are distinct raster owners, although tile contents share COW. */
    separate=pc_raster_clone(original);
    assert(separate);
    assert(pc_document_append_raster(a,6,"Fresh clone",separate));
    assert(!pc_document_append_raster(b,7,"clone claimed",separate));
    assert(pc_raster_write_pixel(pc_document_layer_raster(a,1),2,3,blue,4));
    pix(pc_document_layer_raster(a,0),red);
    pix(pc_document_layer_raster(a,1),blue);
    snapshot=pc_document_clone(a);
    assert(snapshot);
    assert(pc_document_layer_count(snapshot)==2);
    pc_document_destroy(a);
    pix(pc_document_layer_raster(snapshot,0),red);
    pix(pc_document_layer_raster(snapshot,1),blue);
    pc_document_destroy(b);
    pc_document_destroy(snapshot);
}

int main(void)
{
    PcPixelFormat f={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const uint8_t red[4]={255,0,0,255},green[4]={0,255,0,255};
    PcDocument *d=pc_document_new("Sample",640,480),*snapshot;
    PcRaster *bottom=pc_raster_new(f,NULL),*top=pc_raster_new(f,NULL);
    assert(d&&bottom&&top);
    assert(pc_document_width(d)==640 && pc_document_height(d)==480);
    assert(pc_document_append_raster(d,101,"Background",bottom));
    assert(!pc_document_append_raster(d,101,"duplicate id",top));
    assert(pc_document_append_raster(d,102,"Foreground",top));
    assert(pc_document_layer_count(d)==2);
    assert(pc_document_layer_id(d,0)==101 && pc_document_layer_id(d,1)==102);
    assert(strcmp(pc_document_layer_name(d,0),"Background")==0);
    assert(strcmp(pc_document_layer_name(d,1),"Foreground")==0);
    assert(pc_raster_write_pixel(pc_document_layer_raster(d,0),2,3,red,4));
    assert(pc_raster_write_pixel(pc_document_layer_raster(d,1),2,3,green,4));
    snapshot=pc_document_clone(d);assert(snapshot);
    assert(pc_document_set_layer_visible(snapshot,1,0));
    assert(pc_document_layer_visible(snapshot,1)==0);
    assert(pc_document_layer_visible(d,1)==1);
    pix(pc_document_layer_raster(snapshot,0),red);
    assert(pc_raster_write_pixel(pc_document_layer_raster(snapshot,0),2,3,green,4));
    pix(pc_document_layer_raster(d,0),red);
    pix(pc_document_layer_raster(snapshot,0),green);
    pc_document_destroy(d);
    pix(pc_document_layer_raster(snapshot,1),green);
    pc_document_destroy(snapshot);
    test_layer_contract();
    check_exclusive_raster_owner();
    puts("PASS: PhotoCraft flat raster-layer document ownership/order/COW/opacity/shift");
    return 0;
}
