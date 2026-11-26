#!/usr/bin/env bash
set -euo pipefail

# Frozen Review 1 link-stress profile:
# Name: constrained-urban-cellular
# Bandwidth: 8 Mbps
# One-way latency: 60 ms
# Packet loss: 0.5 percent

INTERFACE="${1:-eth0}"
BANDWIDTH="${2:-8mbit}"
LATENCY="${3:-60ms}"
LOSS="${4:-0.5%}"

if ! command -v tc >/dev/null 2>&1; then
    echo "tc is unavailable. Use the application-layer token-bucket fallback."
    exit 0
fi

if ! command -v iperf3 >/dev/null 2>&1; then
    echo "iperf3 is unavailable. Install it with: sudo apt install iperf3"
    exit 1
fi

sudo tc qdisc replace dev "$INTERFACE" root netem \
    rate "$BANDWIDTH" \
    delay "$LATENCY" \
    loss "$LOSS"

echo "Applied link stress:"
echo "  Interface: $INTERFACE"
echo "  Bandwidth: $BANDWIDTH"
echo "  Delay: $LATENCY"
echo "  Loss: $LOSS"
echo "Run iperf3 separately against the configured receiver."
