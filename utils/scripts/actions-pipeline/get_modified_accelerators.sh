#!/bin/bash

ACCELERATORS="accelerators.json"

if [ ! -f "$ACCELERATORS" ]; then
    echo "$ACCELERATORS file not found."
    exit 1
fi

modified_files=$(git diff master --name-only)

modified_accelerators=()

# Loop through each modified file
for file in $modified_files; do
    # Loop through each accelerator
    while read -r accelerator; do
        path=$(jq -r '.path' <<< "$accelerator")
        if [[ "$file" == "$path"* ]]; then
            accelerator_name=$(jq -r '.name' <<< "$accelerator")
            if [[ ! " ${modified_accelerators[@]} " =~ " $accelerator_name " ]]; then
                modified_accelerators+=("$accelerator_name")
            fi
        fi
    done < <(jq -c '.accelerators[]' "$ACCELERATORS")
done

echo "Modified accelerators:"
for acc in "${modified_accelerators[@]}"; do
	echo "-- $acc"
done
