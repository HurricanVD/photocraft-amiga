#include "pc_core.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define TILE_BYTES ((size_t)PC_CORE_TILE_SIZE * (size_t)PC_CORE_TILE_SIZE * 4u)

typedef struct PcTile {
    size_t refs;
    uint8_t bytes[PC_CORE_TILE_SIZE * PC_CORE_TILE_SIZE * 4u];
} PcTile;
typedef struct PcTileEntry { PcTileCoord coord; PcTile *tile; } PcTileEntry;
struct PcSurface {
    uint8_t default_pixel[4];
    PcTileEntry *entries;
    size_t count;
    size_t capacity;
};

static int32_t sat_i32(int64_t v)
{
    if (v > INT32_MAX) return INT32_MAX;
    if (v < INT32_MIN) return INT32_MIN;
    return (int32_t)v;
}
static int32_t min_i32(int32_t a, int32_t b) { return a < b ? a : b; }
static int32_t max_i32(int32_t a, int32_t b) { return a > b ? a : b; }
static int32_t div_euclid_256(int32_t value)
{
    int32_t q = value / PC_CORE_TILE_SIZE;
    if (value % PC_CORE_TILE_SIZE < 0) --q;
    return q;
}
static size_t mod_euclid_256(int32_t value)
{
    int32_t r = value % PC_CORE_TILE_SIZE;
    return (size_t)(r < 0 ? r + PC_CORE_TILE_SIZE : r);
}
PcRect pc_rect_new(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    PcRect r = {x0, y0, x1, y1}; return r;
}
PcRect pc_rect_from_xywh(int32_t x, int32_t y, uint32_t w, uint32_t h)
{
    /* Rust converts (w as i32) before saturating_add; avoid
     * implementation-defined C conversion of large uint32_t. */
    int32_t wi = (w <= INT32_MAX) ? (int32_t)w : (int32_t)((int64_t)w - INT64_C(4294967296));
    int32_t hi = (h <= INT32_MAX) ? (int32_t)h : (int32_t)((int64_t)h - INT64_C(4294967296));
    return pc_rect_new(x, y, sat_i32((int64_t)x + wi), sat_i32((int64_t)y + hi));
}
uint32_t pc_rect_width(PcRect r)
{
    return r.x1 > r.x0 ? (uint32_t)((int64_t)r.x1 - r.x0) : 0u;
}
uint32_t pc_rect_height(PcRect r)
{
    return r.y1 > r.y0 ? (uint32_t)((int64_t)r.y1 - r.y0) : 0u;
}
int pc_rect_empty(PcRect r) { return r.x1 <= r.x0 || r.y1 <= r.y0; }
int pc_rect_contains(PcRect r, int32_t x, int32_t y)
{
    return x >= r.x0 && x < r.x1 && y >= r.y0 && y < r.y1;
}
PcRect pc_rect_intersect(PcRect a, PcRect b)
{
    PcRect r = pc_rect_new(max_i32(a.x0, b.x0), max_i32(a.y0, b.y0),
                           min_i32(a.x1, b.x1), min_i32(a.y1, b.y1));
    return pc_rect_empty(r) ? pc_rect_new(0, 0, 0, 0) : r;
}
PcRect pc_rect_union(PcRect a, PcRect b)
{
    if (pc_rect_empty(a)) return b;
    if (pc_rect_empty(b)) return a;
    return pc_rect_new(min_i32(a.x0,b.x0), min_i32(a.y0,b.y0),
                       max_i32(a.x1,b.x1), max_i32(a.y1,b.y1));
}
PcRect pc_rect_translate(PcRect r, int32_t dx, int32_t dy)
{
    return pc_rect_new(sat_i32((int64_t)r.x0 + dx),
                       sat_i32((int64_t)r.y0 + dy),
                       sat_i32((int64_t)r.x1 + dx),
                       sat_i32((int64_t)r.y1 + dy));
}
PcTileCoord pc_tile_containing(int32_t x, int32_t y)
{
    PcTileCoord c = {div_euclid_256(x), div_euclid_256(y)}; return c;
}
int pc_tile_equal(PcTileCoord a, PcTileCoord b)
{
    return a.tx == b.tx && a.ty == b.ty;
}
int pc_tile_rect(PcTileCoord c, PcRect *out)
{
    int64_t x = (int64_t)c.tx * PC_CORE_TILE_SIZE;
    int64_t y = (int64_t)c.ty * PC_CORE_TILE_SIZE;
    if (!out || x < INT32_MIN || y < INT32_MIN ||
        x + PC_CORE_TILE_SIZE > INT32_MAX ||
        y + PC_CORE_TILE_SIZE > INT32_MAX) return 0;
    *out = pc_rect_new((int32_t)x, (int32_t)y,
                       (int32_t)(x + PC_CORE_TILE_SIZE),
                       (int32_t)(y + PC_CORE_TILE_SIZE));
    return 1;
}
size_t pc_format_channels(PcPixelFormat f)
{
    size_t channels;
    switch (f.mode) {
    case PC_COLOR_BITMAP: case PC_COLOR_GRAY: case PC_COLOR_DUOTONE:
        channels = 1; break;
    case PC_COLOR_INDEXED: case PC_COLOR_RGB: case PC_COLOR_LAB:
    case PC_COLOR_MULTICHANNEL:
        channels = 3; break;
    case PC_COLOR_CMYK: channels = 4; break;
    default: return 0;
    }
    return channels + (f.alpha != 0);
}
size_t pc_format_bytes_per_pixel(PcPixelFormat f)
{
    size_t n = pc_format_channels(f);
    switch (f.sample) {
    case PC_SAMPLE_U8: return n;
    case PC_SAMPLE_U16: return n * 2;
    case PC_SAMPLE_F32: return n * 4;
    default: return 0;
    }
}
static void tile_release(PcTile *t)
{
    if (t && --t->refs == 0) free(t);
}
static PcTile *tile_new(const uint8_t def[4])
{
    size_t i;
    PcTile *t = (PcTile *)malloc(sizeof(*t));
    if (!t) return NULL;
    t->refs = 1;
    if (def[0] == 0 && def[1] == 0 && def[2] == 0 && def[3] == 0) {
        memset(t->bytes, 0, TILE_BYTES);
    } else {
        for (i=0; i<TILE_BYTES; i+=4) memcpy(t->bytes+i, def, 4);
    }
    return t;
}
PcSurface *pc_surface_rgba8_new(const uint8_t default_pixel[4])
{
    PcSurface *s = (PcSurface *)calloc(1, sizeof(*s));
    if (!s) return NULL;
    if (default_pixel) memcpy(s->default_pixel, default_pixel, 4);
    return s;
}
PcSurface *pc_surface_clone(const PcSurface *s)
{
    PcSurface *copy;
    size_t i;
    if (!s) return NULL;
    copy = pc_surface_rgba8_new(s->default_pixel);
    if (!copy) return NULL;
    if (s->count) {
        if (s->count > SIZE_MAX/sizeof(*copy->entries)) {
            free(copy); return NULL;
        }
        copy->entries = (PcTileEntry *)malloc(s->count*sizeof(*copy->entries));
        if (!copy->entries) { free(copy); return NULL; }
        memcpy(copy->entries, s->entries, s->count*sizeof(*copy->entries));
        for (i=0; i<s->count; ++i) {
            if (copy->entries[i].tile->refs == SIZE_MAX) {
                while (i) --copy->entries[--i].tile->refs;
                free(copy->entries); free(copy); return NULL;
            }
            ++copy->entries[i].tile->refs;
        }
        copy->count = copy->capacity = s->count;
    }
    return copy;
}
void pc_surface_destroy(PcSurface *s)
{
    size_t i;
    if (!s) return;
    for (i=0; i<s->count; ++i) tile_release(s->entries[i].tile);
    free(s->entries);
    free(s);
}
size_t pc_surface_tile_count(const PcSurface *s)
{
    return s ? s->count : 0;
}
static size_t find_tile(const PcSurface *s, PcTileCoord c)
{
    size_t i;
    for (i=0; i<s->count; ++i)
        if (pc_tile_equal(s->entries[i].coord,c)) return i;
    return s->count;
}
static size_t tile_offset(int32_t x, int32_t y)
{
    return (mod_euclid_256(y)*PC_CORE_TILE_SIZE + mod_euclid_256(x))*4;
}
int pc_surface_read_rgba8(const PcSurface *s, int32_t x, int32_t y,
                          uint8_t rgba[4])
{
    PcTileCoord c;
    size_t pos;
    if (!s || !rgba) return 0;
    c = pc_tile_containing(x,y);
    pos = find_tile(s,c);
    memcpy(rgba, pos == s->count ? s->default_pixel :
           s->entries[pos].tile->bytes + tile_offset(x,y), 4);
    return 1;
}
static int grow_entries(PcSurface *s)
{
    size_t next;
    PcTileEntry *p;
    if (s->count < s->capacity) return 1;
    next = s->capacity ? s->capacity*2 : 4;
    if (next < s->capacity || next > SIZE_MAX/sizeof(*p)) return 0;
    p = (PcTileEntry *)realloc(s->entries, next*sizeof(*p));
    if (!p) return 0;
    s->entries = p;
    s->capacity = next;
    return 1;
}
int pc_surface_write_rgba8(PcSurface *s, int32_t x, int32_t y,
                           const uint8_t rgba[4])
{
    PcTileCoord c;
    size_t pos;
    PcTile *t;
    if (!s || !rgba) return 0;
    c = pc_tile_containing(x,y);
    pos = find_tile(s,c);
    if (pos == s->count) {
        if (!grow_entries(s)) return 0;
        t = tile_new(s->default_pixel);
        if (!t) return 0;
        s->entries[s->count].coord = c;
        s->entries[s->count].tile = t;
        ++s->count;
    } else {
        t = s->entries[pos].tile;
        if (t->refs > 1) {
            PcTile *replacement = (PcTile *)malloc(sizeof(*replacement));
            if (!replacement) return 0;
            memcpy(replacement, t, sizeof(*replacement));
            replacement->refs = 1;
            tile_release(t);
            t = replacement;
            s->entries[pos].tile = t;
        }
    }
    memcpy(t->bytes + tile_offset(x,y), rgba, 4);
    return 1;
}
void pc_surface_prune(PcSurface *s)
{
    size_t i, keep=0, j;
    if (!s) return;
    for (i=0; i<s->count; ++i) {
        const uint8_t *data = s->entries[i].tile->bytes;
        int nondefault=0;
        for (j=0; j<TILE_BYTES; j+=4) {
            if (memcmp(data+j,s->default_pixel,4) != 0) {
                nondefault=1; break;
            }
        }
        if (nondefault) s->entries[keep++] = s->entries[i];
        else tile_release(s->entries[i].tile);
    }
    s->count = keep;
}
