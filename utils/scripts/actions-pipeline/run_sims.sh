#!/bin/bash

list_descendants() {
  local children=$(ps -o pid= --ppid "$1")

  for pid in $children; do
    list_descendants "$pid"
  done

  echo "$children"
}

cleanup() {
  echo "Terminating all child processes and their descendants..."
  local descendants=$(list_descendants $$)
  kill $descendants
  echo "Exiting..."
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

for ((i=0; i<CORES; i++)); do
    core_jobs[$i]=0
done

results=()
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
	sim=$(jq --arg name "$accelerator_name" '.accelerators[] | select(.name == $name)' "$ACCELERATORS" | jq -r '.sim')

    (cd ~/esp/socs/xilinx-vc707-xc7vx485t && taskset -c "$min_core" setsid make "$sim" > "logs/hls/${accelerator_name}_sim.log" 2>&1) &
	child_processes+=("$!")

	echo "Process ${child_processes[-1]}"

    ((core_jobs[$min_core]++))
	min_jobs=${core_jobs[$min_core]}
done

# Wait for all jobs to finish
echo ""
echo "Waiting for all jobs to finish..."

# Check the exit status of each job and store the result
for accelerator_name in "${modified_accelerators[@]}"; do
    if wait "${!}"; then
        results+=("$accelerator_name: pass")
    else
        results+=("$accelerator_name: fail")
    fi
	core=$(ps -o psr= -p $! | awk '{print $1}')
	((core_jobs[core]--))
done

# Print job results
echo ""
echo "Job Results:"
printf '%s\n' "${results[@]}"
