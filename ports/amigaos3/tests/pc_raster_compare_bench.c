/* PF-TD-001: exact same deterministic workload for historical linear and
 * current indexed PcRaster. Benchmarks are diagnostics, never CI speed gates. */
#include "pc_raster.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double seconds_since(clock_t start)
{
    return (double)(clock()-start)/(double)CLOCKS_PER_SEC;
}
static uint32_t mix(uint32_t x)
{
    x ^= x >> 16;
    x *= UINT32_C(0x7feb352d);
    x ^= x >> 15;
    x *= UINT32_C(0x846ca68b);
    return x ^ (x >> 16);
}
int main(int argc,char **argv)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    const unsigned reads=150000u,writes=30000u;
    unsigned count,i;
    uint32_t seed=UINT32_C(0x3d6f1459);
    volatile unsigned long checksum=0;
    clock_t start;
    double insert_sec,read_sec,write_sec;
    PcRaster *r;
    uint8_t pixel=7,got=0;
    char *end=0;
    unsigned long parsed;
    if(argc!=2)return 2;
    parsed=strtoul(argv[1],&end,10);
    if(end==argv[1] || !end || *end || parsed<1 || parsed>2048)return 2;
    count=(unsigned)parsed;
    r=pc_raster_new(fmt,NULL);
    if(!r)return 3;

    start=clock();
    /* Reverse-order insert stresses sorted-vector shifting while the old
     * linear implementation appends. This is deliberately reported. */
    for(i=count;i>0;--i){
        int32_t x=(int32_t)((i-1u)*PC_CORE_TILE_SIZE);
        if(!pc_raster_write_pixel(r,x,0,&pixel,1))return 4;
    }
    insert_sec=seconds_since(start);
    if(pc_raster_tile_count(r)!=count)return 5;

    start=clock();
    for(i=0;i<reads;++i){
        unsigned slot=mix(seed+i)%count;
        int32_t x=(int32_t)(slot*PC_CORE_TILE_SIZE);
        if(!pc_raster_read_pixel(r,x,0,&got,1) || got!=7)return 6;
        checksum+=got;
    }
    read_sec=seconds_since(start);

    start=clock();
    for(i=0;i<writes;++i){
        unsigned slot=mix(seed+(uint32_t)i+reads)%count;
        int32_t x=(int32_t)(slot*PC_CORE_TILE_SIZE);
        if(!pc_raster_write_pixel(r,x,0,&pixel,1))return 7;
    }
    write_sec=seconds_since(start);
    if(checksum!=(unsigned long)reads*7ul)return 8;
    printf("%u,%u,%u,%.9f,%.9f,%.9f,%lu\n",
           count,reads,writes,insert_sec,read_sec,write_sec,
           (unsigned long)checksum);
    pc_raster_destroy(r);
    return 0;
}
