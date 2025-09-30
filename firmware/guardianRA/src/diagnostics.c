#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "diagnostics.h"

static inline unsigned long systime_ms_local(void)
{
    return (unsigned long)(tm_systim_l() & 0xFFFFFFFFu);
}

void diag_init(void)
{
    /* In a production build, this would hook timers, RTOS counters, etc. */
}

diag_snapshot_t diag_read(void)
{
    const unsigned long t = systime_ms_local();
    diag_snapshot_t s;
    s.time_ms = (uint32_t)t;
    s.tick = (uint32_t)(t / 10u);
    /* Placeholder counters derived deterministically from time. */
    s.ctx_switches    = (uint32_t)((t / 2u) % 100000u);
    s.isr_count       = (uint32_t)((t / 3u) % 50000u);
    s.mem_total_bytes = (uint32_t)(512u * 1024u); /* example: 512 KiB */
    s.mem_free_bytes  = (uint32_t)(200u * 1024u + (t % 1000u));
    return s;
}
