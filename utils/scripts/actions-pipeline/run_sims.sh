#!/bin/bash

list_descendants() {
  local children=$(ps -o pid= --ppid "$1")

  for pid in $children; do
    list_descendants "$pid"
  done

  echo "$children"
}

cleanup() {
  echo ""
  echo "Terminating all child processes and their descendants..."
  local descendants=$(list_descendants $$)
  for pid in $descendants; do
    if kill -0 "$pid" &>/dev/null; then
      kill "$pid"
    fi
  done
  echo "Done!"
  exit
}


# Trap SIGINT signal (Ctrl+C) to run cleanup function
trap cleanup SIGINT

echo "Getting modified accelerators..."
source get_modified_accelerators.sh

echo ""
echo "Sourcing tools"
source /opt/cad/scripts/tools_env.sh
echo ""

CORES=$(nproc)
declare -A core_jobs
declare -A job_names  # Associative array to store accelerator name for each job

for ((i=0; i<CORES; i++)); do
    core_jobs[$i]=0
done

child_processes=()

min_core=0
min_jobs=100
for accelerator_name in "${modified_accelerators[@]}"; do
    for ((i=0; i<CORES; i++)); do
        if (( core_jobs[$i] < min_jobs )); then
            min_core=$i
            break
        fi
    done

    echo "Starting RTL simulation for $accelerator_name on CPU core $min_core"
    hls=$(jq --arg name "$accelerator_name" '.accelerators[] | select(.name == $name)' "$ACCELERATORS" | jq -r '.hls')

    (cd ~/esp/socs/xilinx-vc707-xc7vx485t && taskset -c "$min_core" setsid make "$hls" > "logs/hls/${accelerator_name}_test.log" 2>&1) &
    child_processes+=("$!")

    job_names[$!]="$accelerator_name"  # Associate PID with accelerator name

    echo "Process ${child_processes[-1]}"

    ((core_jobs[$min_core]++))
    min_jobs=${core_jobs[$min_core]}
done

# Wait for each job to finish and update results as each process completes
echo ""
echo "Waiting for all jobs to finish..."

for pid in "${child_processes[@]}"; do
    wait "$pid"
    # Print the latest job result with accelerator name
	echo "HLS completed for acc: ${job_names[$pid]}"
done