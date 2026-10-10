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

/* Nested Rust-compatible group subset: bottom-first child order,
 * globally unique IDs, controlled depth, deep cloning and tile COW. */
static void test_group_tree(void)
{
    PcPixelFormat fmt={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const uint8_t red[4]={237,39,67,255},blue[4]={22,71,223,255};
    PcDocument *d=pc_document_new("Nested",32,64),*copy;
    PcRaster *owned=pc_raster_new(fmt,NULL);
    PcRaster *nested=pc_raster_new(fmt,NULL);
    PcRaster *rejected=pc_raster_new(fmt,NULL);
    uint64_t parent;
    size_t i;
    assert(d&&owned&&nested&&rejected);
    assert(pc_document_append_group(d,101,"Root group"));
    assert(pc_document_layer_count(d)==1);
    assert(pc_document_layer_id(d,0)==101);
    assert(pc_document_layer_raster(d,0)==NULL);
    assert(pc_document_group_child_count(d,101)==0);
    assert(pc_document_group_append_raster(d,101,102,"Bottom",owned));
    assert(pc_document_group_append_group(d,101,103,"Child group"));
    assert(pc_document_group_append_raster(d,103,104,"Nested pix",nested));
    assert(pc_document_group_child_count(d,101)==2);
    assert(pc_document_root_layer_count(d)==1);
    assert(pc_document_layer_count(d)==4); /* root + raster + group + raster */
    assert(pc_document_group_child_id(d,101,0)==102);
    assert(pc_document_group_child_id(d,101,1)==103);
    assert(pc_document_group_child_is_group(d,101,0)==0);
    assert(pc_document_group_child_is_group(d,101,1)==1);
    assert(strcmp(pc_document_group_child_name(d,101,1),"Child group")==0);
    assert(pc_document_group_child_is_group(d,101,2)==-1);
    assert(pc_document_group_child_id(d,101,99)==0);
    assert(pc_document_group_child_count(d,999)==0);
    assert(pc_document_group_child_raster(d,101,1)==NULL);
    assert(pc_document_group_child_raster(d,103,0)==nested);
    assert(!pc_document_append_raster(d,104,"ID collision",rejected));
    assert(!pc_document_group_append_raster(d,999,105,"No parent",rejected));
    assert(!pc_document_group_append_raster(d,102,105,"Raster is not group",rejected));
    assert(!pc_document_group_append_group(d,101,103,"Duplicate ID"));
    assert(!pc_document_group_append_group(d,101,101,"Ancestor ID"));
    assert(!pc_document_group_append_group(d,0,106,"Root via group-only API"));
    assert(pc_document_group_child_count(d,101)==2);
    assert(pc_raster_write_pixel(nested,-1,256,red,4));

    /* The public document-level shift must find the nested sibling list. */
    assert(pc_document_shift_layer(d,103,-1));
    assert(pc_document_group_child_id(d,101,0)==103);
    assert(pc_document_group_child_id(d,101,1)==102);
    assert(pc_document_shift_layer(d,103,1));
    assert(pc_document_group_shift_child(d,101,103,-1));
    assert(pc_document_group_child_id(d,101,0)==103);
    assert(pc_document_group_child_id(d,101,1)==102);
    assert(!pc_document_group_shift_child(d,101,103,-1));
    assert(!pc_document_group_shift_child(d,101,999,0));
    assert(!pc_document_group_shift_child(d,103,102,0));

    copy=pc_document_clone(d);
    assert(copy);
    assert(pc_document_group_child_id(copy,101,0)==103);
    assert(pc_document_group_child_id(copy,103,0)==104);
    assert(pc_document_group_child_raster(copy,103,0)!=nested);
    assert(pc_raster_write_pixel(pc_document_group_child_raster(copy,103,0),
                                 -1,256,blue,4));
    {
        uint8_t out[4];
        assert(pc_raster_read_pixel(nested,-1,256,out,4));
        assert(memcmp(out,red,4)==0);
        assert(pc_raster_read_pixel(pc_document_group_child_raster(copy,103,0),
                                    -1,256,out,4));
        assert(memcmp(out,blue,4)==0);
    }
    assert(pc_document_group_append_group(copy,101,105,"New"));
    assert(pc_document_group_child_count(copy,101)==3);
    assert(pc_document_group_child_count(d,101)==2);
    pc_document_destroy(d);
    assert(pc_document_group_child_id(copy,103,0)==104);
    pc_document_destroy(copy);
    pc_raster_destroy(rejected);

    /* Rust MAX_GROUP_DEPTH=100; a raster inside the deepest legal group
     * stays findable for the global ID uniqueness check. */
    d=pc_document_new("Depth",1,1);
    assert(d && pc_document_append_group(d,1,"level-1"));
    parent=1;
    for(i=2;i<=PC_DOCUMENT_MAX_GROUP_DEPTH;++i){
        assert(pc_document_group_append_group(d,parent,(uint64_t)i,"nest"));
        parent=(uint64_t)i;
    }
    assert(!pc_document_group_append_group(d,parent,200,"too deep"));
    owned=pc_raster_new(fmt,NULL);
    assert(owned && pc_document_group_append_raster(d,parent,201,"deep raster",owned));
    assert(pc_document_group_child_id(d,parent,0)==201);
    assert(pc_document_root_layer_count(d)==1);
    assert(pc_document_layer_count(d)==PC_DOCUMENT_MAX_GROUP_DEPTH+1);
    assert(!pc_document_append_group(d,201,"duplicate deep ID"));
    copy=pc_document_clone(d);
    assert(copy && pc_document_group_child_id(copy,parent,0)==201);
    pc_document_destroy(d);
    pc_document_destroy(copy);
}

/* Rust LayerMask subset: sparse Gray8 byte coverage, default 255/0,
 * enabled/linked flags and independent cloned mask surface ownership.
 * Density=1 and feather=0; no compositor is asserted by these tests. */
static void test_mask_contract(void)
{
    PcPixelFormat f={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcPixelFormat gray={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    const uint8_t painted=0,mid=128;
    PcDocument *d=pc_document_new("Mask document",20,30),*snapshot;
    PcRaster *pixels=pc_raster_new(f,NULL);
    PcRaster *invalid=pc_raster_new(f,NULL);
    PcRaster *mask=pc_document_mask_reveal_all();
    PcRaster *other=pc_document_mask_hide_all();
    PcRaster *group_mask=pc_raster_new(gray,&mid);
    PcRaster *taken;
    uint8_t coverage=77;
    assert(d&&pixels&&invalid&&mask&&other&&group_mask);
    assert(pc_document_append_group(d,101,"Parent"));
    assert(pc_document_group_append_raster(d,101,102,"Nested",pixels));
    assert(pc_document_mask_enabled(d,102)==-1);
    assert(!pc_document_mask_value_u8(d,102,-1,256,&coverage));
    assert(coverage==77);
    assert(!pc_document_attach_mask(d,0,mask,1,1));
    assert(!pc_document_attach_mask(d,999,mask,1,1));
    assert(!pc_document_attach_mask(d,102,invalid,1,1));
    assert(!pc_document_attach_mask(d,102,pixels,1,1));
    assert(pc_document_attach_mask(d,102,mask,1,1));
    assert(!pc_document_attach_mask(d,102,other,1,1));
    assert(!pc_document_attach_mask(d,101,mask,1,1));
    assert(pc_document_layer_mask(d,102)==mask);
    assert(pc_document_mask_enabled(d,102)==1);
    assert(pc_document_mask_linked(d,102)==1);
    assert(pc_document_mask_value_u8(d,102,300,-300,&coverage) && coverage==255);
    assert(pc_raster_write_pixel(mask,-1,256,&painted,1));
    assert(pc_document_mask_value_u8(d,102,-1,256,&coverage) && coverage==0);
    assert(pc_document_set_mask_enabled(d,102,0));
    assert(pc_document_mask_value_u8(d,102,-1,256,&coverage) && coverage==255);
    assert(pc_document_set_mask_enabled(d,102,1));
    assert(pc_document_set_mask_linked(d,102,0));
    assert(pc_document_mask_linked(d,102)==0);
    assert(pc_document_mask_value_u8(d,102,-1,256,&coverage) && coverage==0);

    /* The Rust mask field also exists on Group layers. */
    assert(pc_document_attach_mask(d,101,group_mask,1,1));
    assert(pc_document_mask_value_u8(d,101,-400,400,&coverage) && coverage==128);
    assert(pc_document_attach_mask(d,101,other,1,1)==0);
    snapshot=pc_document_clone(d);
    assert(snapshot);
    assert(pc_document_layer_mask(snapshot,102)!=mask);
    assert(pc_document_mask_linked(snapshot,102)==0);
    assert(pc_document_mask_enabled(snapshot,102)==1);
    assert(pc_document_mask_value_u8(snapshot,102,-1,256,&coverage) && coverage==0);
    assert(pc_raster_write_pixel(pc_document_layer_mask(snapshot,102),-1,256,&mid,1));
    assert(pc_document_mask_value_u8(snapshot,102,-1,256,&coverage) && coverage==128);
    assert(pc_document_mask_value_u8(d,102,-1,256,&coverage) && coverage==0);
    assert(pc_document_set_mask_linked(snapshot,102,1));
    assert(pc_document_mask_linked(d,102)==0);
    assert(pc_document_mask_value_u8(snapshot,101,0,0,&coverage) && coverage==128);

    /* Detach transfers ownership back to the caller without touching
     * the clone, and the original can then accept another mask. */
    taken=pc_document_detach_mask(d,102);
    assert(taken==mask);
    assert(pc_document_layer_mask(d,102)==NULL);
    assert(pc_document_mask_enabled(d,102)==-1);
    assert(!pc_document_set_mask_enabled(d,102,1));
    assert(!pc_document_mask_value_u8(d,102,-1,256,&coverage));
    pc_raster_destroy(taken);
    assert(pc_document_attach_mask(d,102,other,1,1));
    assert(pc_document_mask_value_u8(d,102,-1,256,&coverage) && coverage==0);
    pc_document_destroy(d);
    assert(pc_document_mask_value_u8(snapshot,102,-1,256,&coverage) && coverage==128);
    pc_document_destroy(snapshot);
    pc_raster_destroy(invalid);
    puts("PASS: PhotoCraft Gray8 LayerMask ownership/defaults/flags/deep COW");
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
    test_group_tree();
    test_mask_contract();
    puts("PASS: PhotoCraft flat raster-layer document ownership/order/COW/opacity/shift");
    puts("PASS: PhotoCraft bounded group hierarchy/deep COW/ID semantics");
    return 0;
}
