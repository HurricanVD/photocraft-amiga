#ifndef PHOTOCRAFT_AMIGA_RASTER_H
#define PHOTOCRAFT_AMIGA_RASTER_H

#include "pc_core.h"

/*
 * Experimental PhotoCraft encoded-format raster spike (PF-SP-002).
 * Source contract: crates/raster::Surface::write_interleaved/to_interleaved.
 * All coordinates signed; 256x256 tiles; missing pixels read default bytes.
 * U16 and F32 encoded samples are native-endian like Rust's to_ne_bytes.
 * Pixel conversion, CMS, blending, and layer semantics are NOT done here.
 *
 * Bulk writes are not transactional on OOM: already written earlier pixels
 * remain applied. Single-pixel write is allocation-safe on error.
 * No function returns an internal tile pointer; caller owns every buffer.
 */
typedef struct PcRaster PcRaster;
PcRaster *pc_raster_new(PcPixelFormat format, const uint8_t *default_bytes);
PcRaster *pc_raster_clone(const PcRaster *raster);
void pc_raster_destroy(PcRaster *raster);
size_t pc_raster_tile_count(const PcRaster *raster);
size_t pc_raster_bytes_per_pixel(const PcRaster *raster);
/* Read the exact encoded format; never infer Gray8 from stride alone. */
int pc_raster_get_format(const PcRaster *raster,PcPixelFormat *out);
int pc_raster_read_pixel(const PcRaster *raster, int32_t x, int32_t y,
                         uint8_t *out, size_t out_len);
int pc_raster_write_pixel(PcRaster *raster, int32_t x, int32_t y,
                          const uint8_t *bytes, size_t len);

/* Packed, tightly row-major encoded pixels. Fails on invalid/overflow
 * rectangle, inadequate caller buffer, or memory allocation failure. */
size_t pc_raster_region_bytes(const PcRaster *raster, PcRect rect);
int pc_raster_read_region(const PcRaster *raster, PcRect rect,
                          uint8_t *out, size_t out_len);
int pc_raster_write_region(PcRaster *raster, PcRect rect,
                           const uint8_t *bytes, size_t len);
void pc_raster_prune(PcRaster *raster);

#endif
