/* Independent C99 oracle for original Rust doc::LayerMask semantics.
 * Restricted to density=1, feather=0, Gray8 U8 encoded mask coverage. */
#include "pc_document.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
static unsigned coverage(const PcDocument *d,int32_t x,int32_t y)
{
    uint8_t value=0;
    assert(pc_document_mask_value_u8(d,102,x,y,&value));
    return (unsigned)value;
}
int main(void)
{
    PcPixelFormat fmt={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const uint8_t black=0,mid=128;
    PcDocument *d=pc_document_new("Mask document",20,30),*copy;
    PcRaster *pixels=pc_raster_new(fmt,NULL);
    PcRaster *mask=pc_document_mask_reveal_all();
    PcRaster *taken;
    assert(d&&pixels&&mask);
    assert(pc_document_append_group(d,101,"Parent"));
    assert(pc_document_group_append_raster(d,101,102,"Nested",pixels));
    assert(pc_document_attach_mask(d,102,mask,1,1));
    printf("default %u\n",coverage(d,300,-300));
    assert(pc_raster_write_pixel(mask,-1,256,&black,1));
    printf("painted %u\n",coverage(d,-1,256));
    assert(pc_document_set_mask_enabled(d,102,0));
    printf("disabled %u\n",coverage(d,-1,256));
    assert(pc_document_set_mask_enabled(d,102,1));
    assert(pc_document_set_mask_linked(d,102,0));
    printf("linked %d\n",pc_document_mask_linked(d,102));
    printf("enabled %u\n",coverage(d,-1,256));
    copy=pc_document_clone(d);
    assert(copy);
    assert(pc_raster_write_pixel(pc_document_layer_mask(copy,102),-1,256,&mid,1));
    printf("clone %u\n",coverage(copy,-1,256));
    printf("original %u\n",coverage(d,-1,256));
    taken=pc_document_detach_mask(d,102);
    printf("detached %d\n",taken!=NULL);
    assert(taken==mask);
    pc_raster_destroy(taken);
    printf("snapshot-after-detach %u\n",coverage(copy,-1,256));
    pc_document_destroy(d);
    pc_document_destroy(copy);
    return 0;
}
