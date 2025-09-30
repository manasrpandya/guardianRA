#include "events.h"

static volatile uint32_t s_heur_only = 0;
static volatile uint32_t s_ae_only = 0;
static volatile uint32_t s_both = 0;

void events_init(void)
{
    s_heur_only = 0;
    s_ae_only = 0;
    s_both = 0;
}

void events_record(event_kind_t kind, uint32_t time_ms, float err, float ema, int ae_label, int heur_label)
{
    (void)time_ms; (void)err; (void)ema; (void)ae_label; (void)heur_label; /* reserved for future buffering */
    if (kind == EV_HEUR_ONLY)      { s_heur_only++; }
    else if (kind == EV_AE_ONLY)   { s_ae_only++; }
    else if (kind == EV_BOTH)      { s_both++; }
}

void events_get_counts(uint32_t *heur_only, uint32_t *ae_only, uint32_t *both)
{
    if (heur_only) *heur_only = s_heur_only;
    if (ae_only) *ae_only = s_ae_only;
    if (both) *both = s_both;
}
