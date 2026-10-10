#ifndef PHOTOCRAFT_AMIGA_DOCUMENT_H
#define PHOTOCRAFT_AMIGA_DOCUMENT_H
#include "pc_raster.h"
/*
 * EXPERIMENTAL raster/group document tree, NOT full PhotoCraft Document.
 * Original crates/doc root ordering: index 0 is bottom, last index is top.
 * Document owns every appended PcRaster and duplicated layer names.
 * Nested raster groups use bottom-first child order (maximum group depth
 * 100, matching Rust MAX_GROUP_DEPTH). Names, IDs and child arrays are
 * owned; raster bytes use COW snapshots. Group masks, artboards,
 * pass-through blending, PSD persistence and history are NOT supported.
 * Masks are initially a Gray8 ownership/sampling slice at fixed density=1,
 * feather=0, not a compositor or full LayerMask feature.
 * Opacity and fill opacity are metadata only; there is NO rendering yet.
 */
typedef struct PcDocument PcDocument;
#define PC_DOCUMENT_MAX_GROUP_DEPTH 100u
PcDocument *pc_document_new(const char *name,uint32_t width,uint32_t height);
PcDocument *pc_document_clone(const PcDocument *document);
void pc_document_destroy(PcDocument *document);
size_t pc_document_layer_count(const PcDocument *document);
int pc_document_append_raster(PcDocument *document,uint64_t id,
                              const char *name,PcRaster *owned_surface);
/* Append group at the document root; id must be unique across the tree. */
int pc_document_append_group(PcDocument *document,uint64_t id,
                             const char *name);
/* Parent is an existing group ID (never zero). Successful raster append
 * transfers ownership; on failure the caller still owns the surface.
 * Duplicate IDs, missing/non-group parents and excessive depth fail without
 * mutating the tree. */
int pc_document_group_append_group(PcDocument *document,uint64_t parent_id,
                                   uint64_t id,const char *name);
int pc_document_group_append_raster(PcDocument *document,uint64_t parent_id,
                                    uint64_t id,const char *name,
                                    PcRaster *owned_surface);
/* Read-only group traversal by parent ID. Missing/invalid parent returns
 * zero count, zero child ID, -1 type or NULL borrowed name/pixel surface. */
size_t pc_document_group_child_count(const PcDocument *document,uint64_t parent_id);
uint64_t pc_document_group_child_id(const PcDocument *document,uint64_t parent_id,
                                    size_t index);
int pc_document_group_child_is_group(const PcDocument *document,uint64_t parent_id,
                                     size_t index);
const char *pc_document_group_child_name(const PcDocument *document,
                                          uint64_t parent_id,size_t index);
PcRaster *pc_document_group_child_raster(PcDocument *document,uint64_t parent_id,
                                         size_t index);
/* Shift only siblings within one group, reusing Rust Document::shift's
 * signed direction. Does not reparent or allocate. */
int pc_document_group_shift_child(PcDocument *document,uint64_t parent_id,
                                  uint64_t child_id,int delta);
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
/* Original Rust LayerMask's initial Gray8 contract:
 * reveal-all default=255, hide-all default=0; enabled=false reveals all.
 * Fixed density=1, feather=0. No masks are rendered by this API.
 * Attach accepts only exactly Gray/U8/no alpha. Ownership transfers on
 * success, otherwise caller retains mask. The same surface pointer must
 * not be owned elsewhere in this document. An existing mask is rejected. */
PcRaster *pc_document_mask_reveal_all(void);
PcRaster *pc_document_mask_hide_all(void);
int pc_document_attach_mask(PcDocument *document,uint64_t layer_id,
                            PcRaster *owned_gray8,int enabled,int linked);
/* Borrowed reference, invalid after detach/destroy; mutating it triggers
 * PcRaster COW on cloned documents. */
PcRaster *pc_document_layer_mask(PcDocument *document,uint64_t layer_id);
/* Detach transfers ownership back to caller; NULL means absent/invalid. */
PcRaster *pc_document_detach_mask(PcDocument *document,uint64_t layer_id);
int pc_document_mask_enabled(const PcDocument *document,uint64_t layer_id);
int pc_document_set_mask_enabled(PcDocument *document,uint64_t layer_id,int enabled);
int pc_document_mask_linked(const PcDocument *document,uint64_t layer_id);
int pc_document_set_mask_linked(PcDocument *document,uint64_t layer_id,int linked);
/* Effective Gray8 coverage at fixed density=1. Disabled masks reveal 255.
 * Returns 0 for absent/invalid mask, otherwise writes one U8 byte. */
int pc_document_mask_value_u8(const PcDocument *document,uint64_t layer_id,
                              int32_t x,int32_t y,uint8_t *out);

uint32_t pc_document_width(const PcDocument *document);
uint32_t pc_document_height(const PcDocument *document);
#endif
