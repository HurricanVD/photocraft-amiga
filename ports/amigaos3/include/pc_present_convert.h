#ifndef PHOTOCRAFT_AMIGA_PRESENT_CONVERT_H
#define PHOTOCRAFT_AMIGA_PRESENT_CONVERT_H
#include <stddef.h>
#include <stdint.h>
/* RGBA8 byte-interleaved -> numeric 0xAARRGGBB. Strides in source bytes,
 * destination uint32_t pixels. No overlap. Caller owns both buffers.
 * Rejects invalid dimensions/overflow without touching output. */
int pc_rgba8_to_argb32(const uint8_t *src, size_t src_stride,
                       uint32_t *dst, size_t dst_stride,
                       size_t width, size_t height);
#endif
