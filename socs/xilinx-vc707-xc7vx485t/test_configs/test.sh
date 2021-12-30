#!/bin/bash

echo "cache_cfg.svh"
diff ../socgen/esp/cache_cfg.svh ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/cache_cfg.svh
echo "esp_global.vhd"
diff ../socgen/esp/esp_global.vhd ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/esp_global.vhd
echo "mmi64_regs.h"
diff ../socgen/esp/mmi64_regs.h ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/mmi64_regs.h
echo "power.h"
diff ../socgen/esp/power.h ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/power.h
echo "riscv.dts"
diff ../socgen/esp/riscv.dts ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/riscv.dts
echo "S64esp"
diff ../socgen/esp/S64esp ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/S64esp
echo "socmap.h"
diff ../socgen/esp/socmap.h ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/socmap.h
echo "socmap.vhd"
diff ../socgen/esp/socmap.vhd ~/espcryo-ai/socs/xilinx-vc707-xc7vx485t/socgen/esp/socmap.vhd

echo "done"
