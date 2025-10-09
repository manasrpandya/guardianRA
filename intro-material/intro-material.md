# µSentinel: Real‑Time Intrusion Detection on µT‑Kernel

µSentinel brings on‑device awareness to the microcontrollers that drive modern factories, energy systems, and connected products. These devices keep equipment safe and productive, yet a misconfiguration, slow‑burn fault, or targeted intrusion can escalate into costly downtime and risk. Traditional monitoring relies on fixed rules—fast and simple, but often blind to subtle problems that appear only when several signals move together. µSentinel runs locally to learn what normal operation looks like across multiple signals and to flag deviations early, before small issues become big ones.

## Background and Motivation

Industrial systems demand reliability under tight timing and memory constraints. Sending everything to the cloud is not always possible or desirable, and simple thresholds alone can miss cross‑signal anomalies. µSentinel addresses this by combining classic, explainable rules with a compact AI model that captures correlations between signals while staying within the real‑time and memory limits of embedded hardware.

## Approach and Design

The firmware samples a compact set of operating signals—timing behavior, resource usage, and other health indicators—at a fixed cadence. Two detectors analyze the data: a rule‑based baseline for immediate checks, and a lightweight autoencoder that learns a reference pattern for “normal” and reports deviations via reconstruction error. The model is exported as plain C headers and runs in a few milliseconds on the Arm Cortex‑M85, avoiding external dependencies and preserving determinism.

## What Runs on EK‑RA8D1

On the EK‑RA8D1 board, µSentinel is a µT‑Kernel task that wakes periodically, gathers the feature vector, computes an anomaly score, and prints results over T‑Monitor. Output includes human‑readable summaries and CSV lines suitable for post‑analysis, with periodic summaries that report averages and event counts. An optional display overlay can render a short status line for demos. The code path per cycle is bounded and predictable, supporting consistent real‑time behavior.

## Reliability and Transparency

Transparency and safety are central to the implementation. The scaler and model parameters are exported as standard C headers, the inference routine is compact and auditable, and the rule‑based baseline remains active for cross‑checks. Because detection runs at the edge, it continues to function even when connectivity is limited, and operators can read logs directly from the device.

## Industry Impact

Adopting this pattern helps shift monitoring from reactive to proactive. Earlier and more accurate detection reduces false alarms, lowers downtime, and speeds triage when incidents occur. Brownfield deployments benefit without redesign: existing rule‑based monitoring remains, augmented by a compact model that learns normal behavior and highlights deviations. Over time, common benchmarks and repeatable evaluations can emerge without exposing proprietary plant data, enabling fair comparisons across solutions.

## Quick FAQ

What does the AI add beyond rules? It captures correlations across signals and is sensitive to gradual, multi‑signal drifts that fixed thresholds may miss, reducing false positives while improving coverage.

Does this fit real‑time constraints? Yes. The exported model runs in a few milliseconds with a small memory footprint on the EK‑RA8D1’s Arm Cortex‑M85.

Is the runtime a black box? No. All parameters are exported as C headers, and the inference routine is a short, inspectable function.

## Next Steps

To run the offline demo, build the firmware, and flash the board, please proceed to `README.md` in the repository root for step‑by‑step instructions.
