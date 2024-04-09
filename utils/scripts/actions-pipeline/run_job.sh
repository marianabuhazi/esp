#!/bin/bash

# Usage: ./run_job.sh <accelerator_name> <cpu_core>
ACCELERATORS="accelerators.json"

accelerator_name="$1"
cpu_core="$2"
sim=$(jq --arg name "$accelerator_name" '.accelerators[] | select(.name == $name)' "$ACCELERATORS" | jq -r '.sim')

# Set affinity for the specified CPU core
cd ~/esp/socs/xilinx-vc707-xc7vx485t

# Redirect both stdout and stderr to the log file
taskset -c "$cpu_core" setsid make "$sim" > "logs/hls/${accelerator_name}_sim.log" 2>&1

if grep -q "validation passed!" "logs/hls/${accelerator_name}_sim.log"; then
    exit 0
else
    exit 1
fi
