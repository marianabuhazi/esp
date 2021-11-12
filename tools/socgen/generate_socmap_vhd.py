
#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import sys
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *
from constants import *

def print_header(fp, package):
  fp.write("-- Copyright (c) 2011-2021 Columbia University, System Level Design Group\n")
  fp.write("-- SPDX-License-Identifier: Apache-2.0\n\n")

  fp.write("------------------------------------------------------------------------------\n")
  fp.write("--  This file is a configuration file for the ESP NoC-based architecture\n")
  fp.write("-----------------------------------------------------------------------------\n")
  fp.write("-- Package:     " + package + "\n")
  fp.write("-- File:        " + package + ".vhd\n")
  fp.write("-- Author:      Paolo Mantovani - SLD @ Columbia University\n")
  fp.write("-- Author:      Christian Pilato - SLD @ Columbia University\n")
  fp.write("-- Description: System address mapping and NoC tiles configuration\n")
  fp.write("------------------------------------------------------------------------------\n\n")

def print_libs(fp, std_only):
  fp.write("library ieee;\n")
  fp.write("use ieee.std_logic_1164.all;\n")
  fp.write("use ieee.numeric_std.all;\n")

  if not std_only:
    fp.write("use work.esp_global.all;\n")
    fp.write("use work.stdlib.all;\n")
    fp.write("use work.grlib_config.all;\n")
    fp.write("use work.gencomp.all;\n")
    fp.write("use work.amba.all;\n")
    fp.write("use work.sld_devices.all;\n")
    fp.write("use work.devices.all;\n")
    fp.write("use work.misc.all;\n")
    fp.write("use work.leon3.all;\n")
    fp.write("use work.nocpackage.all;\n")
    fp.write("use work.cachepackage.all;\n")
    fp.write("use work.allcaches.all;\n")

  fp.write("\n")

