#include "pc_compose.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
/* PhotoCraft Rust compositor: straight-alpha source over and opacity*fill
 * metadata; equivalent normal blend to within <=1 U8 roundoff for the
 * tested subset. We use bounded integer arithmetic to avoid GCC13's
 * missing softfloat comparison helpers in the existing target lane.
 */
static unsigned pc_fraction_255(float value)
{
    uint32_t bits,exp,significand,shift;
    uint64_t scaled;
    if (sizeof(value)!=sizeof(bits)) return 0;
    memcpy(&bits,&value,sizeof(bits));
    exp=(bits>>23)&255U;
    if ((bits&UINT32_C(0x80000000)) || exp<118U) return 0;
    if (exp>=127U) return 255U;
    significand=(bits&UINT32_C(0x7fffff))|UINT32_C(0x800000);
    shift=150U-exp; /* 24..32 for a nontrivial fraction */
    scaled=((uint64_t)significand*255U+((uint64_t)1<<(shift-1U)))>>shift;
    return scaled>255U?255U:(unsigned)scaled;
}
static unsigned pc_scale(unsigned a,unsigned b)
{
    return (a*b+127U)/255U;
}
static void pc_over(uint8_t *backdrop,const uint8_t *source,unsigned sa)
{
    unsigned da,ao,den,i;
    if (!sa) return;
    da=backdrop[3];
    ao=sa+pc_scale(da,255U-sa);
    if (!ao) { memset(backdrop,0,4);return; }
    den=ao*255U;
    for (i=0;i<3;++i) {
        unsigned num=(unsigned)source[i]*sa*255U+
                     (unsigned)backdrop[i]*da*(255U-sa);
        unsigned result=(num+den/2U)/den;
        backdrop[i]=(uint8_t)(result>255U?255U:result);
    }
    backdrop[3]=(uint8_t)ao;
}
int pc_compose_rgba8_region(const PcDocument *doc,PcRect r,
                           uint8_t *dst,size_t stride,size_t cap)
{
    size_t count,li,rowbytes,y,x;
    if (!doc || !dst || pc_rect_empty(r) || r.x0<0 || r.y0<0 ||
        r.x1<0 || r.y1<0 ||
        (uint32_t)r.x1>pc_document_width(doc) ||
        (uint32_t)r.y1>pc_document_height(doc)) return 0;
    if ((size_t)pc_rect_width(r)>SIZE_MAX/4U) return 0;
    rowbytes=(size_t)pc_rect_width(r)*4U;
    if (stride<rowbytes || cap<rowbytes ||
        ((size_t)pc_rect_height(r)-1U)>(cap-rowbytes)/stride) return 0;
    count=pc_document_root_layer_count(doc);
    /* A strict preflight ensures unsupported groups/formats never silently
     * disappear or leave half-rendered destination pixels. */
    for (li=0;li<count;++li) {
        PcPixelFormat fmt;
        PcRaster *surface=pc_document_layer_raster((PcDocument *)doc,li);
        if (!surface || !pc_raster_get_format(surface,&fmt) ||
            fmt.mode!=PC_COLOR_RGB || fmt.sample!=PC_SAMPLE_U8 ||
            fmt.alpha!=1 || pc_raster_bytes_per_pixel(surface)!=4) return 0;
    }
    for (y=0;y<(size_t)pc_rect_height(r);++y) {
        uint8_t *line=dst+y*stride;
        memset(line,0,rowbytes);
        for (li=0;li<count;++li) {
            unsigned opacity;
            const uint64_t id=pc_document_layer_id(doc,li);
            PcRaster *surface=pc_document_layer_raster((PcDocument *)doc,li);
            int enabled=pc_document_layer_visible(doc,li);
            int has_mask=pc_document_layer_mask((PcDocument *)doc,id)!=NULL;
            if (enabled<=0) continue;
            opacity=pc_scale(pc_fraction_255(pc_document_layer_opacity(doc,li)),
                             pc_fraction_255(pc_document_layer_fill_opacity(doc,li)));
            if (!opacity) continue;
            for (x=0;x<(size_t)pc_rect_width(r);++x) {
                uint8_t pixel[4];
                unsigned coverage=255,alpha;
                int32_t xx=r.x0+(int32_t)x,yy=r.y0+(int32_t)y;
                if (!pc_raster_read_pixel(surface,xx,yy,pixel,4)) return 0;
                if (has_mask && pc_document_mask_enabled(doc,id)>0) {
                    uint8_t mask_value;
                    if (!pc_document_mask_value_u8(doc,id,xx,yy,&mask_value)) return 0;
                    coverage=(unsigned)mask_value;
                }
                alpha=pc_scale(pc_scale((unsigned)pixel[3],opacity),coverage);
                pc_over(line+x*4,pixel,alpha);
            }
        }
    }
    return 1;
}
