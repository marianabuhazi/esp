#!/bin/bash

root_dir=$(git rev-parse --show-toplevel)

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

echo "Formatting..."
echo ""


for file in $modified_files; do
    type="${file##*.}"

    # Call appropriate formatting tool based on file extension
    case "$type" in
        c | h | cpp | hpp)
            clang-format-10 -i "$file" > /dev/null 2>&1;;
        py)
            python3 -m autopep8 -i -a -a "$file" > /dev/null 2>&1;;
		v) 
			verible-verilog-format --inplace --port_declarations_alignment=preserve -assignment_statement_alignment=align --indentation_spaces=4 "$file" > /dev/null 2>&1;;
        vhdl)
            vsg -f "$file" --fix -c ~/esp/vhdl-style-guide.yaml -of summary > /dev/null 2>&1;;
        *)
            echo "Skipping formatting for $file (unsupported extension)." ;;
    esac
done 

echo -e "\U00002728 Done formatting!"
echo "The files have been modified in place."
