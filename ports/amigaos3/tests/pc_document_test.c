#include "pc_document.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void pix(const PcRaster *r,const uint8_t expected[4])
{
    uint8_t actual[4]={0,0,0,0};
    assert(pc_raster_read_pixel(r,2,3,actual,4));
    assert(memcmp(actual,expected,4)==0);
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
    check_exclusive_raster_owner();
    puts("PASS: PhotoCraft flat raster-layer document ownership/order/COW");
    return 0;
}
