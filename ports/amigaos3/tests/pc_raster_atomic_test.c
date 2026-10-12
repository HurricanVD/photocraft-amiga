#include "pc_raster.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* This test must be built with -DPC_RASTER_TEST_ALLOC. */
extern void pc_raster_test_fail_after(long successful_allocations);

static void check_order_and_clone(void)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    const int32_t xy[][2]={{1024,1024},{-1,0},{0,-257},{0,0},{-700,-700},
                            {256,256},{-257,1024},{1024,-257}};
    size_t i;
    uint8_t v,got;
    PcRaster *r=pc_raster_new(fmt,NULL),*copy;
    assert(r);
    for(i=0;i<sizeof(xy)/sizeof(xy[0]);++i){
        v=(uint8_t)(i+1);
        assert(pc_raster_write_pixel(r,xy[i][0],xy[i][1],&v,1));
    }
    assert(pc_raster_tile_count(r)==sizeof(xy)/sizeof(xy[0]));
    for(i=0;i<sizeof(xy)/sizeof(xy[0]);++i){
        assert(pc_raster_read_pixel(r,xy[i][0],xy[i][1],&got,1));
        assert(got==(uint8_t)(i+1));
    }
    copy=pc_raster_clone(r);assert(copy);
    v=77;
    assert(pc_raster_write_pixel(copy,0,0,&v,1));
    assert(pc_raster_read_pixel(r,0,0,&got,1)&&got==4);
    pc_raster_prune(copy);
    pc_raster_destroy(copy);
    pc_raster_destroy(r);
}
static void check_fault_rollback(void)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    PcRect area=pc_rect_new(-1,-1,1,1);
    uint8_t original=9, data[4]={31,32,33,34},out[4];
    long fail;
    size_t failed=0,passed=0;
    for(fail=0;fail<32;++fail){
        PcRaster *r=pc_raster_new(fmt,NULL),*snapshot;
        assert(r);
        assert(pc_raster_write_pixel(r,-1,-1,&original,1));
        snapshot=pc_raster_clone(r);assert(snapshot);
        pc_raster_test_fail_after(fail);
        if(pc_raster_write_region(r,area,data,sizeof(data))){
            ++passed;
            pc_raster_test_fail_after(-1);
            assert(pc_raster_read_region(r,area,out,4));
            assert(memcmp(data,out,4)==0);
        }else{
            ++failed;
            pc_raster_test_fail_after(-1);
            assert(pc_raster_tile_count(r)==1);
            assert(pc_raster_read_region(r,area,out,4));
            assert(out[0]==9&&out[1]==0&&out[2]==0&&out[3]==0);
        }
        assert(pc_raster_read_region(snapshot,area,out,4));
        assert(out[0]==9&&out[1]==0&&out[2]==0&&out[3]==0);
        pc_raster_destroy(snapshot);
        pc_raster_destroy(r);
    }
    assert(failed>=5&&passed>0);
}
static void check_ram_cycles(void)
{
    PcPixelFormat fmt={PC_COLOR_RGB,PC_SAMPLE_U8,1};
    PcRaster *r=pc_raster_new(fmt,NULL),*snapshot;
    PcMemoryStats before,after;
    uint8_t pixel[4]={10,20,30,255},replacement[4]={40,50,60,255},got[4];
    unsigned i,cycle;
    assert(r);
    /* RAM-02: 16 RGBA8 tiles, each 256 KiB, sparse baseline. */
    for(i=0;i<16;++i)
        assert(pc_raster_write_pixel(r,(int32_t)(i*256u),0,pixel,4));
    snapshot=pc_raster_clone(r);assert(snapshot);
    pc_raster_test_stats(&before);
    /* RAM-03/RAM-05: repeat successful transactional COW updates. */
    for(cycle=0;cycle<3;++cycle){
        for(i=0;i<16;++i){
            PcRect rect=pc_rect_new((int32_t)(i*256u),0,
                                    (int32_t)(i*256u+1u),1);
            assert(pc_raster_write_region(r,rect,replacement,4));
        }
        pc_raster_test_stats(&after);
        assert(after.current_bytes <= before.current_bytes+16u*262144u+65536u);
    }
    for(i=0;i<16;++i){
        assert(pc_raster_read_pixel(snapshot,(int32_t)(i*256u),0,got,4));
        assert(memcmp(got,pixel,4)==0);
        assert(pc_raster_read_pixel(r,(int32_t)(i*256u),0,got,4));
        assert(memcmp(got,replacement,4)==0);
    }
    pc_raster_destroy(snapshot);
    pc_raster_destroy(r);
    pc_raster_test_stats(&after);
    assert(after.current_bytes==0);
}
int main(void)
{
    check_order_and_clone();
    check_fault_rollback();
    check_ram_cycles();
    {
        PcMemoryStats stats;
        pc_raster_test_stats(&stats);
        assert(stats.current_bytes==0);
        assert(stats.peak_bytes>0);
        assert(stats.failed_allocations>0);
        printf("PF-TD-001 tracked peak %lu bytes, allocations %lu, failures %lu\\n",
               (unsigned long)stats.peak_bytes,
               (unsigned long)stats.allocation_count,
               (unsigned long)stats.failed_allocations);
    }
    puts("PASS: PF-TD-001 sorted lookup and fault-atomic COW region");
    return 0;
}
