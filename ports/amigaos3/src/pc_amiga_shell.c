/* PhotoCraft AmigaOS 3.2 first native ReAction smoke shell.
 * UI lifetime/Hook/Workbench patterns adapted from HurricanVD/dunkelkammer
 * commit 26295506ba363b1bd52bfca1852b0230ffb2bad9 (MIT, 2026).
 * Copyright (c) 2026 Virtual Dimension.
 * See ../NOTICE-DUNKELKAMMER-MIT.txt for full permission text.
 *
 * Deliberately NOT a full editor: the visible canvas is synthetic.
 * No PhotoCraft Rust service, DkDocument, MiniGL or network dependency.
 */
#include "pc_present_m2.h"
#include "pc_tile_convert.h"
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/tasks.h>
#include <intuition/intuition.h>
#include <intuition/classusr.h>
#include <intuition/gadgetclass.h>
#include <graphics/rastport.h>
#include <graphics/gfx.h>
#include <classes/window.h>
#include <gadgets/layout.h>
#include <gadgets/space.h>
#include <reaction/reaction.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/space.h>
#include <clib/alib_protos.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

struct Library *WindowBase=0;
struct Library *LayoutBase=0;
struct Library *SpaceBase=0;
static Object *pc_winobj=0;
static Object *pc_root=0;
static struct Window *pc_window=0;
static struct Hook pc_render_hook;
static uint8_t *pc_frame=0;
static unsigned long pc_frame_w=0,pc_frame_h=0;
static int pc_root_owned=0;
static const char pc_version[]="\0$VER: PhotoCraft AmigaOS Shell 0.0.1 (12.10.2026)";
/* First-run drawing budget only; later the compositor streams dirty bands. */
#define PC_UI_MAX_WIDTH 1024UL
#define PC_UI_MAX_HEIGHT 768UL

