#include "pc_raster.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

/* Host-only, deterministic fixture; results are measured, not acceptance
 * thresholds. The tile index uses O(log N) search and O(N) insertion. */
int main(void)
{
    PcPixelFormat fmt={PC_COLOR_GRAY,PC_SAMPLE_U8,0};
    PcRaster *r=pc_raster_new(fmt,NULL);
    const unsigned side=64, count=side*side, reads=120000;
    unsigned i;
    uint8_t value=7, out=0;
    clock_t begin,end;
    double seconds;
    volatile unsigned checksum=0;
    assert(r);
    begin=clock();
    for(i=0;i<count;++i) {
        unsigned index=count-1-i;
        int32_t x=(int32_t)((index%side)*PC_CORE_TILE_SIZE);
        int32_t y=(int32_t)((index/side)*PC_CORE_TILE_SIZE);
        assert(pc_raster_write_pixel(r,x,y,&value,1));
    }
    end=clock();
    seconds=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("PF-TD-001 insert %u tiles CPU seconds %.6f\n",count,seconds);
    assert(pc_raster_tile_count(r)==count);
    begin=clock();
    for(i=0;i<reads;++i){
        unsigned index=(i*1031u)%count;
        int32_t x=(int32_t)((index%side)*PC_CORE_TILE_SIZE);
        int32_t y=(int32_t)((index/side)*PC_CORE_TILE_SIZE);
        assert(pc_raster_read_pixel(r,x,y,&out,1));
        checksum+=out;
    }
    end=clock();
    printf("PF-TD-001 lookup %u reads CPU seconds %.6f checksum %u\n",
           reads,(double)(end-begin)/CLOCKS_PER_SEC,checksum);
    assert(checksum==reads*value);
    pc_raster_destroy(r);
    return 0;
}
