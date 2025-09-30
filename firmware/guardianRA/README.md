# µSentinel Firmware (µT‑Kernel on EK‑RA8D1)

This directory contains a realistic firmware skeleton of the µSentinel runtime for the Renesas EK‑RA8D1 running µT‑Kernel 3.0. It performs periodic feature acquisition, evaluates a heuristic baseline and a lightweight autoencoder, maintains moving statistics, and logs results over T‑Monitor in both human‑readable and optional CSV form.

## Contents

- `src/usermain.c` – µT‑Kernel entry; spawns the monitor task.
- `src/monitor.c` – Core runtime loop: features → heuristics + AE → logs + stats.
- `src/features.c` – Replaceable feature acquisition layer (currently synthetic but structured for real hooks).
- `src/heuristic.c` – Simple, tunable heuristic rules (z‑score and drift proxy).
- `src/diagnostics.c` – Diagnostics snapshot (placeholders) to illustrate extensibility.
- `src/ringbuf.c`, `src/ringbuf.h` – Lightweight ring buffer for moving stats.
- `include/*.h` – Public includes for config, logging, heuristics, scaler/model headers.
- `include/scaler.h`, `include/ae_model.h` – Pre‑exported example headers (overwritten by offline export).
- `../jlink/flash_hex.jlink` and `../jlink/flash_full_erase_hex.jlink` – J‑Link scripts for flashing.

## Build and Flash (Evaluator Summary)

1) Prerequisites
- e² studio with RA FSP for EK‑RA8D1
- µT‑Kernel 3.0 BSP2 for Renesas RA/FSP integrated in your workspace
- SEGGER J‑Link (JLinkExe on PATH)

2) Import into e² studio
- File → Import → Existing Projects into Workspace
- Select this folder: `firmware/guardianRA/`
- Ensure compiler includes/defines resolve µT‑Kernel and RA/FSP headers
- Select a linker script suitable for EK‑RA8D1

3) Model headers
- The repository ships with example `scaler.h` and `ae_model.h` so the project builds out of the box.
- To regenerate with the offline pipeline, run at repo root:
  - `bash run.sh` (puts fresh headers into `include/`)

4) Build and Flash
- Build Debug in e² studio to produce a HEX.
- Edit `../jlink/flash_hex.jlink` to point to your HEX path.
- Flash from a terminal:
  - `JLinkExe -CommandFile ../jlink/flash_hex.jlink`

On boot, T‑Monitor prints lines like:
```
[µSentinel] t=123456 ms | err=0.732100 ema=0.654300 thr=1.238000 | AE=ok HEUR=ok | anom_total=0
123456,0.732100,1.238000,0,0,0.10123,0.11345,0.12678,0.14012,0.15347,0.16680
```
