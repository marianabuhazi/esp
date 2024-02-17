#!/bin/bash

# Function to check if a directory is a submodule
is_submodule() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"

  # Get the relative path of the directory
  local rel_dir="${dir#$cwd/}"

  # Check if the directory is mentioned in the .gitmodules file
  grep -q "\[submodule \"$rel_dir\"\]" "$gitmodules"
}

# Function to descend into directories, check for submodules, and call find file functions
descend_and_check() {
  local dir="$1"
  local gitmodules="$2"
  local cwd="$3"

  # Call functions to find specific file types in the current directory
#   find_c_h_files "$dir"
	# find_cpp_files "$dir"
#   find_py_files "$dir"
	find_vhd_files "$dir"
#   find_v_files "$dir"

  for item in "$dir"/*; do
    if [[ -d "$item" ]]; then
      # Check if the directory is a submodule
      if is_submodule "$item" "$gitmodules" "$cwd"; then
        continue
      fi
      # Print the directory and recursively descend into it
      descend_and_check "$item" "$gitmodules" "$cwd"
    fi
  done
}

# Function to find .c and .h files in the current directory
# find_c_h_files() {
#   local dir="$1"
#   for file in "$dir"/*.c "$dir"/*.h; do
#     if [[ -f "$file" ]]; then
#     #   clang-format-10 -i "$file"
# 	echo "$file"
#     fi
#   done
# }

# Function to find .cpp files in the current directory
# find_cpp_files() {
#   local dir="$1"
#   for file in "$dir"/*.cpp; do
#     if [[ -f "$file" ]]; then
#     clang-format-10 -i "$file"
# 	#echo "$file"
#     fi
#   done
# }

# Function to find .py files in the current directory
# find_py_files() {
#   local dir="$1"
#   for file in "$dir"/*.py; do
#     if [[ -f "$file" ]]; then
#       echo "$(basename "$file")"
# 	  python3 -m autopep8 -i -a -a "$file"
#     fi
#   done
# }

# # Function to find .vhd files in the current directory
find_vhd_files() {
  local dir="$1"
  for file in "$dir"/*.vhd; do
    if [[ -f "$file" ]]; then
    #   echo "$(basename "$file")"
	vsg -f "$file" --fix -c ~/esp/vhdl-style-guide.yaml
    fi
  done
}

# # Function to find .v files in the current directory
# find_v_files() {
#   local dir="$1"
#   for file in "$dir"/*.v "$dir"/*.sv; do
#     if [[ -f "$file" ]]; then
# 	  verible-verilog-format --inplace --port_declarations_alignment=preserve -assignment_statement_alignment=align --indentation_spaces=4 "$file"
#     fi
#   done
# }

# Get the current working directory
cwd="$(git rev-parse --show-toplevel)"
gitmodules="$cwd/.gitmodules"

# Call the function to descend into directories and check for submodules
descend_and_check "$cwd" "$gitmodules" "$cwd"
