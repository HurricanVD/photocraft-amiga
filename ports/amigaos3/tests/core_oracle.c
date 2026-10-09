/*
 * PF-SP-002: machine-readable fixtures from the C99 PhotoCraft core.
 * Compare byte-for-byte against the Rust original oracle in CI.
 */
#include "pc_core.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void rect_line(const char *label, PcRect r)
{
    printf("%s %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 "\n",
           label, r.x0, r.y0, r.x1, r.y1);
}

static void coord_line(int32_t x, int32_t y)
{
    PcTileCoord c = pc_tile_containing(x, y);
    printf("tile %" PRId32 " %" PRId32 " %" PRId32 " %" PRId32 "\n",
           x, y, c.tx, c.ty);
}

static int pixel_line(const char *label, const PcSurface *surface,
                      int32_t x, int32_t y)
{
    uint8_t p[4];
    if (!pc_surface_read_rgba8(surface, x, y, p)) return 0;
    printf("%s %u %u %u %u\n", label,
           (unsigned)p[0], (unsigned)p[1],
           (unsigned)p[2], (unsigned)p[3]);
    return 1;
}

int main(void)
{
    static const uint8_t red[4] = {255, 0, 0, 255};
    static const uint8_t green[4] = {0, 255, 0, 255};
    static const uint8_t blue[4] = {0, 0, 255, 255};
    static const uint8_t zero[4] = {0, 0, 0, 0};
    static const uint8_t gray[4] = {128, 128, 128, 255};
    PcRect a = pc_rect_new(0, 0, 10, 10);
    PcRect b = pc_rect_new(5, 5, 15, 15);
    PcPixelFormat fmt = {PC_COLOR_CMYK, PC_SAMPLE_U8, 1};
    PcSurface *s, *snapshot, *mask;
    PcTileCoord c = {1, -2};
    PcRect tile_rectangle;
    int result = 1;

    rect_line("rect-xywh", pc_rect_from_xywh(10, 20, 30, 40));
    rect_line("rect-intersect", pc_rect_intersect(a, b));
    rect_line("rect-union", pc_rect_union(a, b));
    rect_line("rect-disjoint", pc_rect_intersect(a,
                                 pc_rect_new(20, 20, 30, 30)));
    rect_line("rect-translate", pc_rect_translate(
        pc_rect_new(INT32_MAX - 10, INT32_MIN + 10,
                    INT32_MAX, INT32_MIN + 20), 100, -100));
    printf("rect-extreme-width %" PRIu32 "\n",
           pc_rect_width(pc_rect_new(INT32_MIN, 0, INT32_MAX, 1)));
    coord_line(-257, -256);
    coord_line(-256, -257);
    coord_line(-1, -1);
    coord_line(0, 0);
    coord_line(255, 255);
    coord_line(256, 256);
    if (!pc_tile_rect(c, &tile_rectangle)) return 2;
    rect_line("tile-rect", tile_rectangle);
    printf("format-rgba8 %lu\n", (unsigned long)pc_format_bytes_per_pixel(
        (PcPixelFormat){PC_COLOR_RGB, PC_SAMPLE_U8, 1}));
    printf("format-rgba16 %lu\n", (unsigned long)pc_format_bytes_per_pixel(
        (PcPixelFormat){PC_COLOR_RGB, PC_SAMPLE_U16, 1}));
    printf("format-rgba32f %lu\n", (unsigned long)pc_format_bytes_per_pixel(
        (PcPixelFormat){PC_COLOR_RGB, PC_SAMPLE_F32, 1}));
    printf("format-cmyka-channels %lu\n",
           (unsigned long)pc_format_channels(fmt));

    s = pc_surface_rgba8_new(NULL);
    if (!s) return 2;
    printf("surface-empty %lu\n", (unsigned long)pc_surface_tile_count(s));
    result &= pixel_line("surface-default", s, 300, -200);
    result &= pc_surface_write_rgba8(s, 0, 0, red);
    result &= pc_surface_write_rgba8(s, 256, 0, green);
    result &= pc_surface_write_rgba8(s, -1, -1, blue);
    result &= pc_surface_write_rgba8(s, -256, -257, red);
    if (!result) { pc_surface_destroy(s); return 2; }
    printf("surface-tiles %lu\n", (unsigned long)pc_surface_tile_count(s));
    snapshot = pc_surface_clone(s);
    if (!snapshot) { pc_surface_destroy(s); return 2; }
    result &= pc_surface_write_rgba8(snapshot, 0, 0, green);
    result &= pc_surface_write_rgba8(snapshot, -1, -1, zero);
    if (!result) { pc_surface_destroy(snapshot); pc_surface_destroy(s); return 2; }
    result &= pixel_line("original-red", s, 0, 0);
    result &= pixel_line("snapshot-green", snapshot, 0, 0);
    result &= pixel_line("original-blue", s, -1, -1);
    result &= pixel_line("snapshot-cleared", snapshot, -1, -1);
    result &= pixel_line("shared-neighbor", snapshot, 256, 0);
    pc_surface_prune(snapshot);
    printf("snapshot-pruned-tiles %lu\n",
           (unsigned long)pc_surface_tile_count(snapshot));
    printf("original-still-tiles %lu\n",
           (unsigned long)pc_surface_tile_count(s));
    pc_surface_destroy(s);
    result &= pixel_line("snapshot-after-drop", snapshot, 256, 0);
    pc_surface_destroy(snapshot);

    mask = pc_surface_rgba8_new(gray);
    if (!mask) return 2;
    result &= pixel_line("nonzero-default", mask, 999, -999);
    result &= pc_surface_write_rgba8(mask, 999, -999, gray);
    pc_surface_prune(mask);
    printf("default-tile-pruned %lu\n",
           (unsigned long)pc_surface_tile_count(mask));
    pc_surface_destroy(mask);
    return result ? 0 : 2;
}
