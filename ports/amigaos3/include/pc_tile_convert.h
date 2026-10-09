#ifndef PHOTOCRAFT_AMIGA_TILE_CONVERT_H
#define PHOTOCRAFT_AMIGA_TILE_CONVERT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A staging-only adapter for importing legacy Amiga ARGB32 buffers.
 *
 * src: uint32_t pixels with *numeric* value 0xAARRGGBB
 * dst: byte-interleaved R,G,B,A suitable for GL_RGBA + GL_UNSIGNED_BYTE
 * Strides are measured in src pixels and dst bytes, respectively.
 * No in-place conversion, and buffers must not overlap.
 * Returns 1 on success, 0 for invalid dimensions, strides, or pointers.
 *
 * PhotoCraft's own renderer should deliver RGBA8 directly. This function
 * does NOT redefine PhotoCraft's runtime pixel format or tile size.
 */
int pc_argb32_to_rgba8(const uint32_t *src, size_t src_stride_pixels,
                       uint8_t *dst, size_t dst_stride_bytes,
                       size_t width, size_t height);

/* Generate an opaque/semtransparent RGBA8 reference texture. */
int pc_fill_rgba8_test_tile(uint8_t *dst, size_t dst_stride_bytes,
                            size_t width, size_t height);

#ifdef __cplusplus
}
#endif
#endif
