#!/bin/bash
: '
  This script accepts a -f or -c flag to indicate "formatting" or "checking."
  The flag selected should be followed by a filename.
  Alternatively, -f could be replaced with -fa, and -c with -ca to indicate
  "format all" or "check all."

  The script determines which files have been modified and applies the appropriate
  open-source formatting tool (Clang-Format, Verible, VHDL-Style-Guide and AutoPep8.)

  All formatting tools should be properly installed before usage.
'

root_dir=$(git rev-parse --show-toplevel)
is_github_actions=false

# Output styles
NC='\033[0m' 
BOLD='\033[1m'
GREEN='\033[32m'
RED='\033[31m'
BLUE='\033[34m'
ERROR=${RED}${BOLD}ERROR${NC}
SUCCESS=${GREEN}${BOLD}SUCCESS${NC}
INFO=${BLUE}${BOLD}INFO${NC}

header() {
	echo ""
	figlet -c -f big FORMAT MODIFIED
	echo -e "  Report formatting violations, or format modified files in-place.\n\n"
}

usage() {
  header
  cat<<EOF
  Usage: $0

  Options:
  -h            Display this help message
  -f <file>     Fix formatting for file <file>
  -a            Apply to all
  -g            Run as Github Actions workflow or pre-push hook

  Examples:
  $0 -fa                    # Fix all modified files in-place
  $0 -f myfile.py           # Fix myfile.py in-place
  $0 -ca                    # Report violations for all modified files
  $0 -c myfile.py           # Report violations for myfile.py
  $0 -g -ca                 # Report violations as part of a workflow or hook

EOF
}

check_tools() {
  local tools=("clang-format-10" "vsg" "verible-verilog-format" "autopep8" "figlet")
  for tool in "${tools[@]}"; do
    if ! command -v $tool &> /dev/null; then
      echo -e "${ERROR}: This script depends on the proper installation of $tool.\nPlease install it, and try again.\n"
      exit 1
    fi
  done
}

# Determine what formatting tool to use, and add appropriate flags
format_file() {
    local file_to_format="$1"
	local action="$2"
    local type="${file_to_format##*.}"

	case "$action" in
		format)
			case "$type" in
				c | h | cpp | hpp)
					clang_format_flags="-i" ;;
				py)
					autopep8_flags="-i" ;;
				sv | v) 
					verible_flags="--inplace" ;;
				vhd)
					vsg_flags="--fix" ;;
			esac
			;;
		check)
			case "$type" in
				c | h | cpp | hpp)
					clang_format_flags="--dry-run" ;;
				py)
					autopep8_flags="--list-fixes" ;;
				sv | v) 
					verible_flags="--verify";;
				vhd)
					;;
			esac
			;;
		*)
			usage
			echo -e "${ERROR}: Invalid action." >&2
			;;
	esac

    local out
    case "$type" in
        c | h | cpp | hpp)
            out=$(clang-format-10 $clang_format_flags "$file_to_format" 2>&1);;
        py)
            out=$(python3 -m autopep8 $autopep8_flags -a -a "$file_to_format" 2>&1);;
        sv | v) 
            out=$(verible-verilog-format $verible_flags --port_declarations_alignment=preserve -assignment_statement_alignment=align --indentation_spaces=4 "$file_to_format" 2>&1) ;;
        vhd)
            out=$(vsg -f "$file_to_format" $vsg_flags -c ~/esp/vhdl-style-guide.yaml 2>&1) ;;
    esac

	if [ $? -eq 0 ] && ! echo "$out" | grep -qi "warning"; then
    	echo -e "${SUCCESS}\n"
    	return 0
	else
		echo -e "${ERROR} $out\n"
		return 1
	fi
}

# Determine whether to format in-place or check for violations
while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        -f)
            if [[ -z $2 || $2 == -* ]]; then
				usage
                echo -e "${ERROR}: Option -f requires an argument. Provide a filename, or specify -fa to format all.\n" >&2
            fi
            action="format"
            file_to_format="$2"
            shift
            ;;
        -fa)
            action="format"
            all_files=true
            ;;
        -c)
            if [[ -z $2 || $2 == -* ]]; then
                usage
                echo -e "${ERROR}: Option -c requires an argument. Provide a filename, or specify -ca to check all.\n" >&2
            fi
            action="check"
            file_to_format="$2"
            shift
            ;;
        -ca)
            action="check"
            all_files=true
            ;;
		-g)
            is_github_actions=true
            ;;
        -h|--help)
            usage
            ;;
        *)
			usage
            echo -e "${ERROR}: Invalid option: $1.\n" >&2
            ;;
    esac
    shift
done

# User must specify a file to format if the -f flag is called
if [ -n "$file_to_format" ]; then
	if [ ! -f "$file_to_format" ]; then
        echo -e "$0: ${ERROR}: File '$file_to_format' not found." >&2
        exit 1
    fi
	echo -n "Formatting/checking $file_to_format..."
    if format_file "$file_to_format" "$action"; then
		echo -e "${SUCCESS}: Action completed successfully."
		exit 0
	else
		echo -e "${ERROR}: Action did not complete successfully."
		exit 1
	fi

	exit 0
fi


# Check all modified files of the file types: C/C++, Python, Verilog/SystemVerilog, VHDL
if [ "$all_files" = true ]; then
    modified_files=$(git status --porcelain | grep -E '^ M|^??' | awk '$2 ~ /\.(c|h|cpp|hpp|py|v|sv|vhd)$/ {print $2}')
	if [ "$is_github_actions" = true ]; then
		modified_files=$(git diff --name-only HEAD^..HEAD | grep -E '\.(c|h|cpp|hpp|py|v|sv|vhd)$')
	fi
    if [ -z "$modified_files" ]; then
        echo -e "🔍 No modified files found."
        exit 0
    fi

    modified_count=$(echo "$modified_files" | wc -l)
    echo -e "🔍 Found $modified_count modified files:\n"
    for file in $modified_files; do
        echo "   - $file"
    done
    echo ""
    modified_files=$(echo "$modified_files" | sed "s|^|$root_dir/|")

    error_files=""
    success_files=""

	# Format/check each file in place with the appropriate tool
    for file in $modified_files; do
		echo -e -n "${INFO}: Formatting/checking $(basename "$file")..."
		if ! format_file "$file" "$action"; then
			error_files="$error_files $file"
		fi
	done

	echo ""
	if [ -n "$error_files" ]; then
		echo -e "${ERROR}: Action did not complete successfully."
		exit 1
	else
		echo -e "${SUCCESS}: Action completed successfully."
		exit 0
	fi
else
    usage
fi