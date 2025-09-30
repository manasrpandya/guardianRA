#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "features.h"
#include "config.h"

/*
 * Example feature acquisition. In a production build, replace these
 * computations with hooks into µT-Kernel timing, resource counters,
 * IPC stats, and control-loop markers. This keeps the sample fully
 * self-contained yet structured for realistic extension.
 */

static inline unsigned long systime_ms_local(void)
{
    return (unsigned long)(tm_systim_l() & 0xFFFFFFFFu);
}

void features_acquire(float *x, int dim)
{
    const unsigned long t = systime_ms_local();
    const float t_s = (float)(t % 100000) / 1000.0f; /* ~100 s rollover */

    for (int i = 0; i < dim; ++i)
    {
        const float phase = 0.07f * (float)i;
        const float base = 0.002f * (float)(t % 1000);
        const float slow = 0.00015f * t_s + 0.03f * (float)i;
        /* Pseudo-sine via Taylor approx around small angles to keep math light */
        const float a = 0.002f * (float)((t / 10u) % 500u) + phase;
        const float psin = a - (a*a*a)/6.0f + (a*a*a*a*a)/120.0f;
        x[i] = base + slow + psin;
    }

    /* Simulate bursts and cross-feature inconsistencies */
    if ((t / 4000u) % 9u == 5u)
    {
        for (int j = 0; j < (dim/6); ++j) {
            x[j] += 0.9f; /* abrupt resource spike */
        }
        /* cross-feature inconsistency: invert sign of a block */
        for (int j = dim/3; j < dim/3 + dim/6; ++j) {
            x[j] = -x[j];
        }
    }
}
