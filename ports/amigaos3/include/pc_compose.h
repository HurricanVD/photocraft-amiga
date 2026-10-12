#ifndef PHOTOCRAFT_AMIGA_COMPOSE_H
#define PHOTOCRAFT_AMIGA_COMPOSE_H
#include "pc_document.h"
/* PF-SP-007: first normal-blend PhotoCraft CPU reference subset.
 * Writes a rectangle of straight-alpha RGBA8 into caller-owned rows.
 * Only flat RGBA8 raster layers are currently supported. A group or
 * unsupported format is rejected BEFORE any destination change.
 * This is NOT full Photoshop composite, ICC, CMYK or isolated groups.
 * Coordinates are document-local; stride and capacity are in bytes.
 * No full-document scratch allocation, no AmigaOS library dependency.
 */
int pc_compose_rgba8_region(const PcDocument *document, PcRect rect,
                           uint8_t *dst, size_t stride, size_t len);
#endif
