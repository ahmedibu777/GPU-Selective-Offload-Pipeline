#!/usr/bin/env python3
import argparse
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("metrics_csv", type=Path)
    parser.add_argument(
        "--output",
        type=Path,
        default=Path("output/metrics.png"),
    )
    args = parser.parse_args()

    frame = pd.read_csv(args.metrics_csv)

    required = {
        "policy",
        "event_recall",
        "bandwidth_reduction",
        "latency_p95_ms",
        "deadline_miss_rate",
    }

    missing = required.difference(frame.columns)

    if missing:
        raise ValueError(f"Missing columns: {sorted(missing)}")

    axes = frame.plot(
        x="policy",
        y=[
            "event_recall",
            "bandwidth_reduction",
            "latency_p95_ms",
            "deadline_miss_rate",
        ],
        kind="bar",
        subplots=True,
        layout=(2, 2),
        figsize=(12, 8),
        legend=False,
    )

    for axis in axes.ravel():
        axis.grid(True, alpha=0.3)

    plt.tight_layout()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(args.output, dpi=300, transparent=True)
    print(f"Saved plot to {args.output}")


if __name__ == "__main__":
    main()
