# `tools/socgen`

## Files

### `constants.py`
- Contains constants used by all file generators

### `generate_cache_config_svh.py`
- Generates `cache_cfg.svh` file
- Defines `BIG`/`LITTLE_ENDIAN`, `ADDR`/`BYTE`/`WORD_BITS`, `L2`/`LLC_SETS`/`WAYS`
- Run using: `python3 cache_cfg_svh.py NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_esp_global_vhd.py`
- Generates `esp_global_vhd` file
- Defines global constants for architecture parameters
- Run using: `python3 generate_esp_global_vhd.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_mmi64.py`
- Generates `mmi64_regs.h` file
- Defines various `*_NUM` and `_offset` constants and constants for individual tile
- Run using: `python3 generate_mmi64.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_power.py`
- Generates `power.h` file
- Defines `energy_weight` and `period` vectors
- Run using: `python3 generate_power.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_riscv_dts.py`
- Generates `riscv.dts` file
- Run using: `python3 generate_riscv_dts.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_S64esp.py`
- Generates `S64esp` file
- Run using: `python3 generate_S64esp.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_socmap_h.py`
- Generates `socmap.h` file
- Defines `EDCL_IP`, `BASE_FREQ`, `*_BASE_ADDR`, `RODATA_START_ADDR`, `TARGET_BYTE_ORDER` constants
- Run using: `python3 generate_socmap_h.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

### `generate_socmap_vhd.py`
- Generates `socmap.vhd` file
- Run using: `python3 generate_socmap_vhd.py $(NOC_WIDTH) $(TECHLIB) $(LINUX_MAC) $(LEON3_STACK)`

## To do
- Add descriptions for `generate_riscv_dts.py`, `generate_S64esp.py`, and `generate_socmap_h.py` in `README.md` 
