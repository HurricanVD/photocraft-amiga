#ifndef PHOTOCRAFT_AMIGA_CANVAS_DOC_H
#define PHOTOCRAFT_AMIGA_CANVAS_DOC_H

#include "pc_document.h"
#include <stddef.h>
#include <stdint.h>

/* Small offline document fixture made from real PhotoCraft C99 layer/tile
 * types. No imported image, no DkDocument, and no host/remote service.
 * This is explicitly NOT a file loader or finished editor.
 */
PcDocument *pc_canvas_example_document(void);

/* Render a bounded vertical canvas band from a PhotoCraft Document.
 * The document is centered when smaller than the viewport. Outside-document
 * pixels are an opaque neutral backdrop. Visible document pixels are
 * composited through pc_compose_rgba8_region and displayed straight over
 * that backdrop, preserving opacity/mask behavior.
 * Destination RGBA8, caller owned, row stride in BYTES.
 * No allocations or full-frame buffers in this function.
 * Return zero on unsupported document/layout/input or failed compositing;
 * the caller MUST NOT upload the resulting band after a failure.
 */
int pc_canvas_document_band(const PcDocument *doc,
                            unsigned long viewport_w,
                            unsigned long viewport_h,
                            unsigned long y,
                            unsigned long rows,
                            uint8_t *out, size_t stride, size_t capacity);
#endif
