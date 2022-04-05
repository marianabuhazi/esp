#!/bin/bash

# Usage: batch_test.sh <soc>

# setup
# set repo to file path from home
repo="espcryo-ai"

# flags
while getopts "h" flag; do
    case ${flag} in
        h) # help
            echo "Usage: $0 <soc>" >&2
            exit;;
        *) # invalid option
            echo "Unexpected option ${flag}" >&2
            exit;;
    esac
done

# number of arguments
if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <soc>" >&2
    exit
fi

# paths of /socs/xilinx-vc707-xc7vx485t/ for both repos
soc=$1
esp_path_1="../../../socs/${soc}/" # relative path for current dir
esp_path_2="${HOME}/${repo}/socs/${soc}/"

# create tmp file
file="batch-test-tmp.txt"
rm -f ${file}
touch ${file}
echo "n|cache_cfg.svh|esp_global.vhd|mmi64_regs.h|power.h|riscv.dts|S64esp|socmap.h|socmap.vhd" >> ${file}

# get max .esp_config.bak.n value
n=$(ls -lav | grep ".esp_config.bak." | tail -1 | rev | cut -d. -f1 | rev)

# main loop
for i in $(seq 1 ${n}) ; do
    file_i="batch-test-tmp-${i}.txt"
    rm -f ${file_i}
    echo -n "${i}|" >> ${file}

    # diff
    ./test_config.sh ${soc} ${i} &> ${file_i}
    cat ${file_i}

    # grep

    # cache_cfg.svh
    if [[ $(grep "cache_cfg.svh" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # esp_global.vhd
    if [[ $(grep "esp_global.vhd" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # mmi64_regs.h
    if [[ $(grep "mmi64_regs.h" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # power.h
    if [[ $(grep "power.h" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # riscv.dts
    if [[ $(grep "riscv.dts" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # S64esp
    if [[ $(grep "S64esp" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # socmap.h
    if [[ $(grep "socmap.h" ${file_i} | wc -l) == 0 ]]; then
        echo -n ".|" >> ${file}
    else
        echo -n "X|" >> ${file}
    fi

    # socmap.vhd
    if [[ $(grep "socmap.vhd" ${file_i} | wc -l) == 0 ]]; then
        echo "." >> ${file}
    else
        echo "X" >> ${file}
    fi
    rm -f ${file_i}
done

echo "'X' indicates a difference between the files, '.' indicates no difference"
echo ""
column ${file} -t -s "|"
rm ${file}