def print_mapping(fp, soc, esp_config):

  fp.write("  constant CFG_FABTECH : integer := " + soc.TECH  + ";\n\n")

  #
  fp.write("  ------ Maximum number of slaves on both HP bus and I/O-bus\n")
  fp.write("  constant maxahbm : integer := NAHBMST;\n")
  fp.write("  constant maxahbs : integer := NAHBSLV;\n\n")

  #
  fp.write("  -- Arrays of Plug&Play info\n")
  fp.write("  subtype apb_l2_pconfig_vector is apb_config_vector_type(0 to CFG_NL2-1);\n")
  fp.write("  subtype apb_llc_pconfig_vector is apb_config_vector_type(0 to CFG_NLLC-1);\n")

  #
  fp.write("  -- Array of design-point or implementation IDs\n")
  fp.write("  type tile_hlscfg_array is array (0 to CFG_TILES_NUM - 1) of hlscfg_t;\n")

  #
  fp.write("  -- Array of attributes for clock regions\n")
  fp.write("  type domain_type_array is array (0 to " + str(esp_config.ndomain - 1) + ") of integer;\n")

  #
  fp.write("  -- Array of device IDs\n")
  fp.write("  type tile_device_array is array (0 to CFG_TILES_NUM - 1) of devid_t;\n")

  #
  fp.write("  -- Array of I/O-bus indices\n")
  fp.write("  type tile_idx_array is array (0 to CFG_TILES_NUM - 1) of integer range 0 to NAPBSLV - 1;\n")

  #
  fp.write("  -- Array of attributes for I/O-bus slave devices\n")
  fp.write("  type apb_attribute_array is array (0 to NAPBSLV - 1) of integer;\n")

  #
  fp.write("  -- Array of IRQ line numbers\n")
  fp.write("  type tile_irq_array is array (0 to CFG_TILES_NUM - 1) of integer range 0 to NAHBIRQ - 1;\n")

  #
  fp.write("  -- Array of 12-bit addresses\n")
  fp.write("  type tile_addr_array is array (0 to CFG_TILES_NUM - 1) of integer range 0 to 4095;\n")

  #
  fp.write("  -- Array of flags\n")
  fp.write("  type tile_flag_array is array (0 to CFG_TILES_NUM - 1) of integer range 0 to 1;\n")

  fp.write("\n")

  #
  fp.write("  ------ Plug&Play info on HP bus\n")

  #
  fp.write("  -- Leon3 CPU cores\n")
  fp.write("  constant leon3_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_LEON3, 0, LEON3_VERSION, 0),\n")
  fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- Ibex CPU cores\n")
  fp.write("  constant ibex_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_LOWRISC, LOWRISC_IBEX_SMALL, 0, 0, 0),\n")
  fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- JTAG master interface, acting as debug access point\n")
  if esp_config.has_jtag:
    fp.write("  constant JTAG_USEOLDCOM : integer := 0;\n")
    fp.write("  constant JTAG_REREAD : integer := 1;\n")
    fp.write("  constant JTAG_REVISION : integer := 2 - (2-JTAG_REREAD)*JTAG_USEOLDCOM;\n")
    fp.write("  constant jtag_hconfig : ahb_config_type := (\n")
    fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_AHBJTAG, 0, JTAG_REVISION, 0),\n")
    fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- Ethernet master interface, acting as debug access point\n")
  if esp_config.has_eth:
    fp.write("  constant eth0_hconfig : ahb_config_type := (\n")
    fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_ETHMAC, 0, 0, 0),\n")
    fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- Enable SGMII controller iff needed\n")
  if esp_config.has_sgmii:
    fp.write("  constant CFG_SGMII : integer range 0 to 1 := 1;\n\n")
  else:
    fp.write("  constant CFG_SGMII : integer range 0 to 1 := 0;\n\n")

  #
  fp.write("  -- SVGA controller, acting as master on a dedicated bus connecte to the frame buffer\n")
  if esp_config.has_svga:
    fp.write("  constant svga_hconfig : ahb_config_type := (\n")
    fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_SVGACTRL, 0, 3, 1),\n")
    fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- BOOT ROM HP slave\n")
  fp.write("  constant ahbrom_hindex  : integer := " + str(AHBROM_HINDEX) + ";\n")
  fp.write("  constant ahbrom_haddr   : integer := 16#000#;\n")
  fp.write("  constant ahbrom_hmask   : integer := 16#fff#;\n")
  fp.write("  constant ahbrom_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_AHBROM, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(ahbrom_haddr, '1', '1', ahbrom_hmask),\n")
  fp.write("    others => zero32);\n")

  
  fp.write("  -- AHB2APB bus bridge slave\n")
  fp.write("  constant CFG_APBADDR : integer := 16#" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03X') + "#;\n")
  fp.write("  constant ahb2apb_hindex : integer := " + str(AHB2APB_HINDEX) + ";\n")
  fp.write("  constant ahb2apb_haddr : integer := CFG_APBADDR;\n")
  fp.write("  constant ahb2apb_hmask : integer := 16#F00#;\n")
  fp.write("  constant ahb2apb_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( 1, 6, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(CFG_APBADDR, '0', '0', ahb2apb_hmask),\n")
  fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- RISC-V CLINT\n")
  fp.write("  constant clint_hindex  : integer := " + str(RISCV_CLINT_HINDEX) + ";\n")
  fp.write("  constant clint_haddr   : integer := 16#020#;\n")
  fp.write("  constant clint_hmask   : integer := 16#fff#;\n")
  fp.write("  constant clint_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_SIFIVE, SIFIVE_CLINT0, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(clint_haddr, '0', '0', clint_hmask),\n")
  fp.write("    others => zero32);\n\n")

  #
  fp.write("  -- Debug access points proxy index\n")
  fp.write("  constant dbg_remote_ahb_hindex : integer := 3;\n\n")

  #
  fp.write("  ----  Shared Local Memory\n")
  # Reserve 64MB (no need to check total size!)
  # If memory tiles are present: 0x04000000 - 0x08000000 
  # If no memory tile is present:
  #  - RISC-V   : 0x80000000 - 0x88000000
  #  - SPARC V8 : 0x40000000 - 0x48000000
  # Smaller alowed size for an SLM tile is 1 MB
  if esp_config.nmem == 0:
    # Use SLM in as main memory
    global SLM_HADDR
    SLM_HADDR = DDR_HADDR[esp_config.cpu_arch];
  offset = SLM_HADDR;
  mask = 0xfff
  if esp_config.slm_kbytes >= 1024:
    mask = 0xfff & ~(int(esp_config.slm_kbytes / 1024) - 1)
  # Use any available hindex
  hindex = SLM_HINDEX

  if esp_config.nslm == 0:
    fp.write("  constant slm_hindex : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slm_hindex : attribute_vector(0 to CFG_NSLM_TILE - 1) := (\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i) + " => " + str(hindex) + ",\n")
    hindex = hindex + 1;
    if hindex == 12:
      hindex = 13
  fp.write("    others => 0);\n")

  if esp_config.nslm == 0:
    fp.write("  constant slm_haddr : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slm_haddr : attribute_vector(0 to CFG_NSLM_TILE - 1) := (\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i) + " => 16#" + format(offset, '03X') + "#,\n")
    offset = offset + int(esp_config.slm_kbytes / 1024)
  fp.write("    others => 0);\n")

  if esp_config.nslm == 0:
    fp.write("  constant slm_hmask : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slm_hmask : attribute_vector(0 to CFG_NSLM_TILE - 1) := (\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i) + " => 16#" + format(mask, '03X') + "#,\n")
  fp.write("    others => 0);\n")

  fp.write("  constant slm_hconfig : ahb_slv_config_vector := (\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      0 => ahb_device_reg ( VENDOR_SLD, SLD_SLM, 0, 0, 0),\n")
    fp.write("      4 => ahb_membar(slm_haddr(" + str(i) + "), '0', '0', slm_hmask(" + str(i) + ")),\n")
    fp.write("      others => zero32),\n")
  fp.write("    others => hconfig_none);\n\n")

  fp.write("  -- CPU tiles don't need to know how the address space is split across shared\n")
  fp.write("  -- local memory tiles and each CPU should be able to address any region\n")
  fp.write("  -- transparently.\n")
  fp.write("  constant cpu_tile_slm_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_SLD, SLD_SLM, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(16#" + format(SLM_HADDR, '03X') + "#, '0', '0', 16#" + format(esp_config.slm_full_mask, '03X')  + "#),\n")
  fp.write("    others => zero32);\n\n")


  # SLM Tiles with off-chip DDR memory
  global SLMDDR_HADDR
  offset = SLMDDR_HADDR;
  mask = 0xfff
  mask = 0xfff & ~(int(esp_config.slmddr_kbytes / 1024) - 1)
  # Use any available hindex
  hindex = SLM_HINDEX + esp_config.nslm

  if esp_config.nslmddr == 0:
    fp.write("  constant slmddr_hindex : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slmddr_hindex : attribute_vector(0 to CFG_NSLMDDR_TILE - 1) := (\n")
  for i in range(0, esp_config.nslmddr):
    fp.write("    " + str(i) + " => " + str(hindex) + ",\n")
    hindex = hindex + 1;
    if hindex == 12:
      hindex = 13
  fp.write("    others => 0);\n")

  if esp_config.nslmddr == 0:
    fp.write("  constant slmddr_haddr : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slmddr_haddr : attribute_vector(0 to CFG_NSLMDDR_TILE - 1) := (\n")
  for i in range(0, esp_config.nslmddr):
    fp.write("    " + str(i) + " => 16#" + format(offset, '03X') + "#,\n")
    offset = offset + int(esp_config.slmddr_kbytes / 1024)
  fp.write("    others => 0);\n")

  if esp_config.nslmddr == 0:
    fp.write("  constant slmddr_hmask : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slmddr_hmask : attribute_vector(0 to CFG_NSLMDDR_TILE - 1) := (\n")
  for i in range(0, esp_config.nslmddr):
    fp.write("    " + str(i) + " => 16#" + format(mask, '03X') + "#,\n")
  fp.write("    others => 0);\n")

  fp.write("  constant slmddr_hconfig : ahb_slv_config_vector := (\n")
  for i in range(0, esp_config.nslmddr):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      0 => ahb_device_reg (VENDOR_GAISLER, GAISLER_MIG_7SERIES, 0, 0, 0),\n")
    fp.write("      4 => ahb_membar(slmddr_haddr(" + str(i) + "), '0', '0', slmddr_hmask(" + str(i) + ")),\n")
    fp.write("      others => zero32),\n")
  fp.write("    others => hconfig_none);\n\n")

  fp.write("  -- CPU tiles don't need to know how the address space is split across shared\n")
  fp.write("  -- local memory tiles and each CPU should be able to address any region\n")
  fp.write("  -- transparently.\n")
  fp.write("  constant cpu_tile_slmddr_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_MIG_7SERIES, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(16#" + format(SLMDDR_HADDR, '03X') + "#, '0', '0', 16#" + format(esp_config.slmddr_full_mask, '03X')  + "#),\n")
  fp.write("    others => zero32);\n\n")


  #
  fp.write("  ----  Memory controllers\n")
  offset = DDR_HADDR[esp_config.cpu_arch];
  if esp_config.nmem > 0:
    size = int(DDR_SIZE / esp_config.nmem)
  else:
    size = DDR_SIZE
  mask = 0xfff & ~(size - 1)
  full_mask = 0xfff & ~(DDR_SIZE - 1)

  #
  fp.write("  -- CPU tiles don't need to know how the address space is split across memory tiles\n")
  fp.write("  -- and each CPU should be able to address any region transparently.\n")
  fp.write("  constant cpu_tile_mig7_hconfig : ahb_config_type := (\n")
  fp.write("    0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_MIG_7SERIES, 0, 0, 0),\n")
  fp.write("    4 => ahb_membar(16#" + format(DDR_HADDR[esp_config.cpu_arch], '03X') + "#, '1', '1', 16#" + format(full_mask, '03X')  + "#),\n")
  fp.write("    others => zero32);\n")

  #
  if esp_config.nmem == 0:
    ddr_vec_attribute_msb = "0"
  else:
    ddr_vec_attribute_msb = "CFG_NMEM_TILE - 1"

  fp.write("  -- Network interfaces and ESP proxies, instead, need to know how to route packets\n")
  fp.write("  constant ddr_hindex : attribute_vector(0 to " + ddr_vec_attribute_msb  + ") := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => " + str(DDR_HINDEX[i]) + ",\n")
  fp.write("    others => 0);\n")

  fp.write("  constant ddr_haddr : attribute_vector(0 to " + ddr_vec_attribute_msb  + ") := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => 16#" + format(offset, '03X') + "#,\n")
    offset = offset + size
  fp.write("    others => 0);\n")

  fp.write("  constant ddr_hmask : attribute_vector(0 to " + ddr_vec_attribute_msb  + ") := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => 16#" + format(mask, '03X') + "#,\n")
  fp.write("    others => 0);\n")

  #
  fp.write("  -- Create a list of memory controllers info based on the number of memory tiles\n")
  fp.write("  -- We use the MIG interface from GRLIB, which has a device entry for the 7SERIES\n")
  fp.write("  -- Xilinx FPGAs only, however, we provide a patched version of the IP for the\n")
  fp.write("  -- UltraScale(+) FPGAs. The patched intercace shared the same device ID with the\n")
  fp.write("  -- 7SERIES MIG.\n")
  fp.write("  constant mig7_hconfig : ahb_slv_config_vector := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_MIG_7SERIES, 0, 0, 0),\n")
    fp.write("      4 => ahb_membar(ddr_haddr(" + str(i) + "), '1', '1', ddr_hmask(" + str(i) + ")),\n")
    fp.write("      others => zero32),\n")
  fp.write("    others => hconfig_none);\n")

  #
  fp.write("  -- On-chip frame buffer (GRLIB)\n")
  fp.write("  constant fb_hindex : integer := " + str(FB_HINDEX) + ";\n")
  fp.write("  constant fb_hmask : integer := 16#FFF#;\n")
  fp.write("  constant fb_haddr : integer := CFG_SVGA_MEMORY_HADDR;\n")
  if esp_config.has_svga:
    fp.write("  constant fb_hconfig : ahb_config_type := (\n")
    fp.write("    0 => ahb_device_reg ( VENDOR_SLD, SLD_AHBRAM_DP, 0, 11, 0),\n")
    fp.write("    4 => ahb_membar(fb_haddr, '0', '0', fb_hmask),\n")
    fp.write("    others => zero32);\n\n")
  else:
    fp.write("  constant fb_hconfig : ahb_config_type := hconfig_none;\n\n")

  #
  fp.write("  -- HP slaves index / memory map\n")
  fp.write("  constant fixed_ahbso_hconfig : ahb_slv_config_vector := (\n")
  fp.write("    " + str(AHBROM_HINDEX) + " => ahbrom_hconfig,\n")
  fp.write("    " + str(AHB2APB_HINDEX) + " => ahb2apb_hconfig,\n")
  if esp_config.cpu_arch == "ariane" or esp_config.cpu_arch == "ibex":
    fp.write("    " + str(RISCV_CLINT_HINDEX) + " => clint_hconfig,\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(DDR_HINDEX[i]) + " => mig7_hconfig(" + str(i) + "),\n")
  for i in range(0, esp_config.nslm):
    index = SLM_HINDEX + i
    if index >= 12:
      index = index + 1
    fp.write("    " + str(index) + " => slm_hconfig(" + str(i) + "),\n")
  for i in range(0, esp_config.nslmddr):
    index = SLM_HINDEX + esp_config.nslm + i
    if index >= 12:
      index = index + 1
    fp.write("    " + str(index) + " => slmddr_hconfig(" + str(i) + "),\n")
  fp.write("    " + str(FB_HINDEX) + " => fb_hconfig,\n")
  fp.write("    others => hconfig_none);\n\n")

  #
  fp.write("  -- HP slaves index / memory map for CPU tile\n")
  fp.write("  -- CPUs need to see memory as a single address range\n")
  fp.write("  constant cpu_tile_fixed_ahbso_hconfig : ahb_slv_config_vector := (\n")
  fp.write("    " + str(AHBROM_HINDEX) + " => ahbrom_hconfig,\n")
  fp.write("    " + str(AHB2APB_HINDEX) + " => ahb2apb_hconfig,\n")
  if esp_config.cpu_arch == "ariane" or esp_config.cpu_arch == "ibex":
    fp.write("    " + str(RISCV_CLINT_HINDEX) + " => clint_hconfig,\n")
  if esp_config.nmem != 0:
    fp.write("    " + str(DDR_HINDEX[0]) + " => cpu_tile_mig7_hconfig,\n")
  fp.write("    " + str(SLM_HINDEX) + " => cpu_tile_slm_hconfig,\n")
  fp.write("    " + str(SLM_HINDEX + 1) + " => cpu_tile_slmddr_hconfig,\n")
  fp.write("    " + str(FB_HINDEX) + " => fb_hconfig,\n")
  fp.write("    others => hconfig_none);\n\n")

  #
  fp.write("  ------ Plug&Play info on I/O bus\n")

  #
  fp.write("  -- UART (GRLIB)\n")
  fp.write("  constant uart_pconfig : apb_config_type := (\n")
  fp.write("  0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_APBUART, 0, 1, CFG_UART1_IRQ),\n")
  fp.write("  1 => apb_iobar(16#001#, 16#fff#),\n")
  fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- Interrupt controller (Architecture-dependent)\n")
  fp.write("  -- RISC-V PLIC is using the extended APB address space\n")
  fp.write("  constant irqmp_pconfig : apb_config_type := (\n")
  if esp_config.cpu_arch == "leon3":
    fp.write("  0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_IRQMP, 0, 3, 0),\n")
    fp.write("  1 => apb_iobar(16#002#, 16#fff#),\n")
    fp.write("  2 => (others => '0'));\n\n")
  elif esp_config.cpu_arch == "ariane" or esp_config.cpu_arch == "ibex":
    fp.write("  0 => ahb_device_reg ( VENDOR_SIFIVE, SIFIVE_PLIC0, 0, 3, 0),\n")
    fp.write("  1 => apb_iobar(16#000#, 16#000#),\n")
    fp.write("  2 => apb_iobar(16#0C0#, 16#FC0#));\n\n")

  #
  fp.write("  -- Timer (GRLIB)\n")
  fp.write("  constant gptimer_pconfig : apb_config_type := (\n")
  fp.write("  0 => ahb_device_reg (VENDOR_GAISLER, GAISLER_GPTIMER, 0, 1, CFG_GPT_IRQ),\n")
  fp.write("  1 => apb_iobar(16#003#, 16#fff#),\n")
  fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- Ibex Timer\n")
  fp.write("  constant ibex_timer_pconfig : apb_config_type := (\n")
  fp.write("  0 => ahb_device_reg (VENDOR_LOWRISC, LOWRISC_IBEX_TIMER, 0, 1, 0),\n")
  fp.write("  1 => apb_iobar(16#003#, 16#fff#),\n")
  fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- ESPLink\n")
  fp.write("  constant esplink_pconfig : apb_config_type := (\n")
  fp.write("  0 => ahb_device_reg (VENDOR_SLD, SLD_ESPLINK, 0, 0, 0),\n")
  fp.write("  1 => apb_iobar(16#004#, 16#fff#),\n")
  fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- SVGA controler (GRLIB)\n")
  if esp_config.has_svga:
    fp.write("  constant svga_pconfig : apb_config_type := (\n")
    fp.write("  0 => ahb_device_reg (VENDOR_GAISLER, GAISLER_SVGACTRL, 0, 0, 0),\n")
    fp.write("  1 => apb_iobar(16#006#, 16#fff#),\n")
    fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- Ethernet MAC (GRLIB)\n")
  if esp_config.has_eth:
    fp.write("  constant eth0_pconfig : apb_config_type := (\n")
    fp.write("  0 => ahb_device_reg (VENDOR_GAISLER, GAISLER_ETHMAC, 0, 0, 12),\n")
    fp.write("  1 => apb_iobar(16#800#, 16#f00#),\n")
    fp.write("  2 => (others => '0'));\n\n")

    fp.write("  constant sgmii0_pconfig : apb_config_type := (\n")
    fp.write("  0 => ahb_device_reg (VENDOR_GAISLER, GAISLER_SGMII, 0, 1, 11),\n")
    fp.write("  1 => apb_iobar(16#010#, 16#ff0#),\n")
    fp.write("  2 => (others => '0'));\n\n")

  #
  fp.write("  -- CPU DVFS controller\n")
  fp.write("  -- Accelerators' power controllers are mapped to the upper half of their I/O\n")
  fp.write("  -- address space. In the future, each DVFS controller should be assigned to an independent\n")
  fp.write("  -- region of the address space, thus allowing discovery from the device tree.\n")

  fp.write("  constant cpu_dvfs_paddr : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    dvfs = esp_config.tiles[i].dvfs
    if dvfs.id != -1:
      fp.write("    " + str(i) + " => 16#" + format(0xD0 + dvfs.idx, '03X') + "#,\n")
  fp.write("    others => 0);\n")

  fp.write("  constant cpu_dvfs_pmask : integer := 16#fff#;\n")

  fp.write("  constant cpu_dvfs_pconfig : apb_config_vector_type(0 to " + str(esp_config.ntiles - 1) + ") := (\n")
  for i in range(0, esp_config.ntiles):
    dvfs = esp_config.tiles[i].dvfs
    if dvfs.id != -1:
      fp.write("    " + str(dvfs.id) + " => (\n")
      fp.write("      0 => ahb_device_reg (VENDOR_SLD, SLD_POWERCTRL, 0, 0, 0),\n")
      fp.write("      1 => apb_iobar(16#" + format(0xD0 + dvfs.idx, '03X') + "#, 16#fff#),\n")
      fp.write("      2 => (others => '0')),\n")
  fp.write("    others => pconfig_none);\n\n")

  #
  fp.write("  -- L2\n")
  fp.write("  -- Accelerator's caches cannot be flushed/reset from I/O-bus\n")
  fp.write("  constant l2_cache_pconfig : apb_l2_pconfig_vector := (\n")
  for i in range(0, esp_config.nl2):
    l2 = esp_config.l2s[i]
    if l2.idx != -1:
      fp.write("    " + str(l2.id) + " => (\n")
      if soc.cache_spandex.get() == 1:
        fp.write("      0 => ahb_device_reg (VENDOR_UIUC, UIUC_SPANDEX_L2, 0, 0, CFG_SLD_L2_CACHE_IRQ),\n")
      else:
        fp.write("      0 => ahb_device_reg (VENDOR_SLD, SLD_L2_CACHE, 0, 0, CFG_SLD_L2_CACHE_IRQ),\n")
      fp.write("      1 => apb_iobar(16#" + format(0xD0 + l2.idx, '03X') + "#, 16#fff#),\n")
      fp.write("      2 => (others => '0')),\n")
  fp.write("    others => pconfig_none);\n\n")

  #
  fp.write("  -- LLC\n")
  fp.write("  constant llc_cache_pconfig : apb_llc_pconfig_vector := (\n")
  for i in range(0, esp_config.nllc):
    llc = esp_config.llcs[i]
    fp.write("    " + str(i) + " => (\n")
    if soc.cache_spandex.get() == 1:
      fp.write("      0 => ahb_device_reg (VENDOR_UIUC, UIUC_SPANDEX_LLC, 0, 0, CFG_SLD_LLC_CACHE_IRQ),\n")
    else:
      fp.write("      0 => ahb_device_reg (VENDOR_SLD, SLD_LLC_CACHE, 0, 0, CFG_SLD_LLC_CACHE_IRQ),\n")
    fp.write("      1 => apb_iobar(16#" + format(0xD0 + llc.idx, '03X') + "#, 16#fff#),\n")
    fp.write("      2 => (others => '0')),\n")
  fp.write("    others => pconfig_none);\n\n")


  #
  fp.write("  -- ESP Tiles CSRs\n")
  csr_apb_size = (~CSR_APB_ADDR_MSK & 0xfff) + 1
  for i in range(0, esp_config.ntiles):
    address_str = format(CSR_APB_ADDR + i * csr_apb_size, "03X")
    msk_str = format(CSR_APB_ADDR_MSK, "03X")
    fp.write("  constant csr_t_" + str(i) + "_pindex : integer range 0 to NAPBSLV - 1 := " + str(CSR_PINDEX[i]) + ";\n")
    fp.write("  constant csr_t_" + str(i) + "_paddr : integer range 0 to 4095 := 16#" + address_str + "#;\n")
    fp.write("  constant csr_t_" + str(i) + "_pmask : integer range 0 to 4095 := 16#" + msk_str + "#;\n")
    fp.write("  constant csr_t_" + str(i) + "_pconfig : apb_config_type := (\n")
    fp.write("  0 => ahb_device_reg (VENDOR_SLD, SLD_TILE_CSR, 0, 0, 0),\n")
    fp.write("  1 => apb_iobar(16#" + address_str + "#, 16#" + msk_str  + "#),\n")
    fp.write("  2 => (others => '0'));\n\n")


  #
  fp.write("  -- Accelerators\n")
  fp.write("  constant accelerators_num : integer := " + str(esp_config.nacc) + ";\n\n")
  # Reset all THIRDPARTY accelerators counters
  THIRDPARTY_N = 0

  for i in range(esp_config.nacc):
    acc = esp_config.accelerators[i]
    fp.write("  -- Accelerator " + str(acc.id) + "\n")
    fp.write("  -- APB " + str(acc.idx) + "\n")
    fp.write("  -- " + acc.uppercase_name + "\n")

    if acc.vendor == "sld":
      address = SLD_APB_ADDR + acc.id
      address_ext = 0
      msk = SLD_APB_ADDR_MSK
      msk_ext = 0
      address_str = format(address, "03X")
      address_ext_str = format(address_ext, "03X")
    else:
      n = THIRDPARTY_N
      # Compute base address
      if THIRDPARTY_APB_EXT_ADDRESS == 0:
        # Use part of standard APB address space
        address = THIRDPARTY_APB_ADDRESS + n * THIRDPARTY_APB_ADDRESS_SIZE
        size = THIRDPARTY_APB_ADDRESS_SIZE
        address_ext = 0
        size_ext = 0
      else:
        # Use extended APB address space (large number of registers)
        address = 0
        size = 0
        address_ext = THIRDPARTY_APB_EXT_ADDRESS + n * THIRDPARTY_APB_EXT_ADDRESS_SIZE
        size_ext = THIRDPARTY_APB_EXT_ADDRESS_SIZE

      msk = 0xfff & ~((size >> 8) - 1)
      msk_ext = 0xfff & ~((size_ext >> 20) - 1)
      address_str = format(address >> 8, "03X")
      address_ext_str = format(address_ext >> 20, "03X")

      # Increment count
      THIRDPARTY_N = n + 1;

    msk_str = format(msk, "03X")
    msk_ext_str = format(msk_ext, "03X")

    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_pindex : integer range 0 to NAPBSLV - 1 := " + str(acc.idx) + ";\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_pirq : integer range 0 to NAHBIRQ - 1 := " + str(acc.irq) + ";\n")
    if acc.vendor == "sld":
      fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_irq_type : integer range 0 to 1 := " + "0" + ";\n")
    else:
      fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_irq_type : integer range 0 to 1 := " + THIRDPARTY_IRQ_TYPE[acc.lowercase_name] + ";\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_paddr : integer range 0 to 4095 := 16#" + str(address_str) + "#;\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_pmask : integer range 0 to 4095 := 16#" + str(msk_str) + "#;\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_paddr_ext : integer range 0 to 4095 := 16#" + str(address_ext_str) + "#;\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_pmask_ext : integer range 0 to 4095 := 16#" + str(msk_ext_str) + "#;\n")
    fp.write("  constant " + acc.lowercase_name + "_" + str(acc.id) + "_pconfig : apb_config_type := (\n")
    fp.write("  0 => ahb_device_reg (VENDOR_SLD, SLD_" + acc.uppercase_name + ", 0, 0, " + str(acc.irq) + "),\n")
    fp.write("  1 => apb_iobar(16#" + address_str + "#, 16#" + msk_str  + "#),\n")
    if address_ext == 0:
      fp.write("  2 => (others => '0'));\n\n")
    else:
      fp.write("  2 => apb_iobar(16#" + address_ext_str + "#, 16#" + msk_ext_str  + "#));\n\n")

  #
  fp.write("  -- I/O bus slaves index / memory map\n")
  fp.write("  constant fixed_apbo_pconfig : apb_slv_config_vector := (\n")
  fp.write("    1 => uart_pconfig,\n")
  fp.write("    2 => irqmp_pconfig,\n")
  if esp_config.cpu_arch == "ibex":
    fp.write("    3 => ibex_timer_pconfig,\n")
  else:
    fp.write("    3 => gptimer_pconfig,\n")
  fp.write("    4 => esplink_pconfig,\n")
  for i in range(0, esp_config.ndvfs):
    dvfs = esp_config.dvfs_ctrls[i]
    fp.write("    " + str(dvfs.idx) + " => cpu_dvfs_pconfig(" + str(i) + "),\n")
  for i in range(0, esp_config.nl2):
    l2 = esp_config.l2s[i]
    if l2.idx != -1:
      fp.write("    " + str(l2.idx) + " => l2_cache_pconfig(" + str(l2.id) + "),\n")
  for i in range(0, esp_config.nllc):
    llc = esp_config.llcs[i]
    fp.write("    " + str(llc.idx) + " => llc_cache_pconfig(" + str(i) + "),\n")
  if esp_config.has_svga:
    fp.write("    13 => svga_pconfig,\n")
  if esp_config.has_eth:
    fp.write("    14 => eth0_pconfig,\n")
    if esp_config.has_sgmii:
      fp.write("    15 => sgmii0_pconfig,\n")
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(CSR_PINDEX[i]) + " => csr_t_" + str(i) + "_pconfig,\n")
  for i in range(0, esp_config.nacc):
    acc = esp_config.accelerators[i]
    fp.write("    " + str(acc.idx) + " => " + str(acc.lowercase_name) + "_" + str(acc.id) + "_pconfig,\n")
  fp.write("    others => pconfig_none);\n\n")


  #
  fp.write("  ------ Cross reference arrays\n")

  #
  fp.write("  -- Get CPU ID from tile ID\n")
  fp.write("  constant tile_cpu_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.cpu_id != -1:
      fp.write("    " + str(i) + " => " + str(t.cpu_id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from CPU ID\n")
  fp.write("  constant cpu_tile_id : attribute_vector(0 to CFG_NCPU_TILE - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "cpu":
      fp.write("    " + str(esp_config.tiles[i].cpu_id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get DVFS controller pindex from tile ID\n")
  fp.write("  constant cpu_dvfs_pindex : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.type == "cpu" and t.has_pll != 0:
      fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].dvfs.idx) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get L2 cache ID from tile ID\n")
  fp.write("  constant tile_cache_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    l2 = esp_config.tiles[i].l2
    if l2.id != -1:
      fp.write("    " + str(i) + " => " + str(l2.id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from L2 cache ID\n")
  fp.write("  constant cache_tile_id : cache_attribute_array := (\n")
  for i in range(0, esp_config.ntiles):
    l2 = esp_config.tiles[i].l2
    if l2.id != -1:
      fp.write("    " + str(l2.id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get L2 pindex from tile ID\n")
  fp.write("  constant l2_cache_pindex : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.type == "cpu" and t.l2.idx != -1:
      fp.write("    " + str(i) + " => " + str(t.l2.idx) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Flag tiles that have a private cache\n")
  fp.write("  constant tile_has_l2 : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].has_l2) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get LLC ID from tile ID\n")
  fp.write("  constant tile_llc_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    llc = esp_config.tiles[i].llc
    if llc.id != -1:
      fp.write("    " + str(i) + " => " + str(llc.id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from LLC-split ID\n")
  fp.write("  constant llc_tile_id : attribute_vector(0 to " + ddr_vec_attribute_msb  + ") := (\n")
  for i in  range(0, esp_config.ntiles):
    llc = esp_config.tiles[i].llc
    if llc.id != -1:
      fp.write("    " + str(llc.id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get LLC pindex from tile ID\n")
  fp.write("  constant llc_cache_pindex : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.type == "mem" and t.llc.idx != -1:
      fp.write("    " + str(i) + " => " + str(t.llc.idx) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from shared-local memory ID ID\n")
  if esp_config.nslm == 0:
    fp.write("  constant slm_tile_id : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slm_tile_id : attribute_vector(0 to CFG_NSLM_TILE - 1) := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.slm_id != -1:
      fp.write("    " + str(t.slm_id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get shared-local memory tile ID from tile ID\n")
  fp.write("  constant tile_slm_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.slm_id != -1:
      fp.write("    " + str(i) + " => " + str(t.slm_id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from shared-local memory with DDR ID ID\n")
  if esp_config.nslmddr == 0:
    fp.write("  constant slmddr_tile_id : attribute_vector(0 to 0) := (\n")
  else:
    fp.write("  constant slmddr_tile_id : attribute_vector(0 to CFG_NSLMDDR_TILE - 1) := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.slmddr_id != -1:
      fp.write("    " + str(t.slmddr_id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get shared-local memory tile ID from tile ID\n")
  fp.write("  constant tile_slmddr_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.slmddr_id != -1:
      fp.write("    " + str(i) + " => " + str(t.slmddr_id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID from memory ID\n")
  fp.write("  constant mem_tile_id : attribute_vector(0 to " + ddr_vec_attribute_msb  + ") := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.mem_id != -1:
      fp.write("    " + str(t.mem_id) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get memory tile ID from tile ID\n")
  fp.write("  constant tile_mem_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in  range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.mem_id != -1:
      fp.write("    " + str(i) + " => " + str(t.mem_id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get CSR pindex from tile ID\n")
  fp.write("  constant tile_csr_pindex : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(i) + " => " + str(CSR_PINDEX[i]) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get accelerator ID from tile ID\n")
  fp.write("  constant tile_acc_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    t = esp_config.tiles[i]
    if t.acc.id != -1:
      fp.write("    " + str(i) + " => " + str(t.acc.id) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get miscellaneous tile ID\n")
  misc_id = 0
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "misc":
      misc_id = i
  fp.write("  constant io_tile_id : integer := " + str(misc_id) + ";\n\n")

  #
  fp.write("  -- DMA ID corresponds to accelerator ID for accelerators and nacc for Ethernet\n")
  fp.write("  -- Ethernet must be coherent to avoid flushing private caches every time the\n")
  fp.write("  -- DMA buffer is accessed, but the IP from GRLIB is not coherent. We leverage\n")
  fp.write("  -- LLC-coherent DMA w/ recalls to have Etherent work transparently.\n")

  fp.write("  -- Get DMA ID from tile ID\n")
  fp.write("  constant tile_dma_id : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    acc = esp_config.tiles[i].acc
    if acc.id != -1:
      fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].acc.id) + ",\n")
  fp.write("    io_tile_id => " + str(esp_config.nacc) + ",\n")
  fp.write("    others => -1);\n\n")


  fp.write("  -- Get tile ID from DMA ID (used for LLC-coherent DMA)\n")
  fp.write("  constant dma_tile_id : dma_attribute_array := (\n")
  for i in range(0, esp_config.ntiles):
    acc = esp_config.tiles[i].acc
    if acc.id != -1:
      fp.write("    " + str(acc.id) + " => " + str(i) + ",\n")
  fp.write("    " + str(esp_config.nacc) + " => io_tile_id,\n")
  fp.write("    others => 0);\n\n")


  #
  fp.write("  -- Get type of tile from tile ID\n")
  fp.write("  constant tile_type : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    type = 0
    if esp_config.tiles[i].type == "cpu":
      type = 1
    if esp_config.tiles[i].type == "acc":
      type = 2
    if esp_config.tiles[i].type == "misc":
      type = 3
    if esp_config.tiles[i].type == "mem":
      type = 4
    if esp_config.tiles[i].type == "slm":
      type = 5
    if esp_config.tiles[i].type == "slmddr":
      type = 6
    fp.write("    " + str(i) + " => " + str(type) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get accelerator's implementation (hlscfg or generic design point) from tile ID\n")
  fp.write("  constant tile_design_point : tile_hlscfg_array := (\n")
  for i in range(0, esp_config.ntiles):
    tile = esp_config.tiles[i]
    if tile.type == "acc":
      if str(tile.design_point) != "":
        fp.write("    " + str(i) + " => HLSCFG_" + tile.acc.uppercase_name + "_" + tile.design_point.upper() + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get accelerator device ID (device tree) from tile ID\n")
  fp.write("  constant tile_device : tile_device_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      fp.write("    " + str(i) + " => SLD_" + esp_config.tiles[i].acc.uppercase_name + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get I/O-bus index line for accelerators from tile ID\n")
  fp.write("  constant tile_apb_idx : tile_idx_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_pindex,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get I/O-bus address for accelerators from tile ID\n")
  fp.write("  constant tile_apb_paddr : tile_addr_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_paddr,\n")
  fp.write("    others => 0);\n\n")

  fp.write("  constant tile_apb_paddr_ext : tile_addr_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_paddr_ext,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get I/O-bus address mask for accelerators from tile ID\n")
  fp.write("  constant tile_apb_pmask : tile_addr_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_pmask,\n")
  fp.write("    others => 0);\n\n")

  fp.write("  constant tile_apb_pmask_ext : tile_addr_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_pmask_ext,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get IRQ line for accelerators from tile ID\n")
  fp.write("  constant tile_apb_irq : tile_irq_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_pirq,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get IRQ line type for accelerators from tile ID\n")
  fp.write("  -- IRQ line types:\n")
  fp.write("  --     0 : edge-sensitive\n")
  fp.write("  --     1 : level-sensitive\n")
  fp.write("  constant tile_irq_type : tile_irq_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      acc = esp_config.tiles[i].acc
      fp.write("    " + str(i) + " => " + acc.lowercase_name + "_" + str(acc.id) + "_irq_type,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get DMA memory allocation from tile ID (this parameter must be the same for every accelerator)\n")
  fp.write("  constant tile_scatter_gather : tile_flag_array := (\n")
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].type == "acc":
      fp.write("    " + str(i) + " => CFG_SCATTER_GATHER,\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get number of clock regions (1 if has_dvfs is false)\n")
  fp.write("  constant domains_num : integer := " + str(esp_config.ndomain)+";\n\n")

  #
  fp.write("  -- Flag tiles that belong to a DVFS domain\n")
  fp.write("  constant tile_has_dvfs : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  if esp_config.has_dvfs:
    for i in range(0, esp_config.ntiles):
      region = esp_config.tiles[i].clk_region
      has_dvfs = 0
      if region != 0:
        has_dvfs = 1
      fp.write("    " + str(i) + " => " + str(has_dvfs) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Flag tiles that are master of a DVFS domain (have a local PLL)\n")
  fp.write("  constant tile_has_pll : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  if esp_config.has_dvfs:
    for i in range(0, esp_config.ntiles):
      fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].has_pll) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get clock domain from tile ID\n")
  fp.write("  constant tile_domain : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  if esp_config.has_dvfs:
    for i in range(0, esp_config.ntiles):
      fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].clk_region) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID of the DVFS domain masters for each clock region (these tiles control the corresponding domain)\n")
  pll_tile = [0 for x in range(esp_config.ntiles)]
  for i in range(0, esp_config.ntiles):
    if esp_config.tiles[i].has_pll == 1:
      pll_tile[esp_config.tiles[i].clk_region] = i

  #
  fp.write("  -- Get tile ID of the DVFS domain master from the tile clock region\n")
  fp.write("  constant tile_domain_master : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(i) + " => " + str(pll_tile[esp_config.tiles[i].clk_region]) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Get tile ID of the DVFS domain master from the clock region ID\n")
  fp.write("  constant domain_master_tile : domain_type_array := (\n")
  if esp_config.has_dvfs:
    for i in range(0, esp_config.ndomain):
      region = esp_config.regions[i]
      if region != 0:
        fp.write("    " + str(region) + " => " + str(pll_tile[region]) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  -- Flag domain master tiles w/ additional clock buffer (these are a limited resource on the FPGA)\n")
  fp.write("  constant extra_clk_buf : attribute_vector(0 to CFG_TILES_NUM - 1) := (\n")
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(i) + " => " + str(esp_config.tiles[i].has_clkbuf) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  ---- Get tile ID from I/O-bus index (index 4 is the local DVFS controller to each CPU tile)\n")
  fp.write("  constant apb_tile_id : apb_attribute_array := (\n")
  # 0 - BOOT ROM memory controller
  # 1 - I/O bus bridge
  # 2 - Interrupt controller
  # 3 - Timer
  for i in range(0, 4):
    fp.write("    " + str(i) + " => io_tile_id,\n")
  # 13 - SVGA controller
  if esp_config.has_svga:
    fp.write("    13 => io_tile_id,\n")
  # 14 - Ethernet MAC controller
  # 15 - Ethernet SGMII PHY controller
  if esp_config.has_eth:
    fp.write("    14 => io_tile_id,\n")
    if esp_config.has_sgmii:
      fp.write("    15 => io_tile_id,\n")
  # 20-83 - ESP Tile CSRs
  for i in range(0, esp_config.ntiles):
    fp.write("    " + str(CSR_PINDEX[i]) + " => " + str(i) + ",\n")
  # 20-(NAPBSLV - 1) - Accelerators
  for i in range(0, esp_config.ntiles):
    t =  esp_config.tiles[i]
    if t.type == "acc":
      acc_pindex = t.acc.idx
      fp.write("    " + str(acc_pindex) + " => " + str(i) + ",\n")
    # 16-19 - LLC cache controller (must change with NMEM_MAX)
    if t.type == "mem":
      if esp_config.coherence:
        llc_pindex = t.llc.idx
        fp.write("    " + str(llc_pindex) + " => " + str(i) + ",\n")
    # 5-8 - Processors' DVFS controller (must change with NCPU_MAX)
    # 9-12 - Processors' private cache controller (must change with NCPU_MAX)
    if t.type == "cpu":
      if t.has_pll != 0:
        fp.write("    " + str(t.dvfs.idx) + " => " + str(i) + ",\n")
      if esp_config.coherence:
        fp.write("    " + str(t.l2.idx) + " => " + str(i) + ",\n")
  fp.write("    others => 0);\n\n")

  #
  fp.write("  constant esp_init_sequence : attribute_vector(0 to CFG_TILES_NUM + CFG_NCPU_TILE - 1) := (\n")
  fp.write("    ")
  # Set tile_id to adjacent tiles starting from tile_io
  for i in list(range(esp_config.tiles[misc_id].row, -1, -1)) + list(range(esp_config.tiles[misc_id].row + 1, soc.noc.rows)):
    for j in list(range(esp_config.tiles[misc_id].col, -1, -1)) + list(range(esp_config.tiles[misc_id].col + 1, soc.noc.cols)):
      tile_id = i * soc.noc.cols + j
      if tile_id != misc_id:
        fp.write(", ")
      fp.write(str(tile_id))
  # Mark tile_id configuration valid for CPU tiles. Set CPU0 last.
  for i in range(esp_config.ntiles - 1, -1, -1):
    t =  esp_config.tiles[i]
    if t.type == "cpu":
      fp.write(", " + str(i))
  fp.write(");\n\n")

  fp.write("  constant esp_srst_sequence : attribute_vector(0 to CFG_NMEM_TILE + CFG_NCPU_TILE - 1) := (\n")
  fp.write("    ")
  # Send srs to MEM tiles (LLC) and to CPU tiles. Reset CPU0 last.
  first = True
  for i in range(esp_config.ntiles - 1, -1, -1):
    t =  esp_config.tiles[i]
    if t.type == "mem":
      if first:
        first = False
      else:
        fp.write(", ")
      fp.write(str(i))
  for i in range(esp_config.ntiles - 1, -1, -1):
    t =  esp_config.tiles[i]
    if t.type == "cpu":
      if first:
        first = False
        if esp_config.tiles[i].cpu_id == 0:
          fp.write("0 => ")
      else:
        fp.write(", ")
      fp.write(str(i))
  fp.write(");\n\n")

def print_tiles(fp, esp_config):
  #
  fp.write("  -- Tiles YX coordinates\n")
  fp.write("  constant tile_x : yx_vec(0 to " + str(esp_config.ntiles - 1) + ") := (\n")
  for i in range(0, esp_config.ntiles):
    if i > 0:
       fp.write(",\n")
    fp.write("    " + str(i) + " => \"" + socgen.uint_to_bin(esp_config.tiles[i].col, 3) + "\"")
  fp.write("  );\n")

  fp.write("  constant tile_y : yx_vec(0 to " + str(esp_config.ntiles - 1) + ") := (\n")
  for i in range(0, esp_config.ntiles):
    if i > 0:
      fp.write(",\n")
    fp.write("    " + str(i) + " => \"" + socgen.uint_to_bin(esp_config.tiles[i].row, 3) + "\"")
  fp.write("  );\n\n")

  #
  fp.write("  -- CPU YX coordinates\n")
  fp.write("  constant cpu_y : yx_vec(0 to CFG_NCPU_TILE - 1) := (\n")
  for i in range(0, esp_config.ncpu):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_y(cpu_tile_id(" + str(i) + "))")
  fp.write("  );\n")

  fp.write("  constant cpu_x : yx_vec(0 to CFG_NCPU_TILE - 1) := (\n")
  for i in range(0, esp_config.ncpu):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_x(cpu_tile_id(" + str(i) + "))")
  fp.write("  );\n\n")


  #
  fp.write("  -- L2 YX coordinates\n")
  fp.write("  constant cache_y : yx_vec(0 to " + str(NFULL_COHERENT_MAX - 1) + ") := (\n")
  for i in range(0, NFULL_COHERENT_MAX):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_y(cache_tile_id(" + str(i) + "))")
  fp.write("  );\n")

  fp.write("  constant cache_x : yx_vec(0 to " + str(NFULL_COHERENT_MAX - 1) + ") := (\n")
  for i in range(0, NFULL_COHERENT_MAX):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_x(cache_tile_id(" + str(i) + "))")
  fp.write("  );\n\n")

  #
  fp.write("  -- DMA initiators YX coordinates\n")
  fp.write("  constant dma_y : yx_vec(0 to " + str(NLLC_COHERENT_MAX - 1) + ") := (\n")
  for i in range(0, NLLC_COHERENT_MAX):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_y(dma_tile_id(" + str(i) + "))")
  fp.write("  );\n")

  fp.write("  constant dma_x : yx_vec(0 to " + str(NLLC_COHERENT_MAX - 1) + ") := (\n")
  for i in range(0, NLLC_COHERENT_MAX):
    if i > 0:
      fp.write(",\n")
    fp.write("   " + str(i) + " => tile_x(dma_tile_id(" + str(i) + "))")
  fp.write("  );\n\n")

  #
  fp.write("  -- SLM YX coordinates and tiles routing info\n")
  fp.write("  constant tile_slm_list : tile_mem_info_vector(0 to CFG_NSLM_TILE + CFG_NSLMDDR_TILE + CFG_NMEM_TILE - 1):= (\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      x => tile_x(slm_tile_id(" + str(i) + ")),\n")
    fp.write("      y => tile_y(slm_tile_id(" + str(i) + ")),\n")
    fp.write("      haddr => slm_haddr(" + str(i)  + "),\n")
    fp.write("      hmask => slm_hmask(" + str(i)  + ")\n")
    fp.write("    ),\n")
  for i in range(0, esp_config.nslmddr):
    fp.write("    " + str(esp_config.nslm + i) + " => (\n")
    fp.write("      x => tile_x(slmddr_tile_id(" + str(i) + ")),\n")
    fp.write("      y => tile_y(slmddr_tile_id(" + str(i) + ")),\n")
    fp.write("      haddr => slmddr_haddr(" + str(i)  + "),\n")
    fp.write("      hmask => slmddr_hmask(" + str(i)  + ")\n")
    fp.write("    ),\n")
  fp.write("    others => tile_mem_info_none);\n\n")

  #
  fp.write("  -- LLC YX coordinates and memory tiles routing info\n")
  fp.write("  constant tile_mem_list : tile_mem_info_vector(0 to CFG_NSLM_TILE + CFG_NSLMDDR_TILE + CFG_NMEM_TILE - 1) := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      x => tile_x(mem_tile_id(" + str(i) + ")),\n")
    fp.write("      y => tile_y(mem_tile_id(" + str(i) + ")),\n")
    fp.write("      haddr => ddr_haddr(" + str(i)  + "),\n")
    fp.write("      hmask => ddr_hmask(" + str(i)  + ")\n")
    fp.write("    ),\n")
  fp.write("    others => tile_mem_info_none);\n\n")

  #
  fp.write("  -- Add the frame buffer and SLM tiles entries for accelerators' DMA.\n")
  fp.write("  -- NB: accelerators can only access the frame buffer and SLM if\n")
  fp.write("  -- non-coherent DMA is selected from software.\n")
  fp.write("  constant tile_acc_mem_list : tile_mem_info_vector(0 to CFG_NSLM_TILE + CFG_NSLMDDR_TILE + CFG_NMEM_TILE) := (\n")
  for i in range(0, esp_config.nmem):
    fp.write("    " + str(i) + " => (\n")
    fp.write("      x => tile_x(mem_tile_id(" + str(i) + ")),\n")
    fp.write("      y => tile_y(mem_tile_id(" + str(i) + ")),\n")
    fp.write("      haddr => ddr_haddr(" + str(i)  + "),\n")
    fp.write("      hmask => ddr_hmask(" + str(i)  + ")\n")
    fp.write("    ),\n")
  for i in range(0, esp_config.nslm):
    fp.write("    " + str(i + esp_config.nmem) + " => (\n")
    fp.write("      x => tile_x(slm_tile_id(" + str(i) + ")),\n")
    fp.write("      y => tile_y(slm_tile_id(" + str(i) + ")),\n")
    fp.write("      haddr => slm_haddr(" + str(i)  + "),\n")
    fp.write("      hmask => slm_hmask(" + str(i)  + ")\n")
    fp.write("    ),\n")
  if esp_config.has_svga:
    fp.write("    " + str(esp_config.nmem + esp_config.nslm) + " => (\n")
    fp.write("      x => tile_x(io_tile_id),\n")
    fp.write("      y => tile_y(io_tile_id),\n")
    fp.write("      haddr => fb_haddr,\n")
    fp.write("      hmask => fb_hmask\n")
    fp.write("    ),\n")
  fp.write("    others => tile_mem_info_none);\n\n")

  #
  fp.write("  -- I/O-bus devices routing info\n")
  fp.write("  constant apb_slv_y : yx_vec(0 to NAPBSLV - 1) := (\n")
  for i in range(0, NAPBS):
    if i > 0:
      fp.write(",\n")
    fp.write("    " + str(i) + " => tile_y(apb_tile_id(" + str(i) + "))")
  fp.write("  );\n")

  fp.write("  constant apb_slv_x : yx_vec(0 to NAPBSLV - 1) := (\n")
  for i in range(0, NAPBS):
    if i > 0:
      fp.write(",\n")
    fp.write("    " + str(i) + " => tile_x(apb_tile_id(" + str(i) + "))")
  fp.write("  );\n\n")

  #
  fp.write("  -- Flag I/O-bus slaves that are remote\n")
  fp.write("  -- Note that all components appear as remote to CPU and I/O tiles even if\n")
  fp.write("  -- located in that tile. This is because local masters still have to go\n")
  fp.write("  -- through ESP proxies to address such devices (e.g. L2 cache and DVFS\n")
  fp.write("  -- controller in CPU tiles). This choice allows any master in the SoC to\n")
  fp.write("  -- access these slaves. For instance, when configuring DVFS, a single CPU\n")
  fp.write("  -- must be able to access all DVFS controllers from other CPUS; another\n")
  fp.write("  -- example is the synchronized flush of all private caches, which is\n")
  fp.write("  -- initiated by a single CPU\n")
  fp.write("  constant remote_apb_slv_mask_cpu : std_logic_vector(0 to NAPBSLV - 1) := (\n")
  fp.write("    1 => '1',\n")
  fp.write("    2 => '1',\n")
  fp.write("    3 => '1',\n")
  fp.write("    13 => to_std_logic(CFG_SVGA_ENABLE),\n")
  fp.write("    14 => to_std_logic(CFG_GRETH),\n")
  if esp_config.has_sgmii:
    fp.write("    15 => to_std_logic(CFG_GRETH),\n")
  for j in range(0, esp_config.ndvfs):
    dvfs = esp_config.dvfs_ctrls[j]
    if dvfs.id != -1:
      fp.write("    " + str(dvfs.idx) + " => '1',\n")
  for j in range(0, esp_config.nl2):
    l2 = esp_config.l2s[j]
    if l2.idx != -1:
      fp.write("    " + str(l2.idx) + " => '1',\n")
  for j in range(0, esp_config.nllc):
    llc = esp_config.llcs[j]
    if llc.idx != -1:
      fp.write("    " + str(llc.idx) + " => '1',\n")
  for j in range(0, esp_config.ntiles):
    fp.write("    " + str(CSR_PINDEX[j]) + " => '1',\n")
  for j in range(0, esp_config.nacc):
    acc = esp_config.accelerators[j]
    fp.write("    " + str(acc.idx) + " => '1',\n")
  fp.write("    others => '0');\n\n")

  fp.write("  constant remote_apb_slv_mask_misc : std_logic_vector(0 to NAPBSLV - 1) := (\n")
  for j in range(0, esp_config.ndvfs):
    dvfs = esp_config.dvfs_ctrls[j]
    if dvfs.id != -1:
      fp.write("    " + str(dvfs.idx) + " => '1',\n")
  for j in range(0, esp_config.nl2):
    l2 = esp_config.l2s[j]
    if l2.idx != -1:
      fp.write("    " + str(l2.idx) + " => '1',\n")
  for j in range(0, esp_config.nllc):
    llc = esp_config.llcs[j]
    if llc.idx != -1:
      fp.write("    " + str(llc.idx) + " => '1',\n")
  for j in range(0, esp_config.ntiles):
    if esp_config.tiles[j].type != "misc":
      fp.write("    " + str(CSR_PINDEX[j]) + " => '1',\n")
  for j in range(0, esp_config.nacc):
    acc = esp_config.accelerators[j]
    fp.write("    " + str(acc.idx) + " => '1',\n")
  fp.write("    others => '0');\n")

  #
  fp.write("  -- Flag bus slaves that are remote to each tile (request selects slv proxy)\n")
  fp.write("  constant remote_ahb_mask_misc : std_logic_vector(0 to NAHBSLV - 1) := (\n")
  for j in range(0, esp_config.nmem):
    fp.write("    " + str(DDR_HINDEX[j]) + " => '1',\n")
  for j in range(0, esp_config.nslm):
    index = SLM_HINDEX + j
    if index >= 12:
      index = index + 1
    fp.write("    " + str(index) + " => '1',\n")
  fp.write("    others => '0');\n\n")

  fp.write("  constant remote_ahb_mask_cpu : std_logic_vector(0 to NAHBSLV - 1) := (\n")
  fp.write("    " + str(AHBROM_HINDEX) + "  => '1',\n")
  fp.write("    " + str(DDR_HINDEX[0]) + " => to_std_logic(CFG_L2_DISABLE),\n")
  fp.write("    " + str(FB_HINDEX) + "  => to_std_logic(CFG_SVGA_ENABLE),\n")
  fp.write("    others => '0');\n\n")

  fp.write("  constant slm_ahb_mask : std_logic_vector(0 to NAHBSLV - 1) := (\n")
  for j in range(0, esp_config.nslm):
    index = SLM_HINDEX + j
    if index >= 12:
      index = index + 1
    fp.write("    " + str(index) + " => '1',\n")
  fp.write("    others => '0');\n\n")

def main(argv):

  if len(sys.argv) != 5:
    sys.exit(1)

  root = Tk()

  DMA_WIDTH   = int(sys.argv[1])
  TECH        = sys.argv[2]
  LINUX_MAC   = sys.argv[3]
  LEON3_STACK = sys.argv[4]

  soc = soclib.SoC_Config(DMA_WIDTH, TECH, LINUX_MAC, LEON3_STACK, True)
  soc.noc = noclib.NoC()
  soc.read_config(False)

  esp_config = socgen.soc_config(soc)

  fp = open('socmap.vhd', 'w')

  print_header(fp, "socmap")
  print_libs(fp, False)

  fp.write("package socmap is\n\n")
  print_mapping(fp, soc, esp_config)
  print_tiles(fp, esp_config)

  fp.write("end socmap;\n")
  fp.close()

  print("Created configuration into 'socmap.vhd'")

if __name__ == "__main__":
  main(sys.argv)
