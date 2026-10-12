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
int main(void)
{
    check_order_and_clone();
    check_fault_rollback();
    puts("PASS: PF-TD-001 sorted lookup and fault-atomic COW region");
    return 0;
}
