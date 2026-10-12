#include "pc_compose.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
int main(void)
{
    PcPixelFormat rgba={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcDocument *d=pc_document_new("Compositor",4,4);
    PcRaster *bottom=pc_raster_new(rgba,NULL),*top=pc_raster_new(rgba,NULL);
    const uint8_t red[4]={255,0,0,255};
    const uint8_t blue[4]={0,0,255,255};
    uint8_t result[20],old[20],coverage=0;
    PcRect area=pc_rect_new(0,0,1,1);
    assert(d && bottom && top);
    assert(pc_raster_write_pixel(bottom,0,0,red,4));
    assert(pc_raster_write_pixel(top,0,0,blue,4));
    assert(pc_document_append_raster(d,11,"Bottom",bottom));
    assert(pc_document_append_raster(d,12,"Top",top));
    assert(pc_document_set_layer_opacity(d,1,0.5f));
    memset(result,0xaa,sizeof(result));
    assert(pc_compose_rgba8_region(d,area,result,8,sizeof(result)));
    assert(result[0]==127 && result[1]==0 && result[2]==128 && result[3]==255);
    assert(result[4]==0xaa);
    assert(pc_document_set_layer_visible(d,1,0));
    assert(pc_compose_rgba8_region(d,area,result,8,sizeof(result)));
    assert(!memcmp(result,red,4));
    assert(pc_document_set_layer_visible(d,1,1));
    {
        PcRaster *mask=pc_document_mask_reveal_all();
        assert(mask);
        assert(pc_document_attach_mask(d,12,mask,1,1));
        assert(pc_raster_write_pixel(mask,0,0,&coverage,1));
        assert(pc_compose_rgba8_region(d,area,result,8,sizeof(result)));
        assert(!memcmp(result,red,4));
        assert(pc_document_set_mask_enabled(d,12,0));
        assert(pc_compose_rgba8_region(d,area,result,8,sizeof(result)));
        assert(result[0]==127 && result[2]==128);
    }
    memset(result,0xaa,sizeof(result));
    memcpy(old,result,sizeof(result));
    assert(!pc_compose_rgba8_region(d,area,result,3,sizeof(result)));
    assert(!pc_compose_rgba8_region(d,area,result,8,3));
    assert(!pc_compose_rgba8_region(d,pc_rect_new(-1,0,1,1),
                                    result,8,sizeof(result)));
    assert(!pc_compose_rgba8_region(d,pc_rect_new(0,0,INT_MAX,1),
                                    result,8,sizeof(result)));
    assert(memcmp(old,result,sizeof(result))==0);
    assert(pc_document_append_group(d,99,"Unsupported pass-through group"));
    assert(!pc_compose_rgba8_region(d,area,result,8,sizeof(result)));
    assert(memcmp(old,result,sizeof(result))==0);
    pc_document_destroy(d);
    puts("PASS: PhotoCraft scoped flat RGBA8 normal compositor/opacity/mask");
    return 0;
}
