#!/bin/bash

cleanup() {
    echo "Terminating..."

    script_pid=$$

    # Find all process groups associated with the script
    pgids=$(ps -o pgid= -p $(pgrep -P $script_pid))

    # Kill all process groups
    for pgid in $pgids; do
        echo "Killing process group $pgid"
        kill -- "-$pgid"
    done
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
    setsid ./run_job.sh "$accelerator_name" "$min_core" &

    ((core_jobs[$min_core]++))
	min_jobs=${core_jobs[$min_core]}
done

# Wait for all jobs to finish
echo ""
echo "Waiting for all jobs to finish..."
wait

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
