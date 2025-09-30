#include "heuristic.h"

static float fabsf_local(float x) { return x < 0.0f ? -x : x; }

void heuristic_init(heuristic_cfg_t *cfg)
{
    cfg->z_thresh = 4.0f;
    cfg->drift_win = 20;
    cfg->drift_thresh = 3.0f;
}

int heuristic_eval(const float *x, int dim,
                   const float *mean, const float *std,
                   const heuristic_cfg_t *cfg)
{
    // Rule 1: Any feature exceeding absolute z-score threshold
    for (int d = 0; d < dim; ++d)
    {
        const float z = (x[d] - mean[d]) / std[d];
        if (fabsf_local(z) > cfg->z_thresh) {
            return 1;
        }
    }

    // Rule 2: Moving average drift over last 'drift_win' samples.
    // Note: Without a persistent buffer in this minimal example, approximate by
    // using the current standardized vector magnitude as a proxy for drift.
    // In a full implementation, maintain a circular buffer per-feature.
    float mag = 0.0f;
    for (int d = 0; d < dim; ++d)
    {
        const float z = (x[d] - mean[d]) / std[d];
        mag += z * z;
    }
    // Compare RMS z to drift threshold.
    const float rms = mag / (float)dim;
    if (rms > (cfg->drift_thresh * cfg->drift_thresh))
    {
        return 1;
    }

    return 0;
}
