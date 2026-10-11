#include "pc_compose.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
static void out(const char *tag,const PcDocument *doc)
{
    uint8_t px[4];
    assert(pc_compose_rgba8_region(doc,pc_rect_new(0,0,1,1),px,4,4));
    printf("%s %u %u %u %u\n",tag,
        (unsigned)px[0],(unsigned)px[1],(unsigned)px[2],(unsigned)px[3]);
}
int main(void)
{
    PcPixelFormat f={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    uint8_t red[4]={255,0,0,255},blue[4]={0,0,255,255},value=128;
    PcDocument *doc=pc_document_new("Oracle",1,1);
    PcRaster *a=pc_raster_new(f,0),*b=pc_raster_new(f,0),*mask;
    assert(doc&&a&&b);
    assert(pc_raster_write_pixel(a,0,0,red,4));
    assert(pc_raster_write_pixel(b,0,0,blue,4));
    assert(pc_document_append_raster(doc,11,"Base",a));
    assert(pc_document_append_raster(doc,12,"Top",b));
    assert(pc_document_set_layer_opacity(doc,1,0.5f));
    out("half",doc);
    mask=pc_document_mask_reveal_all();
    assert(mask&&pc_document_attach_mask(doc,12,mask,1,1));
    assert(pc_raster_write_pixel(mask,0,0,&value,1));
    out("mask128",doc);
    assert(pc_document_set_layer_visible(doc,1,0));
    out("hidden",doc);
    pc_document_destroy(doc);
    return 0;
}
