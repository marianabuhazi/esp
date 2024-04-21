#!/bin/bash

passing_accelerators="$HOME/esp/utils/scripts/actions-pipeline/passing_accelerators"

in_config="$HOME/esp/socs/defconfig/esp_xilinx-vc707-xc7vx485t_defconfig"
out_config="$HOME/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/.esp_config"

old_line="TILE_1_0 = 2 empty empty 0 0 0"
new_line="TILE_1_0 = 2 acc NV_NVDLA 0 0 0  0 nvidia"
sed "s/$old_line/$new_line/" "$in_config" > "$out_config"
cd ~/esp/socs/xilinx-vc707-xc7vx485t
make esp-config
make sim

# if [ -f "$passing_accelerators" ]; then
#     echo "Passing Accelerators:"
#     while read -r accelerator; do
#         echo "ACC $accelerator"
# 		new_line="TILE_1_0 = 2 acc $accelerator 0 0 0  0 sld"
# 		sed "s/$old_line/$new_line/" "$in_config" > "$out_config"
#     done < "$passing_accelerators"
# else
#     echo "No passing accelerators found."
# fi