/* Adapted from HurricanVD/dunkelkammer M2 offscreen presenter (MIT, 2026).
 * Source: 26295506ba363b1bd52bfca1852b0230ffb2bad9
 * MIT approval: 70897e91c94214421b0595b3e858446d6b3586ea
 * Copyright (c) 2026 Virtual Dimension.
 * Preserve the permission notice in ../NOTICE-DUNKELKAMMER-MIT.txt.
 *
 * This module never opens an Intuition Window or a Screen, and never locks
 * a visible RTG bitmap. The PhotoCraft C99 core owns its pixel data.
 */
#include "pc_present_m2.h"
#include "pc_present_convert.h"
#include <exec/types.h>
#include <exec/memory.h>
#include <graphics/gfx.h>
#include <graphics/rastport.h>
#include <cybergraphx/cybergraphics.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/cybergraphics.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <string.h>

struct Library *CyberGfxBase = 0;
static struct {
    struct BitMap *bitmap;
    struct RastPort offscreen;
    uint32_t *staging;
    unsigned long width, height;
} pc_m2;

static void pc_m2_release(void)
{
    if (pc_m2.bitmap) { FreeBitMap(pc_m2.bitmap); pc_m2.bitmap=0; }
    if (pc_m2.staging) { FreeVec(pc_m2.staging); pc_m2.staging=0; }
    pc_m2.width=0;
    pc_m2.height=0;
}
int pc_m2_open(void)
{
    if (!CyberGfxBase)
        CyberGfxBase=OpenLibrary((STRPTR)"cybergraphics.library",40);
    return CyberGfxBase!=0;
}
void pc_m2_close(void)
{
    pc_m2_release();
    if (CyberGfxBase) { CloseLibrary(CyberGfxBase); CyberGfxBase=0; }
}
int pc_m2_bind(struct RastPort *rp, unsigned long w, unsigned long h)
{
    ULONG depth;
    size_t bytes;
    if (!CyberGfxBase || !rp || !rp->BitMap || !w || !h ||
        w > (unsigned long)USHRT_MAX/4UL || h > (unsigned long)USHRT_MAX)
        return 0;
    if (!GetCyberMapAttr(rp->BitMap,CYBRMATTR_ISCYBERGFX)) return 0;
    if (pc_m2.bitmap && pc_m2.staging &&
        pc_m2.width==w && pc_m2.height==h) return 1;
    pc_m2_release();
    if ((size_t)w > SIZE_MAX / (size_t)h / sizeof(uint32_t)) return 0;
    bytes=(size_t)w*(size_t)h*sizeof(uint32_t);
    if (bytes > (size_t)ULONG_MAX) return 0;
    depth=(ULONG)GetBitMapAttr(rp->BitMap,BMA_DEPTH);
    if (!depth) return 0;
    pc_m2.bitmap=AllocBitMap((ULONG)w,(ULONG)h,depth,BMF_MINPLANES,rp->BitMap);
    if (!pc_m2.bitmap) return 0;
    pc_m2.staging=(uint32_t *)AllocVec((ULONG)bytes,MEMF_ANY);
    if (!pc_m2.staging) { pc_m2_release(); return 0; }
    memset(pc_m2.staging,0,bytes);
    InitRastPort(&pc_m2.offscreen);
    pc_m2.offscreen.BitMap=pc_m2.bitmap;
    pc_m2.width=w;pc_m2.height=h;
    return 1;
}
static int pc_m2_rect_valid(unsigned long x,unsigned long y,
                             unsigned long w,unsigned long h)
{
    if (!pc_m2.bitmap || !pc_m2.staging || !w || !h) return 0;
    return x<pc_m2.width && y<pc_m2.height &&
           w<=pc_m2.width-x && h<=pc_m2.height-y;
}
int pc_m2_upload_rgba8(const uint8_t *src, size_t stride,
                         unsigned long x,unsigned long y,
                         unsigned long w,unsigned long h)
{
    if (!src || !pc_m2_rect_valid(x,y,w,h)) return 0;
    if (!pc_rgba8_to_argb32(src,stride,
          pc_m2.staging+(size_t)y*pc_m2.width+x,
          (size_t)pc_m2.width,(size_t)w,(size_t)h)) return 0;
    /* WritePixelArray source is the full canvas-width staging cache. */
    WritePixelArray((APTR)pc_m2.staging,(UWORD)x,(UWORD)y,
       (UWORD)(pc_m2.width*4UL),&pc_m2.offscreen,
       (UWORD)x,(UWORD)y,(UWORD)w,(UWORD)h,RECTFMT_ARGB);
    return 1;
}
int pc_m2_blit(struct RastPort *rp,long dest_x,long dest_y,
               unsigned long x,unsigned long y,
               unsigned long w,unsigned long h)
{
    if (!rp || !pc_m2_rect_valid(x,y,w,h) ||
        dest_x < 0 || dest_y < 0 ||
        dest_x > LONG_MAX-(long)w || dest_y > LONG_MAX-(long)h)
        return 0;
    BltBitMapRastPort(pc_m2.bitmap,(WORD)x,(WORD)y,rp,
          (WORD)dest_x,(WORD)dest_y,(WORD)w,(WORD)h,0xC0);
    return 1;
}
