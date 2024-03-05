#!/bin/bash

root_dir=$(git rev-parse --show-toplevel)

usage() {
    echo "Usage: $0 [-f <file>]"
    echo "  -f <file>: Specify a file to see all formatting errors"
    exit 1
}

# Parse command line options
while getopts ":f:" opt; do
    case ${opt} in
        f)
            file_to_check=$OPTARG
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            usage
            ;;
        :)
            echo "Option -$OPTARG requires an argument." >&2
            usage
            ;;
    esac
done
shift $((OPTIND -1))

if [ -n "$file_to_check" ]; then
    if [ ! -f "$file_to_check" ]; then
        echo "Error: File '$file_to_check' not found."
        exit 1
    fi
    echo "Checking formatting for $file_to_check..."
    clang-format-10 --dry-run "$file_to_check" || exit 1
    exit 0
fi

# Check if there are modified files
modified_files=$(git status --porcelain | grep -E '^ M|^??' | awk '$2 ~ /\.(c|h|cpp|hpp|py|v|sv|vhd)$/ {print $2}')

if [ -z "$modified_files" ]; then
    echo -e "No modified files found."
    exit 0
fi

# Count and print modified files
modified_count=$(echo "$modified_files" | wc -l)
echo -e "\U0001F50E Found $modified_count modified files:"
echo ""
for file in $modified_files; do
    echo "   - $file"
done
echo ""
modified_files=$(echo "$modified_files" | sed "s|^|$root_dir/|")

echo "Checking formatting..."
echo ""

# Store files that need formatting changes
changes_required_files=""

for file in $modified_files; do
    type="${file##*.}"

    # Check formatting of each file
    case "$type" in
        c | h | cpp | hpp)
            output=$(clang-format-10 --dry-run "$file" 2>&1)
            warnings_errors=$(echo "$output" | grep -E 'error:' | wc -l)
            if [ "$warnings_errors" -gt 0 ]; then
                echo -e "\033[31m$warnings_errors formatting errors found in $(basename "$file").\033[0m"
                echo ""
                echo "Run the script with '-f $file' to see all errors for this file."
                echo "Example: ./check_format.sh -f $file"
                echo ""
            fi
            ;;
        *)
            echo "Skipping formatting check for $file (unsupported extension)." ;;
    esac
done

echo ""
echo -e "\U00002728 Formatting check completed!"
