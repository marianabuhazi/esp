#!/bin/bash

cd "$HOME/esp/tech/virtex7/"
directory="acc"

accelerators=()

for dir in "$directory"/*/; do
    accelerator_name=$(basename "$dir")
    accelerators+=("$accelerator_name")
done

declare -A latest_versions

for accelerator in "${accelerators[@]}"; do
    versions=()
    for version_dir in "$directory/$accelerator"/*/; do
        version=$(basename "$version_dir")
        versions+=("$version")
    done
    
    sorted_versions=($(printf "%s\n" "${versions[@]}" | sort))
    
    latest_version="${sorted_versions[-1]}"

    latest_versions["$accelerator"]="${latest_version#*_*_}"
done

echo "Successful HLS work folders:"
for accelerator in "${!latest_versions[@]}"; do
    echo "-- $accelerator: ${latest_versions[$accelerator]}"
done