static void pc_release_frame(void)
{
    if (pc_frame) FreeVec(pc_frame);
    pc_frame=0;pc_frame_w=0;pc_frame_h=0;
}
static int pc_make_frame(unsigned long w,unsigned long h)
{
    uint8_t *frame;
    size_t bytes;
    if (!w || !h || w>PC_UI_MAX_WIDTH || h>PC_UI_MAX_HEIGHT ||
        (size_t)w>SIZE_MAX/(size_t)h/4) return 0;
    if (pc_frame && w==pc_frame_w && h==pc_frame_h) return 1;
    bytes=(size_t)w*(size_t)h*4;
    frame=(uint8_t *)AllocVec((ULONG)bytes,MEMF_ANY);
    if (!frame) return 0;
    if (!pc_fill_rgba8_test_tile(frame,(size_t)w*4,(size_t)w,(size_t)h)) {
        FreeVec(frame);return 0;
    }
    pc_release_frame();
    pc_frame=frame;pc_frame_w=w;pc_frame_h=h;
    return 1;
}
static ULONG pc_canvas_render(struct Hook *hook,Object *object,
                              struct gpRender *message)
{
    struct IBox *rect=0;
    struct RastPort *rp;
    ULONG oldpen;
    unsigned long w,h;
    (void)hook;
    if (!object || !message || !message->gpr_RPort) return 0;
    if (!GetAttr(SPACE_AreaBox,object,(ULONG *)&rect) || !rect ||
        rect->Width<=0 || rect->Height<=0) return 0;
    rp=message->gpr_RPort;
    oldpen=rp->FgPen;
    w=(unsigned long)rect->Width; h=(unsigned long)rect->Height;
    if (pc_make_frame(w,h) && pc_m2_bind(rp,w,h) &&
        pc_m2_upload_rgba8(pc_frame,(size_t)w*4,0,0,w,h) &&
        pc_m2_blit(rp,(long)rect->Left,(long)rect->Top,0,0,w,h))
        return 0;
    /* RTG/low-memory fallback is explicit neutral fill, never damaged gadgets.
     * This is not PhotoCraft document rendering on non-RTG. */
    SetAPen(rp,0);
    RectFill(rp,(LONG)rect->Left,(LONG)rect->Top,
         (LONG)rect->Left+rect->Width-1,(LONG)rect->Top+rect->Height-1);
    SetAPen(rp,oldpen);
    return 0;
}
static void pc_shutdown(void)
{
    if (pc_window && pc_winobj) { RA_CloseWindow(pc_winobj);pc_window=0; }
    if (pc_winobj) { DisposeObject(pc_winobj);pc_winobj=0;pc_root=0; }
    if (pc_root && !pc_root_owned) { DisposeObject(pc_root);pc_root=0; }
    pc_release_frame();
    pc_m2_close();
    if (SpaceBase) { CloseLibrary(SpaceBase);SpaceBase=0; }
    if (LayoutBase) { CloseLibrary(LayoutBase);LayoutBase=0; }
    if (WindowBase) { CloseLibrary(WindowBase);WindowBase=0; }
}
int main(void)
{
    Object *area=0;
    struct TagItem lt[]={{LAYOUT_Orientation,LAYOUT_ORIENT_VERT},
                          {TAG_END,0}};
    struct TagItem st[]={{SPACE_RenderHook,0},
                          {GA_RelVerify,TRUE},
                          {TAG_END,0}};
    struct TagItem wt[]={{WA_Title,(ULONG)"PhotoCraft AmigaOS (M2 smoke)"},
                          {WA_Width,640},{WA_Height,480},
                          {WA_DragBar,TRUE},{WA_DepthGadget,TRUE},
                          {WA_CloseGadget,TRUE},{WA_SizeGadget,TRUE},
                          {WA_Activate,TRUE},{WA_AutoAdjust,TRUE},
                          {WA_IDCMP,IDCMP_CLOSEWINDOW|IDCMP_NEWSIZE|IDCMP_REFRESHWINDOW},
                          {WINDOW_ParentGroup,0},{TAG_END,0}};
    int status=20,done=0;
    ULONG sigmask=0,code=0,msg;
    (void)pc_version[1];
    WindowBase=OpenLibrary((STRPTR)"window.class",44);
    LayoutBase=OpenLibrary((STRPTR)"layout.gadget",44);
    SpaceBase=OpenLibrary((STRPTR)"space.gadget",44);
    if (!WindowBase || !LayoutBase || !SpaceBase) goto finish;
    if (!pc_m2_open()) goto finish;
    memset(&pc_render_hook,0,sizeof(pc_render_hook));
    pc_render_hook.h_Entry=(ULONG (*)())HookEntry;
    pc_render_hook.h_SubEntry=(ULONG (*)())pc_canvas_render;
    st[0].ti_Data=(ULONG)&pc_render_hook;
    pc_root=NewObjectA(LAYOUT_GetClass(),0,lt);
    if (!pc_root) goto finish;
    area=NewObjectA(SPACE_GetClass(),0,st);
    if (!area) goto finish;
    SetAttrs(pc_root,LAYOUT_AddChild,(ULONG)area,TAG_END);
    pc_root_owned=1; /* root is handed to window object on construction */
    wt[10].ti_Data=(ULONG)pc_root;
    pc_winobj=NewObjectA(WINDOW_GetClass(),0,wt);
    if (!pc_winobj) { pc_root_owned=0; goto finish; }
    pc_window=RA_OpenWindow(pc_winobj);
    if (!pc_window) goto finish;
    GetAttr(WINDOW_SigMask,pc_winobj,&sigmask);
    if (!sigmask) goto finish;
    status=0;
    while (!done) {
        ULONG signals=Wait(sigmask|SIGBREAKF_CTRL_C);
        if (signals&SIGBREAKF_CTRL_C) break;
        if (!(signals&sigmask)) continue;
        do {
            msg=RA_HandleInput(pc_winobj,&code);
            if (msg==WMHI_CLOSEWINDOW) done=1;
            else if (msg==WMHI_NEWSIZE) {
                /* Layout is reflowed by window.class. Trigger normal hooks. */
                RethinkLayout((struct Gadget *)pc_root,pc_window,0,TRUE);
            }
        } while (msg!=WMHI_LASTMSG);
    }
finish:
    pc_shutdown();
    return status;
}
