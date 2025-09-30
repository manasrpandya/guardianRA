#include "display_overlay.h"

/* 5x7 uppercase glyphs subset. Bits 4..0 left->right. 8 rows (last row blank) */
typedef struct { char ch; unsigned char rows[8]; } glyph5x7_t;

static const glyph5x7_t kGlyphs[] = {
    { ' ', { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 } },
    { 'A', { 0x0E,0x11,0x11,0x1F,0x11,0x11,0x00,0x00 } },
    { 'B', { 0x1E,0x11,0x1E,0x11,0x11,0x1E,0x00,0x00 } },
    { 'C', { 0x0E,0x11,0x10,0x10,0x11,0x0E,0x00,0x00 } },
    { 'D', { 0x1E,0x11,0x11,0x11,0x11,0x1E,0x00,0x00 } },
    { 'E', { 0x1F,0x10,0x1E,0x10,0x10,0x1F,0x00,0x00 } },
    { 'H', { 0x11,0x11,0x1F,0x11,0x11,0x11,0x00,0x00 } },
    { 'I', { 0x1F,0x04,0x04,0x04,0x04,0x1F,0x00,0x00 } },
    { 'L', { 0x10,0x10,0x10,0x10,0x10,0x1F,0x00,0x00 } },
    { 'N', { 0x11,0x19,0x15,0x13,0x11,0x11,0x00,0x00 } },
    { 'O', { 0x0E,0x11,0x11,0x11,0x11,0x0E,0x00,0x00 } },
    { 'R', { 0x1E,0x11,0x1E,0x14,0x12,0x11,0x00,0x00 } },
    { 'S', { 0x0F,0x10,0x0E,0x01,0x01,0x1E,0x00,0x00 } },
    { 'T', { 0x1F,0x04,0x04,0x04,0x04,0x04,0x00,0x00 } },
    { 'U', { 0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00 } },
    { 'W', { 0x11,0x11,0x15,0x0A,0x0A,0x11,0x00,0x00 } },
    { 'Y', { 0x11,0x11,0x0A,0x04,0x04,0x04,0x00,0x00 } },
    { ':', { 0x00,0x04,0x00,0x00,0x04,0x00,0x00,0x00 } },
    { '.', { 0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x00 } },
    { '-', { 0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00 } },
};

static inline void put_px(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                          uint32_t x, uint32_t y, uint32_t color)
{
    if (x < w && y < h) {
        base[y * stride + x] = color;
    }
}

static void draw_char(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                      uint32_t x, uint32_t y, char c, uint32_t color)
{
    const glyph5x7_t *g = 0;
    for (unsigned i = 0; i < sizeof(kGlyphs)/sizeof(kGlyphs[0]); ++i) {
        if (kGlyphs[i].ch == c) { g = &kGlyphs[i]; break; }
    }
    if (!g) return;
    for (uint32_t ry = 0; ry < 8; ++ry) {
        const unsigned char row = g->rows[ry];
        for (uint32_t rx = 0; rx < 5; ++rx) {
            if (row & (1u << (4u - rx))) {
                put_px(base, w, h, stride, x + rx, y + ry, color);
            }
        }
    }
}

void overlay_draw_string(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                         uint32_t x, uint32_t y, const char *s, uint32_t color)
{
    const uint32_t x_advance = 6; /* 5px glyph + 1 space */
    const uint32_t y_advance = 9; /* 8px glyph + 1 space (newline) */
    uint32_t cx = x, cy = y;
    while (*s) {
        if (*s == '\n') { cy += y_advance; cx = x; ++s; continue; }
        draw_char(base, w, h, stride, cx, cy, *s, color);
        cx += x_advance;
        ++s;
    }
}

void overlay_draw_status(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                         uint32_t x, uint32_t y, float err, float thr,
                         int ae_label, int heur_label)
{
    /* Format a short status: "SENTINEL AE:OK HEUR:OK ERR:0.123 THR:1.234" */
    char buf[96];
    const char *ae = ae_label ? "ANOM" : "OK";
    const char *he = heur_label ? "ANOM" : "OK";
    /* No snprintf here; craft a minimal static message for footprint */
    /* For simplicity, only draw the keywords; numeric values omitted in overlay */
    const char *prefix = "SENTINEL AE:";
    overlay_draw_string(base, w, h, stride, x, y, prefix, 0x00FFFFFFu);
    overlay_draw_string(base, w, h, stride, x + 6 * 12, y, ae, ae_label ? 0x0000FF00u : 0x0000FFFFu);
    overlay_draw_string(base, w, h, stride, x + 6 * 18, y, " HEUR:", 0x00FFFFFFu);
    overlay_draw_string(base, w, h, stride, x + 6 * 25, y, he, heur_label ? 0x0000FF00u : 0x0000FFFFu);
    (void)buf; (void)err; (void)thr;
}
