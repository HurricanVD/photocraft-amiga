#include "pc_raster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void print_hex(const char *label,const uint8_t *bytes,size_t n)
{
    size_t i;
    printf("%s ",label);
    for(i=0;i<n;++i)printf("%02x",(unsigned)bytes[i]);
    putchar('\n');
}
static int one(const char *name,PcPixelFormat fmt)
{
    PcRect rect=pc_rect_new(-1,255,1,257);
    PcRaster *r=pc_raster_new(fmt,NULL),*clone;
    uint8_t input[80],original[80],changed[80],replacement[20];
    size_t n,bpp,i;
    char title[70];
    if(!r)return 0;
    n=pc_raster_region_bytes(r,rect);
    bpp=pc_raster_bytes_per_pixel(r);
    if(n>sizeof(input)){pc_raster_destroy(r);return 0;}
    for(i=0;i<n;++i)input[i]=(uint8_t)((i*31+7)%251);
    if(!pc_raster_write_region(r,rect,input,n))return 0;
    if(!pc_raster_read_region(r,rect,original,n))return 0;
    snprintf(title,sizeof(title),"typed-%s-initial",name);
    print_hex(title,original,n);
    printf("typed-%s-tiles %lu\n",name,(unsigned long)pc_raster_tile_count(r));
    clone=pc_raster_clone(r);
    if(!clone)return 0;
    memset(replacement,0xa5,bpp);
    if(!pc_raster_write_pixel(clone,0,256,replacement,bpp))return 0;
    if(!pc_raster_read_region(clone,rect,changed,n))return 0;
    snprintf(title,sizeof(title),"typed-%s-snapshot",name);
    print_hex(title,changed,n);
    if(!pc_raster_read_region(r,rect,original,n))return 0;
    snprintf(title,sizeof(title),"typed-%s-original",name);
    print_hex(title,original,n);
    pc_raster_destroy(r);
    pc_raster_destroy(clone);
    return 1;
}
int main(void)
{
    PcPixelFormat rgba8={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcPixelFormat rgba16={PC_COLOR_RGB,PC_SAMPLE_U16,1};
    PcPixelFormat rgba32f={PC_COLOR_RGB,PC_SAMPLE_F32,1};
    PcPixelFormat graya8={PC_COLOR_GRAY,PC_SAMPLE_U8,1};
    PcPixelFormat cmyka8={PC_COLOR_CMYK,PC_SAMPLE_U8,1};
    PcRaster *mask;
    uint8_t def[2]={128,255},out[2];
    if(!one("rgba8",rgba8)||!one("rgba16",rgba16)||
       !one("rgba32f",rgba32f)||!one("graya8",graya8)||
       !one("cmyka8",cmyka8))return 2;
    mask=pc_raster_new(graya8,def);
    if(!mask)return 2;
    if(!pc_raster_read_pixel(mask,-500,400,out,2))return 2;
    print_hex("typed-graya8-default",out,2);
    pc_raster_destroy(mask);
    return 0;
}
