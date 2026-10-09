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
    puts("PASS: PhotoCraft flat raster-layer document ownership/order/COW");
    return 0;
}
