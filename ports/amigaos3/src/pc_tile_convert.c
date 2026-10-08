#include "pc_tile_convert.h"

#include <stdint.h>

/* Bounds arithmetic is checked before forming row pointers. */
static int pc_checked_rows(size_t row_stride, size_t min_row,
                           size_t height, size_t elem_bytes)
{
    size_t limit;
    if (elem_bytes == 0) return 0;
    limit = SIZE_MAX / elem_bytes;
    if (height == 0 || min_row > limit || row_stride < min_row) return 0;
    if (height > 1 && row_stride > (limit - min_row) / (height - 1))
        return 0;
    return 1;
}

int pc_argb32_to_rgba8(const uint32_t *src, size_t src_stride_pixels,
                       uint8_t *dst, size_t dst_stride_bytes,
                       size_t width, size_t height)
{
    size_t x, y, row_bytes;
    if (!src || !dst || width == 0 || height == 0 || width > SIZE_MAX / 4)
        return 0;
    row_bytes = width * 4;
    if (!pc_checked_rows(src_stride_pixels, width, height, sizeof(*src)) ||
        !pc_checked_rows(dst_stride_bytes, row_bytes, height, 1))
        return 0;

    for (y = 0; y < height; ++y) {
        const uint32_t *in = src + y * src_stride_pixels;
        uint8_t *out = dst + y * dst_stride_bytes;
        for (x = 0; x < width; ++x) {
            uint32_t p = in[x];
            out[x * 4 + 0] = (uint8_t)(p >> 16);
            out[x * 4 + 1] = (uint8_t)(p >> 8);
            out[x * 4 + 2] = (uint8_t)p;
            out[x * 4 + 3] = (uint8_t)(p >> 24);
        }
    }
    return 1;
}

int pc_fill_rgba8_test_tile(uint8_t *dst, size_t dst_stride_bytes,
                            size_t width, size_t height)
{
    size_t x, y, row_bytes;
    if (!dst || width == 0 || height == 0 || width > SIZE_MAX / 4)
        return 0;
    row_bytes = width * 4;
    if (!pc_checked_rows(dst_stride_bytes, row_bytes, height, 1))
        return 0;

    for (y = 0; y < height; ++y) {
        uint8_t *row = dst + y * dst_stride_bytes;
        for (x = 0; x < width; ++x) {
            int bright = (((x / 16) + (y / 16)) & 1) != 0;
            row[x * 4 + 0] = (uint8_t)(bright ? 220 : 24);
            row[x * 4 + 1] = (uint8_t)(bright ? 70 : 180);
            row[x * 4 + 2] = (uint8_t)(bright ? 30 : 220);
            row[x * 4 + 3] = (uint8_t)((x >= width / 2 && y >= height / 2) ? 128 : 255);
        }
    }
    return 1;
}
