#ifndef PHOTOCRAFT_AMIGA_DOCUMENT_H
#define PHOTOCRAFT_AMIGA_DOCUMENT_H
#include "pc_raster.h"
/*
 * EXPERIMENTAL flat raster-document skeleton, NOT full PhotoCraft Document.
 * Original crates/doc root ordering: index 0 is bottom, last index is top.
 * Document owns every appended PcRaster and duplicated layer names.
 * No group/masks/PSD persistence/compositor/history APIs here.
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
uint32_t pc_document_width(const PcDocument *document);
uint32_t pc_document_height(const PcDocument *document);
#endif
