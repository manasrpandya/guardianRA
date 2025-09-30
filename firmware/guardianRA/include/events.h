#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EV_NONE = 0,
    EV_HEUR_ONLY = 1,
    EV_AE_ONLY = 2,
    EV_BOTH = 3
} event_kind_t;

typedef struct {
    uint32_t time_ms;
    float err;
    float ema;
    uint8_t ae_label;
    uint8_t heur_label;
} event_t;

/* Initialize event counters and buffer. */
void events_init(void);

/* Record an event of the given kind with the provided parameters. */
void events_record(event_kind_t kind, uint32_t time_ms, float err, float ema, int ae_label, int heur_label);

/* Snapshot cumulative counts since boot. */
void events_get_counts(uint32_t *heur_only, uint32_t *ae_only, uint32_t *both);

#ifdef __cplusplus
}
#endif

#endif /* EVENTS_H */
