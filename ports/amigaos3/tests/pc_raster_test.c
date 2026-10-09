#include "pc_raster.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static void check_one(PcPixelFormat fmt)
{
    uint8_t data[100],got[100],first[20],mark[20],missing[20];
    uint8_t origin[20];
    PcRaster *r,*clone;
    size_t i,n,bpp=pc_format_bytes_per_pixel(fmt);
    PcRect area=pc_rect_new(-1,255,1,257);
    assert(bpp&&bpp<=20);
    n=pc_raster_region_bytes(pc_raster_new(fmt,NULL),area);
    /* Proper allocation and destruction are checked below; avoid
     * introducing unowned transient rasters in memory-budgeted tests. */
    assert(n==bpp*4);
    r=pc_raster_new(fmt,NULL);assert(r);
    assert(pc_raster_tile_count(r)==0);
    assert(!pc_raster_read_region(r,area,got,n-1));
    assert(pc_raster_read_region(r,area,got,n));
    for(i=0;i<n;++i)assert(got[i]==0);
    for(i=0;i<n;++i)data[i]=(uint8_t)((i*31+7)%251);
    assert(pc_raster_write_region(r,area,data,n));
    assert(pc_raster_tile_count(r)==4);
    assert(pc_raster_read_region(r,area,got,n));
    assert(memcmp(data,got,n)==0);
    clone=pc_raster_clone(r);assert(clone);
    assert(pc_raster_tile_count(clone)==4);
    memcpy(origin,data+(3*bpp),bpp);
    memset(mark,0xa5,bpp);
    assert(pc_raster_write_pixel(clone,0,256,mark,bpp));
    assert(pc_raster_read_pixel(r,0,256,first,bpp));
    assert(memcmp(first,origin,bpp)==0);
    assert(pc_raster_read_pixel(clone,0,256,first,bpp));
    assert(memcmp(first,mark,bpp)==0);
    assert(!pc_raster_write_pixel(clone,3,3,mark,bpp-1));
    assert(pc_raster_read_pixel(clone,1234,-1234,missing,bpp));
    for(i=0;i<bpp;++i)assert(missing[i]==0);
    pc_raster_destroy(r);
    assert(pc_raster_read_pixel(clone,-1,255,first,bpp));
    assert(memcmp(first,data,bpp)==0);
    pc_raster_destroy(clone);
}
static void check_default(void)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,1};
    uint8_t def[2]={128,255},out[2],change[2]={0,0};
    PcRaster *r=pc_raster_new(fmt,def);
    assert(r);
    assert(pc_raster_read_pixel(r,-500,400,out,2));
    assert(memcmp(out,def,2)==0);
    assert(pc_raster_write_pixel(r,-500,400,def,2));
    assert(pc_raster_tile_count(r)==1);
    pc_raster_prune(r);
    assert(pc_raster_tile_count(r)==0);
    assert(pc_raster_write_pixel(r,-500,400,change,2));
    pc_raster_prune(r);
    assert(pc_raster_tile_count(r)==1);
    pc_raster_destroy(r);
}
int main(void)
{
    const PcPixelFormat formats[]={
        {PC_COLOR_RGB,PC_SAMPLE_U8,1},
        {PC_COLOR_RGB,PC_SAMPLE_U16,1},
        {PC_COLOR_RGB,PC_SAMPLE_F32,1},
        {PC_COLOR_GRAY,PC_SAMPLE_U8,1},
        {PC_COLOR_CMYK,PC_SAMPLE_U8,1}
    };
    PcPixelFormat invalid={PC_COLOR_RGB,(PcSampleType)99,1};
    size_t i;
    for(i=0;i<sizeof(formats)/sizeof(formats[0]);++i)check_one(formats[i]);
    check_default();
    assert(pc_raster_new(invalid,NULL)==NULL);
    puts("PASS: PhotoCraft encoded U8/U16/F32 raster regions/COW");
    return 0;
}
