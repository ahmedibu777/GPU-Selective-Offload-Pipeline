# GPU-Accelerated Selective Data Offload Pipeline for Autonomous Vehicles

This repository contains the **Review 1 CPU-first selective data offload prototype**
for BMSCE course `24AM5PWMPW`.

The system selects high-value autonomous-vehicle sensor events, applies a deterministic
safety guard, assigns deadline-aware priorities, and offloads selected payloads over a
constrained network using bounded 64 KiB chunks to reduce head-of-line blocking.

## Locked Review 1 scope

- CPU-only C++17 implementation.
- FIFO versus Guarded Selective Priority offload.
- Multi-factor event value scoring.
- Safety-guarded, deadline-aware priority scheduling.
- Controlled Linux network-stress evaluation.
- Python-based metric aggregation and plotting.

**Out of scope for Review 1:** CUDA kernels, TensorRT engines, Jetson binaries, GPU
inference, and GPU timing measurements. These are Major Project extensions only.

## Build

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## Sanitized build

```bash
cmake -S . -B build-sanitize -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DOFFLOAD_ENABLE_SANITIZERS=ON

cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

## Run demo

```bash
./build/offload_pipeline \
  --frames 100 \
  --budget-bytes 262144 \
  --policy combined
```

Available policies:

```text
upload-all
fifo
random
value-only
deadline-only
combined
```

## Core prioritization formula

For event or frame `i`:

```text
S_i = w1 * R_i + w2 * U_i + w3 * C_i - w4 * A_i
P_i = S_i / D_i
```

Where:

- `R_i` is rarity or novelty.
- `U_i` is uncertainty or anomaly confidence.
- `C_i` is safety criticality.
- `A_i` is similarity/redundancy penalty.
- `D_i` is remaining slack time before the delivery deadline.
- `P_i` is the scheduling priority; higher values transmit earlier.

## Literature anchors

- [M. Waseem Ahmed, G. Kavitha, 2026, Guarded deep reinforcement learning for SLA latency and energy aware task scheduling in IoT edge cloud environments, Discover Computing]
- [R. Feng, Y. Yao, E. M. Atkins, 2021, Smart Black Box 2.0: Efficient High-Bandwidth Driving Data Collection Based on Video Anomalies, Algorithms]
- [S. Wang, C. Tu, H. Chen, 2026, D-Compress: Detail-Preserving LiDAR Range Image Compression for Real-Time Streaming on Resource-Constrained Robots, arXiv preprint arXiv:2603.13699, Presented at IEEE ICRA 2026]

## Repository policy

This repository intentionally excludes GPU-specific code for Review 1. CUDA, TensorRT,
Nsight, and Jetson deployment are documented as Major Project extensions.
