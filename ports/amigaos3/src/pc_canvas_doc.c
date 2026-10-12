#include "pc_canvas_doc.h"
#include "pc_compose.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Keep the first Amiga GUI sample inside a single 256x256 sparse tile per
 * raster/mask. All edits are performed through PhotoCraft PcRaster/Document
 * APIs, never by reinterpreting Dunkelkammer's DkDocument tiles. */
#define PC_EXAMPLE_WIDTH 192U
#define PC_EXAMPLE_HEIGHT 144U
#define PC_VIEWPORT_MAX_WIDTH 1024UL
#define PC_VIEWPORT_MAX_HEIGHT 768UL

PcDocument *pc_canvas_example_document(void)
{
    PcPixelFormat rgba = { PC_COLOR_RGB, PC_SAMPLE_U8, 1 };
    PcDocument *doc = pc_document_new("PhotoCraft Amiga native layers",
                                      PC_EXAMPLE_WIDTH, PC_EXAMPLE_HEIGHT);
    PcRaster *base = NULL, *top = NULL, *mask = NULL;
    uint32_t x, y;
    if (!doc) return NULL;
    base = pc_raster_new(rgba, NULL);
    top = pc_raster_new(rgba, NULL);
    mask = pc_document_mask_reveal_all();
    if (!base || !top || !mask) goto failed;

    for (y = 0; y < PC_EXAMPLE_HEIGHT; ++y) {
        for (x = 0; x < PC_EXAMPLE_WIDTH; ++x) {
            const uint8_t pixel[4] = {
                (uint8_t)(28U + x / 5U),
                (uint8_t)(48U + y / 3U),
                (uint8_t)(90U + x / 7U),
                255U
            };
            if (!pc_raster_write_pixel(base, (int32_t)x, (int32_t)y,
                                       pixel, sizeof(pixel))) goto failed;
        }
    }
    for (y = 32U; y < 112U; ++y) {
        for (x = 32U; x < 160U; ++x) {
            const uint8_t accent[4] = { 244U, 115U, 30U, 192U };
            const uint8_t coverage = (x < 96U) ? 255U : 128U;
            if (!pc_raster_write_pixel(top, (int32_t)x, (int32_t)y,
                                       accent, sizeof(accent)) ||
                !pc_raster_write_pixel(mask, (int32_t)x, (int32_t)y,
                                       &coverage, 1U)) goto failed;
        }
    }
    if (!pc_document_append_raster(doc, 11U, "PhotoCraft background", base))
        goto failed;
    base = NULL;  /* document claimed ownership */
    if (!pc_document_append_raster(doc, 22U, "Masked orange layer", top))
        goto failed;
    top = NULL;
    if (!pc_document_attach_mask(doc, 22U, mask, 1, 1))
        goto failed;
    mask = NULL;
    if (!pc_document_set_layer_opacity(doc, 1U, 0.75f))
        goto failed;
    return doc;

failed:
    pc_raster_destroy(mask);
    pc_raster_destroy(top);
    pc_raster_destroy(base);
    pc_document_destroy(doc);
    return NULL;
}

static void pc_backdrop(uint8_t *p, unsigned long x, unsigned long y)
{
    const uint8_t c = (uint8_t)((((x / 16UL) + (y / 16UL)) & 1UL)
                                ? 53U : 43U);
    p[0] = c; p[1] = c; p[2] = (uint8_t)(c + 4U); p[3] = 255U;
}

int pc_canvas_document_band(const PcDocument *doc,
                            unsigned long viewport_w,
                            unsigned long viewport_h,
                            unsigned long y,
                            unsigned long rows,
                            uint8_t *out, size_t stride, size_t capacity)
{
    unsigned long doc_w, doc_h, left, top, seen_w, seen_h, y0, y1;
    unsigned long row, x, clip_w;
    size_t row_bytes, offset;
    if (!doc || !out || !viewport_w || !viewport_h || !rows ||
        viewport_w > PC_VIEWPORT_MAX_WIDTH ||
        viewport_h > PC_VIEWPORT_MAX_HEIGHT ||
        y >= viewport_h || rows > viewport_h - y) return 0;
    if (viewport_w > SIZE_MAX / 4U) return 0;
    row_bytes = (size_t)viewport_w * 4U;
    if (stride < row_bytes || capacity < row_bytes ||
        (size_t)(rows - 1U) > (capacity - row_bytes) / stride) return 0;

    doc_w = (unsigned long)pc_document_width(doc);
    doc_h = (unsigned long)pc_document_height(doc);
    if (!doc_w || !doc_h) return 0;
    left = (viewport_w > doc_w) ? (viewport_w - doc_w) / 2UL : 0UL;
    top = (viewport_h > doc_h) ? (viewport_h - doc_h) / 2UL : 0UL;
    seen_w = (doc_w < viewport_w) ? doc_w : viewport_w;
    seen_h = (doc_h < viewport_h) ? doc_h : viewport_h;

    for (row = 0; row < rows; ++row) {
        uint8_t *dest_row = out + (size_t)row * stride;
        for (x = 0; x < viewport_w; ++x)
            pc_backdrop(dest_row + (size_t)x * 4U, x, y + row);
    }

    y0 = y > top ? y : top;
    y1 = (y + rows < top + seen_h) ? y + rows : top + seen_h;
    if (y0 >= y1 || !seen_w) return 1;
    clip_w = seen_w;
    offset = (size_t)(y0 - y) * stride + (size_t)left * 4U;
    if (offset >= capacity) return 0;
    if (!pc_compose_rgba8_region(
            doc, pc_rect_new(0, (int32_t)(y0 - top),
                             (int32_t)clip_w, (int32_t)(y1 - top)),
            out + offset, stride, capacity - offset))
        return 0;

    /* The RTG canvas is opaque. Composite document straight-alpha over
     * the checkerboard in a deterministic byte path (PhotoCraft pixels
     * remain unchanged in the document). */
    for (row = y0; row < y1; ++row) {
        uint8_t *row_base = out + (size_t)(row - y) * stride;
        for (x = left; x < left + clip_w; ++x) {
            uint8_t *p = row_base + (size_t)x * 4U;
            uint8_t bg[4];
            unsigned a = (unsigned)p[3];
            unsigned channel;
            if (a == 255U) continue;
            pc_backdrop(bg, x, row);
            for (channel = 0; channel < 3U; ++channel) {
                p[channel] = (uint8_t)(((unsigned)p[channel] * a +
                    (unsigned)bg[channel] * (255U - a) + 127U) / 255U);
            }
            p[3] = 255U;
        }
    }
    return 1;
}
