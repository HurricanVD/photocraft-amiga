#include "pc_canvas_doc.h"
#include "pc_compose.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define VW 320UL
#define VH 256UL
#define ROWS 8UL

static const uint8_t *pixel(const uint8_t *b, unsigned long x,
                             unsigned long row) {
    return b + ((size_t)row * VW + x) * 4U;
}
int main(void)
{
    PcDocument *doc = pc_canvas_example_document();
    uint8_t band[VW * ROWS * 4U];
    uint8_t before[4], after[4], direct[4];
    uint8_t guard[VW * ROWS * 4U];
    const unsigned long center_y = (VH - 144UL) / 2UL;
    const unsigned long center_x = (VW - 192UL) / 2UL;
    assert(doc && pc_document_width(doc) == 192U);
    assert(pc_document_height(doc) == 144U);
    assert(pc_document_layer_count(doc) == 2U);
    assert(pc_document_layer_id(doc, 0) == 11U);
    assert(pc_document_layer_id(doc, 1) == 22U);
    assert(pc_document_layer_mask(doc,22U) != NULL);
    assert(pc_canvas_document_band(doc, VW, VH, 0UL, ROWS,
                                   band, VW*4UL, sizeof(band)));
    assert(pixel(band, 0, 0)[3] == 255U);
    assert(pixel(band, center_x + 96UL, 0)[0] != 244U);

    /* At viewport y=96 we are in the masked upper layer (doc y=40);
     * the compositor must match the document's actual flattened pixel,
     * not a synthetic test texture. */
    assert(pc_canvas_document_band(doc, VW, VH, center_y + 40UL,
                                   ROWS, band, VW*4UL, sizeof(band)));
    memcpy(before, pixel(band, center_x + 96UL, 0UL), 4);
    assert(pc_compose_rgba8_region(doc,pc_rect_new(96,40,97,41),
                                   direct,4,sizeof(direct)));
    assert(memcmp(before,direct,4) == 0);
    assert(before[3] == 255U);

    assert(pc_document_set_layer_visible(doc,1U,0));
    assert(pc_canvas_document_band(doc,VW,VH,center_y+40UL,ROWS,
                                   band,VW*4UL,sizeof(band)));
    memcpy(after, pixel(band,center_x+96UL,0),4);
    assert(memcmp(before,after,4)!=0);
    assert(pc_compose_rgba8_region(doc,pc_rect_new(96,40,97,41),
                                   direct,4,sizeof(direct)));
    assert(memcmp(after,direct,4)==0);

    assert(pc_canvas_document_band(doc, VW, VH, center_y - 3UL,
                                   ROWS, band, VW*4UL,sizeof(band)));
    assert(pixel(band, center_x + 5UL, 0)[3]==255);
    assert(pixel(band, center_x + 5UL, 3)[3]==255);

    memset(band, 0x69, sizeof(band));
    memcpy(guard, band, sizeof(band));
    assert(!pc_canvas_document_band(doc,VW,VH,0,ROWS,
                                    band,VW*4UL-1U,sizeof(band)));
    assert(!pc_canvas_document_band(doc,VW,VH,0,ROWS,
                                    band,VW*4UL,sizeof(band)-1U));
    assert(!pc_canvas_document_band(doc,VW,VH,VH-1UL,ROWS,
                                    band,VW*4UL,sizeof(band)));
    /* Only dimensions beyond the *new shared* upper bound are rejected. */
    assert(!pc_canvas_document_band(doc,PC_CANVAS_VIEWPORT_MAX_WIDTH+1UL,VH,
                                    0,ROWS,band,VW*4UL,sizeof(band)));
    assert(!pc_canvas_document_band(doc,VW,PC_CANVAS_VIEWPORT_MAX_HEIGHT+1UL,
                                    0,ROWS,band,VW*4UL,sizeof(band)));
    assert(!pc_canvas_document_band(NULL,VW,VH,0,ROWS,
                                    band,VW*4UL,sizeof(band)));
    assert(memcmp(band,guard,sizeof(band))==0);
    /* User WinUAE regression: at ~1450 px the former hard-coded
     * 1024x768 guard discarded the image and showed a grey canvas.
     * Large->small->large transitions must retain the document pixels.
     * A single 8-row buffer keeps allocations bounded even at 1920px. */
    {
        const unsigned long widths[] = {1600UL, 320UL, 1920UL, 1450UL};
        const unsigned long heights[] = {900UL, 256UL, 1200UL, 700UL};
        const size_t cap=(size_t)PC_CANVAS_VIEWPORT_MAX_WIDTH * ROWS * 4U;
        uint8_t *large=(uint8_t *)malloc(cap);
        size_t i;
        assert(large);
        for (i=0;i<sizeof(widths)/sizeof(widths[0]);++i) {
            unsigned long w=widths[i],h=heights[i];
            unsigned long offx=(w-192UL)/2UL,offy=(h-144UL)/2UL;
            unsigned long sample_y=offy+40UL;
            const uint8_t *p;
            size_t stride=(size_t)w*4U;
            memset(large,0x69,cap);
            assert(pc_canvas_document_band(doc,w,h,sample_y,ROWS,
                                           large,stride,cap));
            p=large+(size_t)(offx+96UL)*4U;
            assert(memcmp(p,direct,4)==0);
            assert(p[3]==255U);
            /* No writes outside the used row range. */
            if(stride*ROWS<cap)
                assert(large[stride*ROWS]==0x69);
        }
        free(large);
    }
    pc_document_destroy(doc);
    puts("PASS: PhotoCraft PcDocument native canvas bands, mask, visibility and bounds");
    return 0;
}
