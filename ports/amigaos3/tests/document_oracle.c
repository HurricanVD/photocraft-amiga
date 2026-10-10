/* Flat layer metadata oracle against original PhotoCraft Rust doc::Document.
 * Does not assert a complete PhotoCraft Document or public ABI. */
#include "pc_document.h"
#include <assert.h>
#include <stdio.h>

static void order(const char *label,const PcDocument *d)
{
    printf("%s %llu %llu %llu\n",label,
        (unsigned long long)pc_document_layer_id(d,0),
        (unsigned long long)pc_document_layer_id(d,1),
        (unsigned long long)pc_document_layer_id(d,2));
}
int main(void)
{
    PcPixelFormat fmt={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcDocument *d=pc_document_new("Three layers",20,30);
    PcDocument *snapshot;
    PcRaster *a=pc_raster_new(fmt,NULL);
    PcRaster *b=pc_raster_new(fmt,NULL);
    PcRaster *c=pc_raster_new(fmt,NULL);
    int ok;
    assert(d&&a&&b&&c);
    assert(pc_document_append_raster(d,11,"Bottom",a));
    assert(pc_document_append_raster(d,22,"Middle",b));
    assert(pc_document_append_raster(d,33,"Top",c));

    order("initial",d);
    printf("defaults %.2f %.2f\n",pc_document_layer_opacity(d,0),
                                   pc_document_layer_fill_opacity(d,0));
    ok=pc_document_shift_layer(d,11,2);
    printf("raise %d\n",ok);
    order("raised",d);
    ok=pc_document_shift_layer(d,11,-1);
    printf("lower %d\n",ok);
    order("lowered",d);
    printf("boundary %d\n",pc_document_shift_layer(d,11,2));
    printf("absent %d\n",pc_document_shift_layer(d,444,0));
    printf("zero %d\n",pc_document_shift_layer(d,11,0));
    order("stayed",d);

    assert(pc_document_set_layer_opacity(d,0,0.0f));
    assert(pc_document_set_layer_fill_opacity(d,0,0.75f));
    assert(pc_document_set_layer_opacity(d,1,0.5f));
    assert(pc_document_set_layer_fill_opacity(d,1,0.25f));
    printf("meta %.2f %.2f %.2f %.2f\n",
           pc_document_layer_opacity(d,0),
           pc_document_layer_fill_opacity(d,0),
           pc_document_layer_opacity(d,1),
           pc_document_layer_fill_opacity(d,1));

    snapshot=pc_document_clone(d);
    assert(snapshot);
    printf("clone-shift %d\n",pc_document_shift_layer(snapshot,33,-2));
    assert(pc_document_set_layer_opacity(snapshot,1,1.0f));
    assert(pc_document_set_layer_fill_opacity(snapshot,1,1.0f));
    order("original",d);
    order("snapshot",snapshot);
    printf("unchanged %.2f %.2f\n",
           pc_document_layer_opacity(d,1),
           pc_document_layer_fill_opacity(d,1));
    pc_document_destroy(snapshot);
    pc_document_destroy(d);
    return 0;
}
