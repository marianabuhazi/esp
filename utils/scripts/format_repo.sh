#!/bin/bash

	# # # # # # # # # # # # # # # # # # # # # # # # # # # # 
	#   This program recurses through all directories in  #
	#   the /esp repository and formats all files using   #
	#   clang-format-10, autopep8, verible, or vsg.       #
	# # # # # # # # # # # # # # # # # # # # # # # # # # # #

# Display usage instructions
display_usage() {
  echo "Recursively format all files in the /esp repository. 🛠️ ✨"
  echo ""
  echo "Usage: $0 [-t {c, cpp, vhdl, v, py}]"
  echo "Options:"
  echo "  -t, --type {c, cpp, vhdl, v, py}  Specify the type of files to format."
  echo "                                    Supported options: c, cpp, vhdl, v, py"
  echo "  -h, --help                         Display this help message."
}

# Check if a directory is a submodule
is_submodule() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"
  local rel_dir="${dir#$cwd/}"

  # Compare to .gitmodules
  grep -q "\[submodule \"$rel_dir\"\]" "$gitmodules"
}

# Recursively traverse all directory levels and format C/C++ files
descend_and_format() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"
  local format_style="$4"

  "$format_style" "$dir"

  # Traverse all directories under /esp
  for item in "$dir"/*; do
    if [[ -d "$item" ]]; then
      # If directory is a submodule, skip unless its sld-owned, then recurse
      if is_submodule "$item" "$gitmodules" "$cwd"; then
		case "$item" in
			*/rtl/caches/esp-caches|*/accelerators/stratus_hls/common/inc|*/rtl/caches/spandex-caches)
				descend_and_format "$item" "$gitmodules" "$cwd" "$format_style"
				;;
			*)
				continue
				;;
			esac
        continue
      fi
      descend_and_format "$item" "$gitmodules" "$cwd" "$format_style"
    fi
  done
}

# Find all .c and .h files in the current directory
find_c_h_files() {
  local dir="$1"
  for file in "$dir"/*.c "$dir"/*.h; do
    if [[ -f "$file" ]]; then
      clang-format-10 -i "$file" > /dev/null 2>&1
    fi
  done
}

# Find all .cpp and .hpp files in the current directory
find_cpp_hpp_files() {
  local dir="$1"
  for file in "$dir"/*.cpp "$dir"/*.hpp; do
    if [[ -f "$file" ]]; then
      clang-format-10 -i "$file" > /dev/null 2>&1
    fi
  done
}

# Find all .py files in the current directory
find_py_files() {
  local dir="$1"
  for file in "$dir"/*.py; do
    if [[ -f "$file" ]]; then
	  python3 -m autopep8 -i -a -a "$file" > /dev/null 2>&1
    fi
  done
}

# Find .v files in the current directory
find_v_files() {
  local dir="$1"
  for file in "$dir"/*.v "$dir"/*.sv; do
    if [[ -f "$file" ]]; then
 	  verible-verilog-format --inplace --port_declarations_alignment=preserve -assignment_statement_alignment=align --indentation_spaces=4 "$file" > /dev/null 2>&1
	fi
  done
}

# Function to find .vhd files in the current directory
find_vhd_files() {
  local dir="$1"
  for file in "$dir"/*.vhd; do
    if [[ -f "$file" ]]; then
	  vsg -f "$file" --fix -c ~/esp/vhdl-style-guide.yaml -of summary > /dev/null 2>&1
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
        c ) format_style="find_c_h_files" 
			echo "Starting formatting for C files ..."
			echo -e "This should be quick! Approx: 5 mins \U0001F680";;
        cpp ) format_style="find_cpp_hpp_files"
			  echo "Starting formatting for C++ files ..."
			  echo -e "This should be quick! Approx: 5 mins \U0001F680";;
        vhdl ) format_style="find_vhd_files"
			   echo "Starting formatting for VHDL files ..."
			   echo -e "This may take a while, be patient! Approx: 30-40 mins \U0001F691";;
        v ) format_style="find_v_files" 
			echo "Starting formatting for Verilog/SystemVerilog files ..."
			echo -e "This may take a while, be patient. Approx: 15 mins \U0001F691";;
        py ) format_style="find_py_files" 
			 echo "Starting formatting for Python files ..."
			 echo -e "This should be quick! Approx: 5 mins \U0001F680";;
        * )
          echo "Invalid formatting option. Valid options include: c, cpp, vhdl, v, py"
          exit 1
          ;;
      esac
      ;;
    -h | --h | --help )
      display_usage
      exit 0
      ;;
    * )
      echo "Invalid option: $1"
      display_usage
      exit 1
      ;;
  esac
  shift
done

# Check if format_style is set
if [ -z "$format_style" ]; then
  echo "Error: Missing formatting style. Use -t {c, cpp, vhdl, v, py}."
  exit 1
fi

descend_and_format "$cwd" "$gitmodules" "$cwd" "$format_style"
echo ""
echo ""
echo -e "\xE2\x9C\xa8 Done formatting!"