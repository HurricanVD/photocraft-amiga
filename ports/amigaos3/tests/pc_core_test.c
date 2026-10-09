#include "pc_core.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int eq_rect(PcRect a, PcRect b)
{
    return a.x0==b.x0 && a.y0==b.y0 && a.x1==b.x1 && a.y1==b.y1;
}
static void pixel_is(const PcSurface *s, int32_t x, int32_t y,
                     const uint8_t expected[4])
{
    uint8_t actual[4] = {255,255,255,255};
    assert(pc_surface_read_rgba8(s,x,y,actual));
    assert(memcmp(actual,expected,4)==0);
}
static void test_geom(void)
{
    const PcRect a=pc_rect_new(0,0,10,10);
    const PcRect b=pc_rect_new(5,5,15,15);
    PcTileCoord c;
    PcRect r;
    assert(pc_rect_width(pc_rect_from_xywh(10,20,30,40))==30);
    assert(pc_rect_height(pc_rect_from_xywh(10,20,30,40))==40);
    assert(pc_rect_contains(pc_rect_from_xywh(10,20,30,40),10,20));
    assert(!pc_rect_contains(pc_rect_from_xywh(10,20,30,40),40,20));
    assert(pc_rect_empty(pc_rect_new(5,5,5,10)));
    assert(eq_rect(pc_rect_intersect(a,b),pc_rect_new(5,5,10,10)));
    assert(eq_rect(pc_rect_union(a,b),pc_rect_new(0,0,15,15)));
    assert(eq_rect(pc_rect_intersect(a,pc_rect_new(20,20,30,30)),
                   pc_rect_new(0,0,0,0)));
    assert(eq_rect(pc_rect_union(pc_rect_new(0,0,0,0),a),a));
    assert(pc_rect_width(pc_rect_new(INT32_MIN,0,INT32_MAX,5))==UINT32_MAX);
    assert(eq_rect(pc_rect_translate(
                       pc_rect_new(INT32_MAX-10,INT32_MIN+10,INT32_MAX,INT32_MIN+20),
                       100,-100),
                   pc_rect_new(INT32_MAX,INT32_MIN,INT32_MAX,INT32_MIN)));
    c=pc_tile_containing(0,0); assert(c.tx==0&&c.ty==0);
    c=pc_tile_containing(255,255); assert(c.tx==0&&c.ty==0);
    c=pc_tile_containing(256,0); assert(c.tx==1&&c.ty==0);
    c=pc_tile_containing(-1,-1); assert(c.tx==-1&&c.ty==-1);
    c=pc_tile_containing(-256,-257); assert(c.tx==-1&&c.ty==-2);
    c=pc_tile_containing(INT32_MIN,INT32_MIN);
    assert(c.tx==-8388608&&c.ty==-8388608);
    assert(pc_tile_rect(pc_tile_containing(-1,-1),&r));
    assert(eq_rect(r,pc_rect_new(-256,-256,0,0)));
}
static void test_format(void)
{
    const PcPixelFormat rgba8={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    const PcPixelFormat rgba16={PC_COLOR_RGB,PC_SAMPLE_U16,1};
    const PcPixelFormat rgba32f={PC_COLOR_RGB,PC_SAMPLE_F32,1};
    const PcPixelFormat gray8={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    const PcPixelFormat cmyka8={PC_COLOR_CMYK,PC_SAMPLE_U8,1};
    assert(pc_format_bytes_per_pixel(rgba8)==4);
    assert(pc_format_bytes_per_pixel(rgba16)==8);
    assert(pc_format_bytes_per_pixel(rgba32f)==16);
    assert(pc_format_bytes_per_pixel(gray8)==1);
    assert(pc_format_channels(cmyka8)==5);
}
static void test_sparse_copy_on_write(void)
{
    const uint8_t transparent[4]={0,0,0,0};
    const uint8_t red[4]={255,0,0,255};
    const uint8_t green[4]={0,255,0,255};
    const uint8_t blue[4]={0,0,255,255};
    const uint8_t gray[4]={128,128,128,255};
    PcSurface *a=pc_surface_rgba8_new(NULL), *b, *m;
    assert(a);
    assert(pc_surface_tile_count(a)==0);
    pixel_is(a,10,-500,transparent);
    assert(pc_surface_write_rgba8(a,0,0,red));
    assert(pc_surface_write_rgba8(a,256,0,green));
    assert(pc_surface_write_rgba8(a,-1,-1,blue));
    assert(pc_surface_write_rgba8(a,-256,-257,red));
    assert(pc_surface_tile_count(a)==4);
    b=pc_surface_clone(a); assert(b);
    assert(pc_surface_tile_count(b)==4);
    assert(pc_surface_write_rgba8(b,0,0,green));
    pixel_is(a,0,0,red);
    pixel_is(b,0,0,green);
    pixel_is(a,256,0,green);
    pixel_is(b,256,0,green);
    assert(pc_surface_write_rgba8(b,-1,-1,transparent));
    pixel_is(a,-1,-1,blue);
    pixel_is(b,-1,-1,transparent);
    pc_surface_prune(b);
    assert(pc_surface_tile_count(b)==3);
    assert(pc_surface_tile_count(a)==4);
    pc_surface_destroy(a);
    pixel_is(b,256,0,green);
    pc_surface_destroy(b);
    m=pc_surface_rgba8_new(gray); assert(m);
    pixel_is(m,999,-999,gray);
    assert(pc_surface_write_rgba8(m,999,-999,gray));
    assert(pc_surface_tile_count(m)==1);
    pc_surface_prune(m);
    assert(pc_surface_tile_count(m)==0);
    pc_surface_destroy(m);
}
static void test_boundary_matrix(void)
{
    const int32_t coords[]={-513,-512,-257,-256,-255,-1,0,1,
                             255,256,257,511,512};
    const uint8_t p[4]={12,34,56,78};
    const uint8_t zero[4]={0,0,0,0};
    size_t i,j;
    PcSurface *s=pc_surface_rgba8_new(NULL);
    assert(s);
    for (i=0; i<sizeof(coords)/sizeof(coords[0]); ++i)
        for (j=0; j<sizeof(coords)/sizeof(coords[0]); ++j)
            assert(pc_surface_write_rgba8(s,coords[i],coords[j],p));
    for (i=0; i<sizeof(coords)/sizeof(coords[0]); ++i)
        for (j=0; j<sizeof(coords)/sizeof(coords[0]); ++j)
            pixel_is(s,coords[i],coords[j],p);
    pixel_is(s,999,999,zero);
    pc_surface_destroy(s);
}
int main(void)
{
    test_geom();
    test_format();
    test_sparse_copy_on_write();
    test_boundary_matrix();
    puts("PASS: PhotoCraft host core parity fixtures (geom/color/raster RGBA8)");
    return 0;
}
