#!/usr/bin/env python3
import argparse
import os
import shutil
from dataclasses import dataclass
import numpy as np

np.random.seed(7)

@dataclass
class ExportPaths:
    export_dir: str
    firmware_include: str


def ensure_dirs(paths: ExportPaths):
    os.makedirs(paths.export_dir, exist_ok=True)
    if paths.firmware_include:
        os.makedirs(paths.firmware_include, exist_ok=True)


def simulate_normal(n_samples: int, n_features: int) -> np.ndarray:
    x = np.zeros((n_samples, n_features), dtype=np.float32)
    coeff = 0.97
    noise = 0.03 * np.random.randn(n_samples, n_features).astype(np.float32)
    for t in range(1, n_samples):
        x[t] = coeff * x[t - 1] + noise[t]
    scales = np.linspace(0.5, 1.5, n_features).astype(np.float32)
    x *= scales
    return x


def inject_anomalies(x: np.ndarray, n_windows: int = 10, win: int = 50) -> np.ndarray:
    x2 = x.copy()
    n, d = x.shape
    starts = np.random.randint(low=100, high=max(101, n - win - 1), size=n_windows)
    for s in starts:
        idx = np.random.choice(d, size=max(2, d // 8), replace=False)
        spike = (np.random.randn(len(idx)) * 3.0).astype(np.float32)
        drift = (np.linspace(0, 2.0, win, dtype=np.float32).reshape(-1, 1))
        x2[s:s + win, idx] += spike + drift
    return x2


def fit_pca_autoencoder(x_train: np.ndarray, k_latent: int):
    mean = x_train.mean(axis=0)
    std = x_train.std(axis=0) + 1e-6
    xz = (x_train - mean) / std
    U, S, VT = np.linalg.svd(xz, full_matrices=False)
    comps = VT[:k_latent].T.astype(np.float32)
    return mean.astype(np.float32), std.astype(np.float32), comps


def ae_recon_error(x: np.ndarray, mean: np.ndarray, std: np.ndarray, comps: np.ndarray) -> np.ndarray:
    xz = (x - mean) / std
    z = xz @ comps
    xz_hat = z @ comps.T
    return ((xz - xz_hat) ** 2).sum(axis=1)


def choose_threshold(err: np.ndarray, sigma: float = 4.0) -> float:
    mu = float(np.mean(err))
    sd = float(np.std(err) + 1e-9)
    return mu + sigma * sd


def heuristic_detector(x: np.ndarray, mean: np.ndarray, std: np.ndarray,
                       z_thresh: float = 4.0, drift_win: int = 20, drift_thresh: float = 3.0) -> np.ndarray:
    xz = (x - mean) / std
    # Rule 1: any feature exceeding |z| threshold
    rule1 = (np.abs(xz) > z_thresh).any(axis=1)
    # Rule 2: moving average drift on each feature
    mov = np.convolve(np.ones(drift_win) / drift_win, np.ones(1), mode='full')  # dummy to satisfy linter
    # Compute per-feature moving average via cumulative sum for speed
    cs = np.cumsum(xz, axis=0)
    ma = np.empty_like(xz)
    ma[:drift_win] = 0
    ma[drift_win:] = (cs[drift_win:] - cs[:-drift_win]) / float(drift_win)
    rule2 = (np.abs(ma) > drift_thresh).any(axis=1)
    return rule1 | rule2


def export_headers(paths: ExportPaths, mean: np.ndarray, std: np.ndarray, comps: np.ndarray, thr: float):
    D = mean.shape[0]
    K = comps.shape[1]

    def to_c(arr):
        return ', '.join(f'{float(v):.8e}f' for v in arr.flatten())

    scaler_h = f"""
#ifndef SCALER_H
#define SCALER_H
#include <stdint.h>
#define FEATURE_DIM ({D})
static const float g_scaler_mean[FEATURE_DIM] = {{ {to_c(mean)} }};
static const float g_scaler_std[FEATURE_DIM]  = {{ {to_c(std)} }};
#endif
""".strip() + "\n"

    model_h = f"""
#ifndef AE_MODEL_H
#define AE_MODEL_H
#include <stdint.h>
#include "scaler.h"
#define LATENT_DIM ({K})
static const float g_components[FEATURE_DIM * LATENT_DIM] = {{ {to_c(comps)} }};
static inline float ae_infer_recon_error(const float* __restrict__ x) {{
    float z[LATENT_DIM];
    float xz_hat[FEATURE_DIM];
    for (int k = 0; k < LATENT_DIM; ++k) z[k] = 0.0f;
    for (int d = 0; d < FEATURE_DIM; ++d) {{
        const float xz = (x[d] - g_scaler_mean[d]) / g_scaler_std[d];
        const int base = d * LATENT_DIM;
        for (int k = 0; k < LATENT_DIM; ++k) z[k] += xz * g_components[base + k];
    }}
    for (int d = 0; d < FEATURE_DIM; ++d) xz_hat[d] = 0.0f;
    for (int d = 0; d < FEATURE_DIM; ++d) {{
        const int base = d * LATENT_DIM;
        float acc = 0.0f;
        for (int k = 0; k < LATENT_DIM; ++k) acc += z[k] * g_components[base + k];
        xz_hat[d] = acc;
    }}
    float err = 0.0f;
    for (int d = 0; d < FEATURE_DIM; ++d) {{
        const float xz = (x[d] - g_scaler_mean[d]) / g_scaler_std[d];
        const float diff = xz - xz_hat[d];
        err += diff * diff;
    }}
    return err;
}}
static const float g_anomaly_threshold = {thr:.8e}f;
#endif
""".strip() + "\n"

    os.makedirs(paths.export_dir, exist_ok=True)
    with open(os.path.join(paths.export_dir, 'scaler.h'), 'w') as f:
        f.write(scaler_h)
    with open(os.path.join(paths.export_dir, 'ae_model.h'), 'w') as f:
        f.write(model_h)
    if paths.firmware_include:
        os.makedirs(paths.firmware_include, exist_ok=True)
        shutil.copy(os.path.join(paths.export_dir, 'scaler.h'), os.path.join(paths.firmware_include, 'scaler.h'))
        shutil.copy(os.path.join(paths.export_dir, 'ae_model.h'), os.path.join(paths.firmware_include, 'ae_model.h'))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--export-dir', type=str, default='model_export')
    ap.add_argument('--firmware-include', type=str, default='')
    ap.add_argument('--samples', type=int, default=6000)
    ap.add_argument('--features', type=int, default=32)
    ap.add_argument('--latent', type=int, default=8)
    args = ap.parse_args()

    paths = ExportPaths(args.export_dir, args.firmware_include)
    ensure_dirs(paths)

    print('[µSentinel] Simulating normal...')
    x = simulate_normal(args.samples, args.features)
    n_train = int(0.7 * len(x))
    x_tr, x_val = x[:n_train], x[n_train:]

    print('[µSentinel] Fitting lightweight AE (PCA) ...')
    mean, std, comps = fit_pca_autoencoder(x_tr, args.latent)

    print('[µSentinel] Selecting AE threshold on normal validation...')
    err_val = ae_recon_error(x_val, mean, std, comps)
    thr = choose_threshold(err_val, sigma=4.0)

    print('[µSentinel] Generating synthetic intrusions ...')
    x_test = inject_anomalies(x_val, n_windows=8, win=40)

    print('[µSentinel] Running detectors ...')
    y_h = heuristic_detector(x_test, mean, std)
    err_test = ae_recon_error(x_test, mean, std, comps)
    y_ae = err_test > thr

    print(f"[µSentinel] Heuristic anomaly rate: {np.mean(y_h):.3f}")
    print(f"[µSentinel] AE threshold: {thr:.6f}  |  AE anomaly rate: {np.mean(y_ae):.3f}")

    print('[µSentinel] Exporting C headers ...')
    export_headers(paths, mean, std, comps, thr)
    print('[µSentinel] Done.')


if __name__ == '__main__':
    main()
