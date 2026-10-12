#include "pc_present_convert.h"
#include "pc_tile_convert.h"
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
    const uint8_t src[24] = {
       0x11,0x22,0x33,0x44,0xee,0xdd,0xcc,0xbb,0xfe,0xfe,0xfe,0xfe,
       0x99,0x88,0x77,0x66,0x00,0x00,0x00,0x00,0xee,0xee,0xee,0xee
    };
    uint32_t dst[8],before[8];
    uint8_t restored[24];
    memset(dst,0x5a,sizeof(dst));
    assert(pc_rgba8_to_argb32(src,12,dst,4,2,2));
    assert(dst[0]==UINT32_C(0x44112233));
    assert(dst[1]==UINT32_C(0xbbeeddcc));
    assert(dst[4]==UINT32_C(0x66998877));
    assert(dst[5]==0);
    assert(dst[2]==UINT32_C(0x5a5a5a5a) && dst[6]==UINT32_C(0x5a5a5a5a));
    memset(restored,0,sizeof(restored));
    assert(pc_argb32_to_rgba8(dst,4,restored,12,2,2));
    assert(memcmp(src,restored,8)==0);
    assert(memcmp(src+12,restored+12,8)==0);
    memcpy(before,dst,sizeof(dst));
    assert(!pc_rgba8_to_argb32(NULL,12,dst,4,2,2));
    assert(!pc_rgba8_to_argb32(src,7,dst,4,2,2));
    assert(!pc_rgba8_to_argb32(src,12,dst,1,2,2));
    assert(!pc_rgba8_to_argb32(src,12,dst,4,0,2));
    assert(!pc_rgba8_to_argb32(src,12,dst,4,SIZE_MAX,2));
    assert(!pc_rgba8_to_argb32(src,SIZE_MAX,dst,4,2,2));
    assert(!pc_rgba8_to_argb32(src,12,dst,SIZE_MAX,2,2));
    assert(memcmp(before,dst,sizeof(dst))==0);
    puts("PASS: PhotoCraft RGBA8 to numeric M2 ARGB32 stride/alpha bounds");
    return 0;
}
