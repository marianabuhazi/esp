#!/bin/bash

root_dir=$(git rev-parse --show-toplevel)

usage() {
    echo "ESP modified file formatter ✨🛠️"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo "  -h, --help         Display this help message"
    echo "  -a, --all          Format all modified files tracked by Git"
    echo "  -f <file>          Specify a file to format"
    echo ""
    echo "Example:"
    echo "  $0 -a                        # Format all modified files"
    echo "  $0 -f myfile.py              # Format a specific file"
    exit 1
}

format_file() {
    local file_to_format="$1"
    local type="${file_to_format##*.}"

    local output
    case "$type" in
        c | h | cpp | hpp)
            output=$(clang-format-10 -i "$file_to_format" 2>&1) ;;
        py)
            output=$(python3 -m autopep8 -i -a -a "$file_to_format" 2>&1) ;;
        sv | v) 
            output=$(verible-verilog-format --inplace --port_declarations_alignment=preserve -assignment_statement_alignment=align --indentation_spaces=4 "$file_to_format" 2>&1) ;;
        vhd)
            output=$(vsg -f "$file_to_format" --fix -c ~/esp/vhdl-style-guide.yaml 2>&1) ;;
    esac

    if [ $? -eq 0 ]; then
		echo -e " \033[32mSUCCESS\033[0m"
		echo ""
        return 0
    else
		echo -e " \033[31mFAILED\033[0m"
        echo -e "  $output"
		echo ""
        return 1
    fi
}

while getopts ":haf:" opt; do
    case ${opt} in
        h)
            usage
            ;;
        a)
            all_files=true
            ;;
        f)
            file_to_format=$OPTARG
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

if [ -n "$file_to_format" ]; then
	if [ ! -f "$file_to_format" ]; then
        echo "$0: Error: File '$file_to_format' not found." >&2
        exit 1
    fi

    echo -n "Formatting $file_to_format..."
    if format_file "$file_to_format"; then
		echo -e "\U00002728 Done formatting!"
	else
		echo -e "\u274C Formatting failed!"
	fi

	exit 0
fi


if [ "$all_files" = true ]; then
    modified_files=$(git status --porcelain | grep -E '^ M|^??' | awk '$2 ~ /\.(c|h|cpp|hpp|py|v|sv|vhd)$/ {print $2}')

    if [ -z "$modified_files" ]; then
        echo -e "No modified files found."
        exit 0
    fi

    modified_count=$(echo "$modified_files" | wc -l)
    echo -e "\U0001F50E Found $modified_count modified files:"
    echo ""
    for file in $modified_files; do
        echo "   - $file"
    done
    echo ""
    modified_files=$(echo "$modified_files" | sed "s|^|$root_dir/|")

    error_files=""
    success_files=""

    for file in $modified_files; do
		echo -n "Formatting $(basename "$file")..."
		if ! format_file "$file"; then
			error_files="$error_files $file"
		fi
	done


	echo ""
	if [ -n "$error_files" ]; then
		echo -e "\u274C Formatting failed!"
	else
		echo -e "\U00002728 Done formatting!"
	fi
else
    usage
fi
