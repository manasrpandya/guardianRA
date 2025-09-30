#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Snapshot of system metrics for logging and heuristics */
typedef struct {
    uint32_t time_ms;
    uint32_t tick;
    uint32_t ctx_switches;   /* placeholder */
    uint32_t isr_count;      /* placeholder */
    uint32_t mem_free_bytes; /* placeholder */
    uint32_t mem_total_bytes;/* placeholder */
} diag_snapshot_t;

/* Initialize diagnostics subsystem (hook timers/counters if available). */
void diag_init(void);

/* Read current snapshot. In this sample, values are derived from system tick. */
diag_snapshot_t diag_read(void);

#ifdef __cplusplus
}
#endif

#endif /* DIAGNOSTICS_H */
