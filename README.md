# µSentinel: Real‑Time AI‑Based Intrusion Detection on µT‑Kernel

µSentinel is a real‑time intrusion detection system (IDS) implemented on the Renesas EK‑RA8D1 evaluation board running µT‑Kernel 3.0. It addresses the need for lightweight, trustworthy security monitoring in embedded and industrial environments by combining a rule‑based baseline with a lightweight autoencoder‑based anomaly detector optimized for the Arm Cortex‑M85. The AI model is designed for tight real‑time budgets, running in under 5 ms with a memory footprint below 30 KB in our configuration.

Conventional embedded IDS solutions rely on fixed thresholds and rules (for example, CPU usage, memory bounds, and timing checks). While efficient, such heuristics can miss subtle cross‑signal correlations and slowly evolving behaviors. µSentinel augments these capabilities with an autoencoder that captures joint structure across features and flags deviations through reconstruction error. To enable principled evaluation without access to proprietary industrial datasets, we provide controlled synthetic traces that reproduce realistic anomaly modes described in prior testbeds and industry reports, including timing violations, resource drifts, abnormal memory behavior, and cross‑feature inconsistencies. In our experiments, the AI detector reduces false positives by about 40% versus the heuristic baseline and identifies subtle, cross‑signal intrusions without violating real‑time constraints.

This repository contains a self‑contained offline reference demo and a minimal firmware skeleton. The offline demo simulates traces, runs both the heuristic and AI detectors, reports results, and exports C headers (`scaler.h`, `ae_model.h`). The firmware skeleton illustrates how to invoke the exported model in a µT‑Kernel task and report anomaly scores over T‑Monitor. Evaluators can run the offline demo to reproduce the results and then integrate the exported headers into their µT‑Kernel build for EK‑RA8D1.

## How to Run (Offline Demo)

Run on Linux/macOS with Python 3.9+ (I would advise to use WSL in case of windows for better usability).

```bash
cd offline
bash run_demo.sh
```

Console output compares the heuristic and AI detectors on synthetic traces and prints the selected anomaly threshold and anomaly fractions. Exported headers are written to `offline/model_export/` and also copied to `firmware/guardianRA/include/`.

## Firmware Integration (µT‑Kernel on EK‑RA8D1)

The firmware skeleton in `firmware/guardianRA/` demonstrates a minimal µT‑Kernel app that periodically acquires a feature vector, computes the autoencoder reconstruction error via `ae_model.h`, and prints results over T‑Monitor.

Build summary:

1. Install e² studio with RA FSP support for EK‑RA8D1 and µT‑Kernel 3.0 (BSP2 for Renesas RA/FSP).
2. Import `firmware/guardianRA/` as an existing project.
3. Ensure µT‑Kernel BSP2 and RA/FSP includes/defines are configured; place the linker script suitable for EK‑RA8D1.
4. Build `Debug` and flash with J‑Link using the template at `firmware/jlink/flash_hex.jlink` (edit the HEX path).

```bash
JLinkExe -CommandFile firmware/jlink/flash_hex.jlink
```

On boot, T‑Monitor prints periodic anomaly scores. The offline export step places `scaler.h` and `ae_model.h` in `firmware/guardianRA/include/`.

## Repository Layout

- `offline/` — Simulation, heuristic + AI detectors, export of `scaler.h` and `ae_model.h`.
- `firmware/guardianRA/` — Minimal µT‑Kernel app with T‑Monitor logging.
- `firmware/jlink/` — Example J‑Link script for flashing after a build.
- `run.sh` — Convenience script that invokes the offline demo end‑to‑end.

## License

MIT — see `LICENSE`.
