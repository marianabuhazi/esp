#!/bin/bash

# Usage: test_config.sh [options] <soc> <n>
# options:
# -h      show help
# -v      verbose mode

# setup
# set repo to file path from home
repo="espcryo-ai"

# flags
VERBOSE=0
while getopts "hv" flag; do
    case ${flag} in
        h) # help
            echo "Usage: $0 [options] <soc> <n>" >&2
            echo "options:" >&2
            echo "-h      show help" >&2
            echo "-v      verbose mode" >&2
            exit;;
        v) # verbose
            VERBOSE=1;;
        *) # invalid option
            echo "Unexpected option ${flag}" >&2
            exit;;
    esac
done
readonly VERBOSE

# number of arguments
if [[ $(($# - ${VERBOSE})) -ne 2 ]]; then
    echo "Usage: $0 [options] <soc> <n>" >&2
    echo "options:" >&2
    echo "-h      show help" >&2
    echo "-v      verbose mode" >&2
    exit
fi

# paths of /socs/xilinx-vc707-xc7vx485t/ for both repos
soc=$1
n=$2
if [[ ${VERBOSE} == 1 ]]; then
    soc=$2
    n=$3
fi
export esp_path_1="../../../socs/${soc}/" # relative path for current dir
export esp_path_2="${HOME}/${repo}/socs/${soc}/"
esp_config=".esp_config.bak.${n}"

#check if esp_path_* are directories that exist
if [[ ! -d ${esp_path_1} ]]; then
    echo "Relative path [${esp_path_1}] does not exist" >&2
    exit
fi

if [[ ! -d ${esp_path_2} ]]; then
    echo "Absolute path [${esp_path_2}] does not exist" >&2
    exit
fi

if [[ ! -a ${esp_config} ]]; then
    echo "Configuration file [${esp_config}] does not exist" >&2
    exit
fi

# setup .esp_config
rm -f "${esp_path_1}/.esp_config.bak.*"
rm -f "${esp_path_2}/socgen/esp/*"
# check what is necessary and what is going on in cryo-ai
cp "${esp_config}" "${esp_path_1}/.esp_config.bak.1"
cp "${esp_config}" "${esp_path_1}/socgen/esp/.esp_config"
cp "${esp_config}" "${esp_path_2}/.esp_config"
cp "${esp_config}" "${esp_path_2}/.esp_config.bak"
cp "${esp_config}" "${esp_path_2}/socgen/esp/.esp_config"
cp "${esp_config}" "${esp_path_2}/socgen/esp/.esp_config.bak"

echo "Running on ${esp_config}"

# generate files in esp_path_1
if [[ -z "${DISPLAY}" ]]; then
    # bash -c 'cd "${esp_path_1}" && DISPLAY=:50.0 make newsocmap-run'
    bash -c 'cd "${esp_path_1}" && DISPLAY=:50.0 make newsocmap-run &> /dev/null'
else
    # bash -c 'cd "${esp_path_1}" && make newsocmap-run'
    bash -c 'cd "${esp_path_1}" && make newsocmap-run &> /dev/null'
fi

# generate files in esp_path_2
# bash -c 'cd "${esp_path_2}" && make esp-config'
bash -c 'cd "${esp_path_2}" && make esp-config &> /dev/null'

# diff

# cache_cfg.svh
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/cache_cfg.svh ${esp_path_2}/socgen/esp/cache_cfg.svh
else
    echo "---------------"
    echo "cache_cfg.svh"
    diff ${esp_path_1}/socgen/esp/cache_cfg.svh ${esp_path_2}/socgen/esp/cache_cfg.svh
    echo "---------------"
fi

# esp_global.vhd
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/esp_global.vhd ${esp_path_2}/socgen/esp/esp_global.vhd
else
    echo "esp_global.vhd"
    diff ${esp_path_1}/socgen/esp/esp_global.vhd ${esp_path_2}/socgen/esp/esp_global.vhd
    echo "---------------"
fi

# mmi64_regs.h
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/mmi64_regs.h ${esp_path_2}/socgen/esp/mmi64_regs.h
else
    echo "mmi64_regs.h"
    diff ${esp_path_1}/socgen/esp/mmi64_regs.h ${esp_path_2}/socgen/esp/mmi64_regs.h
    echo "---------------"
fi

# power.h
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/power.h ${esp_path_2}/socgen/esp/power.h
    # check if leon3
    if [[ $(grep "leon3" ${esp_config} | wc -l) == 0 ]]; then
        if [[ $(diff ${esp_path_1}/socgen/esp/riscv.dts ${esp_path_2}/socgen/esp/riscv.dts | wc -l) > 4 ]]; then
            # The output of `diff` is ignored when there are at most 4 lines, as the random mac address
            # produces 4 lines of output
            diff -q ${esp_path_1}/socgen/esp/riscv.dts ${esp_path_2}/socgen/esp/riscv.dts
        fi
    fi
else
    echo "power.h"
    diff ${esp_path_1}/socgen/esp/power.h ${esp_path_2}/socgen/esp/power.h
    # check if leon3
    if [[ $(grep "leon3" ${esp_config} | wc -l) == 0 ]]; then
        echo "---------------"
        echo "riscv.dts"
        if [[ $(diff ${esp_path_1}/socgen/esp/riscv.dts ${esp_path_2}/socgen/esp/riscv.dts | wc -l) > 4 ]]; then
            # The output of `diff` is ignored when there are at most 4 lines, as the random mac address
            # produces 4 lines of output
            diff ${esp_path_1}/socgen/esp/riscv.dts ${esp_path_2}/socgen/esp/riscv.dts
        fi
    fi
    echo "---------------"
fi

# S64esp
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/S64esp ${esp_path_2}/socgen/esp/S64esp
else
    echo "S64esp"
    diff ${esp_path_1}/socgen/esp/S64esp ${esp_path_2}/socgen/esp/S64esp
    echo "---------------"
fi

# socmap.h
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/socmap.h ${esp_path_2}/socgen/esp/socmap.h
else
    echo "socmap.h"
    diff ${esp_path_1}/socgen/esp/socmap.h ${esp_path_2}/socgen/esp/socmap.h
    echo "---------------"
fi

# socmap.vhd
if [[ ${VERBOSE} == 0 ]]; then
    diff -q ${esp_path_1}/socgen/esp/socmap.vhd ${esp_path_2}/socgen/esp/socmap.vhd
else
    echo "socmap.vhd"
    diff ${esp_path_1}/socgen/esp/socmap.vhd ${esp_path_2}/socgen/esp/socmap.vhd
    echo "---------------"
fi
