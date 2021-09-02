# Socgen

This directory contains tools for configuring and generating an ESP SoC. The directory also contains the various file generators for `.esp_config` and other files derived from it. 

To run the GUI, enter the directory of an FGPA board (ex: `esp/socs/xilinx-vc707-xc7vx485t/`) and run `$ make esp-xconfig`.
To run any `generate_<file>.py` generators, run `$ python3 generate_esp_global_vhd.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`.

This directory is organized as follows:
* `esp_creator.py` configures and generates an ESP SoC in GUI mode
* `esp_creator_batch.py` configures and generates an ESP SoC in batch mode
* `soc.py` contains the definition and structure of an ESP SoC, which is then configured by `esp_creator*.py` files
* `NoCConfiguration.py` contains most of the logic and setup functions relating to the NoC/tile floorplanning, which are used by `esp_creator*.py` and `soc.py` files 
* `constants.py` contains definitions of constants used in `generate_S64esp.py`, `generate_socmap_h.py`, and `generate_socmap_vhd.py` files
* `generate_cache_config_svh.py` which generates the `cache_cfg.svh` file. The `cache_cfg.svh` file defines `BIG`/`LITTLE_ENDIAN`, `ADDR`/`BYTE`/`WORD_BITS`, `L2`/`LLC_SETS`/`WAYS`.
* `generate_esp_global_vhd.py` which generates the `esp_global.vhd` file. The `esp_global.vhd` file defines global constants for architecture parameters.
* `generate_mmi64.py` which generates the `mmi64_regs.h` file. The `mmi64_regs.h` file defines various `*_NUM` and `_offset` constants and constants for individual tile.
* `generate_power.py` which generates the `power.h` file. The `power.h` file defines `energy_weight` and `period` vectors.
* `generate_riscv_dts.py` which generates the `riscv.dts` file.
* `generate_S64esp.py` which generates `S64esp` file.
* `generate_socmap_h.py` which generates `socmap.h` file. The `socmap.h` file defines `EDCL_IP`, `BASE_FREQ`, `*_BASE_ADDR`, `RODATA_START_ADDR`, `TARGET_BYTE_ORDER` constants.
* `generate_socmap_vhd.py` which generates `socmap.vhd` file.

The overall flow is described below:

![socgen flow](img/socgen_flow.png "socgen flow")

First, `esp_creator.py` creates the `.esp_config*` files, which are then used by the file generators. For each file generator in the named `generate_<file>.py`, the file `<file>` is created.

## To do
- The files that are generated in this `qt-gui2` branch must be checked to match the files generated from the `cryo-ai` branch
  - To test if files match up, use the GUIs in both branches to generate identical SoCs and their respective `.esp_config` files. Once the eight different files are generated, use `$ diff <file_qt-gui2> <file_cryo-ai>` to check if there are any differences.
  - It is known that the following files are not generated in the same manner:
    - `esp_global.vhd`
    - `riscv.dts`
    - `socmap.vhd`
  - The other five files may or may not be generated in the same manner - more testing (with different accelerators and more complex configurations) is needed
  - Additionally, the code that generates the files in the `cryo-ai` branch can be found in `esp/tools/socgen/socmap_gen.py`
