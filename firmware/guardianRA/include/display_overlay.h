#ifndef DISPLAY_OVERLAY_H
#define DISPLAY_OVERLAY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Draw a 5x7 ASCII uppercase string into a 32-bit framebuffer (ARGB8888 or XRGB8888).
 * - base: pointer to framebuffer start
 * - w,h: framebuffer width/height in pixels
 * - stride: line stride in pixels (not bytes)
 * - x,y: top-left of the string in pixels
 * - color: 0x00RRGGBB or 0xAARRGGBB depending on format
 */
void overlay_draw_string(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                         uint32_t x, uint32_t y, const char *s, uint32_t color);

/* Convenience: print status line with anomaly state and error value. */
void overlay_draw_status(uint32_t *base, uint32_t w, uint32_t h, uint32_t stride,
                         uint32_t x, uint32_t y, float err, float thr,
                         int ae_label, int heur_label);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_OVERLAY_H */
