/* Differential output for original Rust doc::Group/Document::shift;
 * layout metadata only, no rendering or serialization assumptions. */
#include "pc_document.h"
#include <assert.h>
#include <stdio.h>
int main(void)
{
    PcPixelFormat fmt={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcDocument *d=pc_document_new("Groups",16,16),*clone;
    PcRaster *a=pc_raster_new(fmt,NULL),*b=pc_raster_new(fmt,NULL);
    assert(d&&a&&b);
    assert(pc_document_append_group(d,101,"Root group"));
    assert(pc_document_group_append_raster(d,101,102,"Bottom",a));
    assert(pc_document_group_append_group(d,101,103,"Child group"));
    assert(pc_document_group_append_raster(d,103,104,"Nested pix",b));
    printf("root %llu\n",(unsigned long long)pc_document_layer_id(d,0));
    printf("children %llu %llu\n",
      (unsigned long long)pc_document_group_child_id(d,101,0),
      (unsigned long long)pc_document_group_child_id(d,101,1));
    printf("inner %llu\n",
      (unsigned long long)pc_document_group_child_id(d,103,0));
    printf("shift %d\n",pc_document_group_shift_child(d,101,103,-1));
    printf("after %llu %llu\n",
      (unsigned long long)pc_document_group_child_id(d,101,0),
      (unsigned long long)pc_document_group_child_id(d,101,1));
    clone=pc_document_clone(d);
    assert(clone);
    printf("clone-shift %d\n",
           pc_document_group_shift_child(clone,101,103,1));
    printf("original %llu %llu\n",
      (unsigned long long)pc_document_group_child_id(d,101,0),
      (unsigned long long)pc_document_group_child_id(d,101,1));
    printf("snapshot %llu %llu\n",
      (unsigned long long)pc_document_group_child_id(clone,101,0),
      (unsigned long long)pc_document_group_child_id(clone,101,1));
    pc_document_destroy(clone);
    pc_document_destroy(d);
    return 0;
}
