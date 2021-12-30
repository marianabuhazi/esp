#!/bin/bash

e=".esp_config.bak.$1"
rm -f ../.esp_config.bak.*
cp $e ../.esp_config.bak.1
cp $e ../socgen/esp/.esp_config
cp $e ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/.esp_config
cp $e ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/.esp_config.bak
