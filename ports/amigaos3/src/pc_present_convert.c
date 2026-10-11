#include "pc_present_convert.h"
#include <stdint.h>
int pc_rgba8_to_argb32(const uint8_t *src, size_t ss,
                       uint32_t *dst, size_t ds, size_t w, size_t h)
{
    size_t x, y, n;
    if (!src || !dst || !w || !h || w > SIZE_MAX/4) return 0;
    n = w * 4;
    if (ss < n || ds < w) return 0;
    if (h > 1 && (ss > (SIZE_MAX-n)/(h-1) ||
                  ds > (SIZE_MAX/sizeof(*dst)-w)/(h-1))) return 0;
    for (y = 0; y < h; ++y) {
        const uint8_t *row = src + y*ss;
        uint32_t *out = dst + y*ds;
        for (x = 0; x < w; ++x) {
            const uint8_t *p = row + x*4;
            out[x] = ((uint32_t)p[3]<<24) | ((uint32_t)p[0]<<16) |
                     ((uint32_t)p[1]<<8) | (uint32_t)p[2];
        }
    }
    return 1;
}
