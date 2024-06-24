#!/bin/bash
: '
  This script accepts a parameter -t or --type which represents a file type (c, cpp, v, vhd, py.)
  The script recurses through all directories in the ESP repository and checks whether the 
  current directory is a Git submodule.
  If it is NOT a Git submodule, it further recurses into the directory and formats all files
  of the specified file type using open-source formatting tools (Clang-Format,
  Verible, VHDL-Style-Guide and AutoPep8.)

  All formatting tools should be properly installed before usage.
'

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
	figlet -c -f big FORMAT ESP
	echo -e "  Recursively format all files in the ESP repository for a given file extension.\n\n"
}

usage() {
  header
  cat<<EOF
  Usage: $0 [-t {c, cpp, vhdl, v, py}]

  Options:
  -h, --help                           Display this help message.
  -t, --type {c, cpp, vhdl, v, py}     Specify the type of files to format.
                                       Supported options: c, cpp, vhdl, v, py
									   
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

is_submodule() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"
  local rel_dir="${dir#$cwd/}"

  grep -q "\[submodule \"$rel_dir\"\]" "$gitmodules"
}

# Recursively traverse all directories and format files
descend_and_format() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"
  local ext="$4"
  local format="$5"

  find_files "$dir" "$ext" "$format"

  for item in "$dir"/*; do
    if [[ -d "$item" ]]; then
      if is_submodule "$item" "$gitmodules" "$cwd"; then
		case "$item" in
			*/rtl/caches/esp-caches|*/accelerators/stratus_hls/common/inc|*/rtl/caches/spandex-caches)
				descend_and_format "$item" "$gitmodules" "$cwd" "$ext" "$format"
				;;
			*)
				continue
				;;
			esac
        continue
      fi
      descend_and_format "$item" "$gitmodules" "$cwd" "$ext" "$format"
    fi
  done
}

# Finds files by file type (ext) and format them using formatting tool (format)
find_files() {
	local dir="$1"
	local ext="$2"
	local format="$3"
	local cond

	if [[ "$ext" == "c" ]]; then
		cond="$dir/*.$ext $dir/*.h"
	elif [[ "$ext" == "cpp" ]]; then
		cond="$dir/*.$ext $dir/*.hpp"
	else
		cond="$dir/*.$ext"
	fi

	for file in $cond; do
		local out
		if [[ -f "$file" ]]; then
			echo -n -e "Formatting $(basename "$file")...\n"
			echo -e "$format $file"
			out=$("$format" "$file" 2>&1)
			if [ ! $? -eq 0 ]; then
				echo -e "${ERROR}: $out\n"
				return 1
			else
				echo -e "${SUCCESS}"
				return 0
			fi
		fi
	done
}

# Get cwd and gitmodules
cwd="$(git rev-parse --show-toplevel)"
gitmodules="$cwd/.gitmodules"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
  case "$1" in
    -t | --type )
      shift
      case "$1" in
        c ) ext="c"
			format="clang-format-10 -i" 
			;;
        cpp ) ext="cpp"
			  format="clang-format-10 -i"
			  ;;
        vhdl ) ext="vhd"
			   format="vsg --fix -c ~/esp/vhdl-style-guide.yaml -of summary -f"
			   ;;
        v ) ext="v" 
		    format="verible-verilog-format --inplace --port_declarations_alignment=preserve-assignment_statement_alignment=align --indentation_spaces=4"
			;;
        py ) ext="py"
			 format="python3 -m autopep8 -i -a -a"
			;;
        * )
		  usage
		  echo -e "${ERROR}: Invalid formatting option. Valid options include: c, cpp, vhdl, v, py.\n"
          exit 1
          ;;
      esac
      ;;
    -h | --h | --help )
      usage
      exit 0
      ;;
    * )
      usage
	  echo -e "${ERROR}: Invalid option. Use -t {c, cpp, vhdl, v, py}.\n"
      exit 1
      ;;
  esac
  shift
done

# Check if file type was specified
if [ -z "$ext" ]; then
  usage
  echo -e "${ERROR}: Select a file type to format. Use -t {c, cpp, vhdl, v, py}.\n"
  exit 1
fi

header
check_tools
descend_and_format "$cwd" "$gitmodules" "$cwd" "$ext" "$format"