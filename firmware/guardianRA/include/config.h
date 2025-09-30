#ifndef CONFIG_H
#define CONFIG_H

/* Global configuration for µSentinel firmware */

/* Monitoring period in milliseconds */
#define MONITOR_PERIOD_MS   (200)

/* How many features to acquire per sample. Must match offline export FEATURE_DIM. */
#ifndef FEATURE_DIM
#define FEATURE_DIM (32)
#endif

/* Optional: enable heuristic baseline alongside AE */
#define ENABLE_HEURISTIC  (1)

/* Optional: print CSV-style compact line in addition to human-readable line */
#define ENABLE_CSV_LOG    (1)

/* CSV header printed once at boot if ENABLE_CSV_LOG */
#define CSV_HEADER "time_ms,err,ema,thr,ae_label,heur_label,ctx_switches,isr_count,mem_free,mem_total,f0,f1,f2,f3,f4,f5"

/* Periodic summary cadence (in samples) */
#define SUMMARY_EVERY_SAMPLES (25)

#endif /* CONFIG_H */
