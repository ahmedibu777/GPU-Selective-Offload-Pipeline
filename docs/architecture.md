# Architecture

## Review 1 CPU pipeline

The Review 1 system is a CPU-only selective-offload pipeline. It receives simulated
autonomous-vehicle sensor frames, computes a value score, applies a deterministic safety
guard, assigns a deadline-aware priority, and transmits selected payloads in bounded
network chunks.

```text
Sensor Frame Input
        |
        v
Event Selector
        |
        +--> Guard Rejection
        |
        v
Priority Queue
        |
        v
Offload Worker
        |
        +--> 64 KiB TCP Chunks
        |
        v
Receiver / Metrics Collector
```

## Dual operating modes

| Mode | Selection policy | Scheduling policy | Purpose |
|---|---|---|---|
| FIFO | Upload all frames in arrival order | First-in, first-out | Baseline |
| Guarded Selective Priority | Retain only events passing the safety guard and value threshold | Highest `P_i` first | Proposed CPU prototype |

## Value and priority scoring

For each candidate event:

```text
S_i = w1 * R_i + w2 * U_i + w3 * C_i - w4 * A_i
P_i = S_i / D_i
```

The safety guard rejects events that violate hard safety constraints, such as a missing
safety-critical label or zero remaining slack time.

## Anti-head-of-line chunking

The offload worker divides each selected payload into chunks of at most 64 KiB before
transmission. This limits the impact of a large frame on subsequent smaller, high-priority
frames and reduces head-of-line blocking.

## Major Project extension

CUDA preprocessing, TensorRT inference, and Jetson deployment are explicitly outside
Review 1. They are represented as a separate Major Project extension and must not be
presented as Review 1 deliverables.
