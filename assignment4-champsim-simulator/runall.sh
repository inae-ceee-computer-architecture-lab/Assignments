#!/bin/bash
BINARY=$1
TRACE_DIR=$2
OUT_DIR=$3

mkdir -p "$OUT_DIR"

for TRACE in "$TRACE_DIR"/*; do
    TRACE_NAME=$(basename "$TRACE")
    echo "Running $TRACE_NAME..."
    bash run_champsim.sh "$TRACE" "$BINARY" > "$OUT_DIR/log_summary_${TRACE_NAME}_${BINARY}.log" &
done

wait  # Wait for all background jobs to finish
echo "All runs completed."
