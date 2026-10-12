#ifndef PHOTOCRAFT_AMIGA_PRESENT_M2_H
#define PHOTOCRAFT_AMIGA_PRESENT_M2_H
#include <stddef.h>
#include <stdint.h>
struct RastPort;
/* Single ReAction-owned window/display. This module owns ONLY its offscreen
 * friend bitmap and ARGB staging memory; no screen or window is opened.
 * Adapted from Dunkelkammer's MIT M2 present model, see NOTICE-DUNKELKAMMER-MIT.txt.
 * All API arguments and dimensions are canvas-relative; the target RastPort
 * is owned by ReAction. No MiniGL, network or vxplatform display runtime.
 */
int pc_m2_open(void);
int pc_m2_bind(struct RastPort *window_rport, unsigned long width,
               unsigned long height);
int pc_m2_upload_rgba8(const uint8_t *pixels, size_t source_stride,
                       unsigned long x, unsigned long y,
                       unsigned long width, unsigned long height);
int pc_m2_blit(struct RastPort *window_rport,
               long dest_x, long dest_y,
               unsigned long src_x, unsigned long src_y,
               unsigned long width, unsigned long height);
void pc_m2_close(void);
#endif
