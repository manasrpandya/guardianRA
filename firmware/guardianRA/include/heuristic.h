#ifndef HEURISTIC_H
#define HEURISTIC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Simple heuristic IDS rules over standardized features. */

typedef struct {
    float z_thresh;       /* per-feature z-score alarm threshold */
    uint16_t drift_win;   /* moving average window (samples) */
    float drift_thresh;   /* moving average threshold */
} heuristic_cfg_t;

/* Initialize with sane defaults. */
void heuristic_init(heuristic_cfg_t *cfg);

/* Evaluate heuristics on raw feature vector x of length dim using provided scaler mean/std.
   Returns 1 if anomaly, 0 otherwise. */
int heuristic_eval(const float *x, int dim,
                   const float *mean, const float *std,
                   const heuristic_cfg_t *cfg);

#ifdef __cplusplus
}
#endif

#endif /* HEURISTIC_H */
