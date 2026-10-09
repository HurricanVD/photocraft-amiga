#ifndef PHOTOCRAFT_AMIGA_CORE_H
#define PHOTOCRAFT_AMIGA_CORE_H

#include <stddef.h>
#include <stdint.h>

/* PhotoCraft reference: crates/geom, crates/color, crates/raster.
 * Independent scoped C99 port, no OS dependencies. This initial surface
 * slice supports only RGBA8 (not all native PhotoCraft pixel formats). */
#define PC_CORE_TILE_SIZE 256

typedef struct PcRect { int32_t x0, y0, x1, y1; } PcRect;
typedef struct PcTileCoord { int32_t tx, ty; } PcTileCoord;

typedef enum PcSampleType { PC_SAMPLE_U8, PC_SAMPLE_U16, PC_SAMPLE_F32 } PcSampleType;
typedef enum PcColorMode {
    PC_COLOR_BITMAP, PC_COLOR_GRAY, PC_COLOR_INDEXED, PC_COLOR_RGB,
    PC_COLOR_CMYK, PC_COLOR_LAB, PC_COLOR_MULTICHANNEL, PC_COLOR_DUOTONE
} PcColorMode;
typedef struct PcPixelFormat {
    PcColorMode mode;
    PcSampleType sample;
    int alpha;
} PcPixelFormat;

/* Half-open rectangles and floor-divided negative tile coordinates. */
PcRect pc_rect_new(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
PcRect pc_rect_from_xywh(int32_t x, int32_t y, uint32_t w, uint32_t h);
uint32_t pc_rect_width(PcRect r);
uint32_t pc_rect_height(PcRect r);
int pc_rect_empty(PcRect r);
int pc_rect_contains(PcRect r, int32_t x, int32_t y);
PcRect pc_rect_intersect(PcRect a, PcRect b);
PcRect pc_rect_union(PcRect a, PcRect b);
PcRect pc_rect_translate(PcRect r, int32_t dx, int32_t dy);
PcTileCoord pc_tile_containing(int32_t x, int32_t y);
int pc_tile_equal(PcTileCoord a, PcTileCoord b);
/* Returns 0 if the tile boundary does not fit signed 32-bit space. */
int pc_tile_rect(PcTileCoord c, PcRect *out);

/* Metadata matches the Rust PixelFormat model. */
size_t pc_format_channels(PcPixelFormat f);
size_t pc_format_bytes_per_pixel(PcPixelFormat f);

/* First raster slice: infinite sparse plane of interleaved RGBA8 pixels.
 * Missing tiles read as the default. Snapshots share tiles until modified.
 * Caller owns each returned surface; no multi-thread synchronization yet. */
typedef struct PcSurface PcSurface;
PcSurface *pc_surface_rgba8_new(const uint8_t default_pixel[4]);
PcSurface *pc_surface_clone(const PcSurface *original);
void pc_surface_destroy(PcSurface *surface);
size_t pc_surface_tile_count(const PcSurface *surface);
int pc_surface_read_rgba8(const PcSurface *surface, int32_t x, int32_t y,
                          uint8_t rgba[4]);
/* Returns 0 on invalid input or allocation failure; mutation is atomic. */
int pc_surface_write_rgba8(PcSurface *surface, int32_t x, int32_t y,
                           const uint8_t rgba[4]);
void pc_surface_prune(PcSurface *surface);

#endif
