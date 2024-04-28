#!/bin/bash

source "get_hls_accelerators.sh"
defconfig="$HOME/esp/socs/defconfig/esp_xilinx-vc707-xc7vx485t_defconfig"
fpga_program="$HOME/esp/utils/scripts/actions-pipeline/fpga_program.log"

for accelerator in "${!latest_versions[@]}"; do
    accelerator_upper=$(echo "$accelerator" | tr '[:lower:]' '[:upper:]')
    esp_config="$HOME/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/.esp_config"
	vivado_syn="$HOME/esp/utils/scripts/actions-pipeline/vivado_syn_$accelerator.log"
    cp "$defconfig" "$esp_config"
	
    sed -i "s/CONFIG_DSU_IP = C0A80107/CONGIG_DSU_IP = C0A8011C/" "$esp_config"
    sed -i "s/CONFIG_DSU_ETH = A6A7A0F8043D/CONGIG_DSU_ETH = A6A7A0F80445/" "$esp_config"
    sed -i "s/TILE_1_0 = 2 empty empty 0 0 0/TILE_1_0 = 2 acc $accelerator_upper 0 0 0 ${latest_versions[$accelerator]} 0 sld/" "$esp_config"
    sed -i "s/POWER_1_0 = empty 0 0 0 0 0 0 0 0 0 0 0 0/POWER_1_0 = $accelerator_upper 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0/" "$esp_config"
	
	cd "$HOME/esp/socs/xilinx-vc707-xc7vx485t"
	rm top.bit
	rm -r vivado
	make clean
	cat $esp_config
	make esp-config
	make vivado-syn &> "$vivado_syn"
	if [ -s "top.bit" ]; then
		make soft
		make fpga-program &> "$fpga_program"
		if grep -q ERROR "$fpga_program"; then
        	echo "FPGA PROGRAM FAILED: $accelerator"
		else
			echo "FPGA PROGRAM SUCCESS: $accelerator"
			
		fi
		rm "$fpga_program"
	else
		echo "Bitstream generation failed: $accelerator"
	fi
    # Clean up
    # fi
	# delete vivado folder and bit file
	# make clean
	# if (top.bit exists, make vivado-syn)
	# make fpga-program
	# make uart
done
