#ifndef PHOTOCRAFT_AMIGA_RASTER_OWNER_INTERNAL_H
#define PHOTOCRAFT_AMIGA_RASTER_OWNER_INTERNAL_H

#include "pc_raster.h"

/* Private to the C99 port's document/raster implementation.
 * Single-threaded document mutation only; not a public or stable ABI.
 *
 * Successful claim transfers the raster object's sole owning lifetime
 * to exactly one document. Clone creates a fresh, unclaimed raster owner
 * (only tile payloads are copy-on-write shared).
 *
 * An unsuccessful claim never consumes or frees the pointer. If it was
 * already claimed, the original owning document still owns it.
 */
int pc_raster_claim_document_ownership(PcRaster *raster);
/* Only a successful owning detach may release a claimed raster. */
int pc_raster_release_document_ownership(PcRaster *raster);

#endif
