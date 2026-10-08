#include "pc_tile_convert.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int failures;

static void check(int condition, const char *message)
{
    if (!condition) {
        fprintf(stderr, "FAIL: %s\n", message);
        ++failures;
    }
}

int main(void)
{
    const uint32_t argb[6] = {
        0x00112233UL, 0x80405060UL, 0xAABBCCDDUL,
        0xFF123456UL, 0xFFABCDEFUL, 0x7F000000UL
    };
    uint8_t dst[32];
    uint8_t tile[2 * 16];
    uint8_t invalid[4];
    static uint8_t full_tile[256U * 256U * 4U];
    static const uint8_t expected_top[12] = {
        0x11, 0x22, 0x33, 0x00, 0x40, 0x50, 0x60, 0x80,
        0xCC, 0xDD, 0x00, 0x00
    };

    memset(dst, 0xEE, sizeof(dst));
    check(pc_argb32_to_rgba8(argb, 3, dst, 16, 2, 2), "2x2 copy");
    check(memcmp(dst, expected_top, 8) == 0, "ARGB channel order and alpha");
    check(dst[8] == 0xEE && dst[15] == 0xEE, "destination row padding untouched");
    check(dst[16] == 0x12 && dst[17] == 0x34 &&
          dst[18] == 0x56 && dst[19] == 0xFF,
          "second source row uses pixel stride");
    check(dst[20] == 0xAB && dst[21] == 0xCD &&
          dst[22] == 0xEF && dst[23] == 0xFF,
          "second pixel of padded row");
    check(dst[31] == 0xEE, "trailing canary untouched");

    check(!pc_argb32_to_rgba8(NULL, 1, dst, 4, 1, 1),
          "null source rejected");
    check(!pc_argb32_to_rgba8(argb, 1, NULL, 4, 1, 1),
          "null destination rejected");
    check(!pc_argb32_to_rgba8(argb, 1, dst, 4, 0, 1),
          "zero width rejected");
    check(!pc_argb32_to_rgba8(argb, 1, dst, 4, 1, 0),
          "zero height rejected");
    check(!pc_argb32_to_rgba8(argb, 1, dst, 8, 2, 1),
          "short source stride rejected");
    check(!pc_argb32_to_rgba8(argb, 2, dst, 7, 2, 1),
          "short destination stride rejected");
    check(!pc_argb32_to_rgba8(argb, SIZE_MAX, dst, 8, 2, 3),
          "source pointer arithmetic overflow rejected");
    check(!pc_argb32_to_rgba8(argb, SIZE_MAX / 2, dst, 8, 2, 2),
          "source byte offset overflow rejected");
    check(!pc_argb32_to_rgba8(argb, 2, dst, SIZE_MAX, 2, 3),
          "destination pointer arithmetic overflow rejected");
    check(!pc_argb32_to_rgba8(argb, 2, dst, 8, SIZE_MAX, 1),
          "pixel byte count overflow rejected");

    memset(tile, 0xEE, sizeof(tile));
    check(pc_fill_rgba8_test_tile(tile, 16, 2, 2), "reference tile");
    check(tile[0] == 24 && tile[1] == 180 && tile[2] == 220 &&
          tile[3] == 255, "test tile first pixel");
    check(tile[4 + 3] == 255 && tile[16 + 4 + 3] == 128,
          "test tile alpha quadrant");
    check(tile[8] == 0xEE && tile[15] == 0xEE,
          "test tile padding untouched");
    check(!pc_fill_rgba8_test_tile(invalid, 3, 1, 1),
          "test tile rejects short stride");
    check(!pc_fill_rgba8_test_tile(NULL, 4, 1, 1),
          "test tile rejects null");
    check(!pc_fill_rgba8_test_tile(invalid, 4, SIZE_MAX, 1),
          "test tile rejects oversized width");

    check(pc_fill_rgba8_test_tile(full_tile, 256U * 4U, 256U, 256U),
          "full 256x256 tile");
    check(full_tile[(8U * 256U + 8U) * 4U] == 24U &&
          full_tile[(8U * 256U + 8U) * 4U + 1U] == 180U &&
          full_tile[(8U * 256U + 8U) * 4U + 2U] == 220U,
          "tile dark pixel");
    check(full_tile[(8U * 256U + 24U) * 4U] == 220U &&
          full_tile[(8U * 256U + 24U) * 4U + 1U] == 70U,
          "tile bright pixel");
    check(full_tile[(129U * 256U + 129U) * 4U + 3U] == 128U &&
          full_tile[(8U * 256U + 8U) * 4U + 3U] == 255U,
          "tile alpha");
    if (failures) {
        fprintf(stderr, "FAIL: %d portable checks\n", failures);
        return 1;
    }
    puts("PASS: portable ARGB/RGBA staging checks");
    return 0;
}
