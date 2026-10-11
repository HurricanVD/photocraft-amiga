#ifndef PHOTOCRAFT_AMIGA_DOCUMENT_H
#define PHOTOCRAFT_AMIGA_DOCUMENT_H
#include "pc_raster.h"
/*
 * EXPERIMENTAL flat raster-document skeleton, NOT full PhotoCraft Document.
 * Original crates/doc root ordering: index 0 is bottom, last index is top.
 * Document owns every appended PcRaster and duplicated layer names.
 * No group/masks/PSD persistence/compositor/history APIs here.
 * Opacity and fill opacity are metadata only; there is NO rendering yet.
 */
typedef struct PcDocument PcDocument;
PcDocument *pc_document_new(const char *name,uint32_t width,uint32_t height);
PcDocument *pc_document_clone(const PcDocument *document);
void pc_document_destroy(PcDocument *document);
size_t pc_document_layer_count(const PcDocument *document);
int pc_document_append_raster(PcDocument *document,uint64_t id,
                              const char *name,PcRaster *owned_surface);
uint64_t pc_document_layer_id(const PcDocument *document,size_t index);
const char *pc_document_layer_name(const PcDocument *document,size_t index);
PcRaster *pc_document_layer_raster(PcDocument *document,size_t index);
int pc_document_layer_visible(const PcDocument *document,size_t index);
int pc_document_set_layer_visible(PcDocument *document,size_t index,int visible);

/* Match Rust doc::Document::shift: +delta raises (toward top), negative
 * lowers; index 0 is bottom. Invalid id or out-of-range target returns 0
 * without mutating the document. A valid zero shift returns 1.
 * Moves ownership without allocating, cloning or changing layer IDs. */
int pc_document_shift_layer(PcDocument *document,uint64_t id,int delta);

/* Original Rust Layer::opacity and Layer::fill_opacity are independent
 * f32 values in [0,1]. They default to 1.0, reject invalid or non-finite
 * inputs without mutating state and survive document COW clones.
 * Getter returns -1.0f on invalid index; no compositing is implied. */
float pc_document_layer_opacity(const PcDocument *document,size_t index);
int pc_document_set_layer_opacity(PcDocument *document,size_t index,float opacity);
float pc_document_layer_fill_opacity(const PcDocument *document,size_t index);
int pc_document_set_layer_fill_opacity(PcDocument *document,size_t index,float opacity);
uint32_t pc_document_width(const PcDocument *document);
uint32_t pc_document_height(const PcDocument *document);
#endif
