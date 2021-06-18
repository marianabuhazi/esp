#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import math
import sys
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *

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

def print_global_constants(fp, soc):
  fp.write("  ------ Global architecture parameters\n")
  fp.write("  constant ARCH_BITS : integer := " + str(soc.DMA_WIDTH) + ";\n")
  fp.write("  constant GLOB_MAXIOSLV : integer := " + str(socgen.NAPBS) + ";\n")
  # Keep cache-line size constant to 128 bits for now. We don't want huge line buffers
  fp.write("  constant GLOB_WORD_OFFSET_BITS : integer := " + str(int(math.log2(128/soc.DMA_WIDTH))) + ";\n")
  fp.write("  constant GLOB_BYTE_OFFSET_BITS : integer := " + str(int(math.log2(soc.DMA_WIDTH/8))) +";\n")
  fp.write("  constant GLOB_OFFSET_BITS : integer := GLOB_WORD_OFFSET_BITS + GLOB_BYTE_OFFSET_BITS;\n")
  fp.write("  constant GLOB_ADDR_INCR : integer := " + str(int(soc.DMA_WIDTH/8)) +";\n")
  # TODO: Keep physical address to 32 bits for now to reduce tag size. This will increase to support more memory
  fp.write("  constant GLOB_PHYS_ADDR_BITS : integer := " + str(32) +";\n")
  fp.write("  type cpu_arch_type is (leon3, ariane, ibex);\n")
  fp.write("  constant GLOB_CPU_ARCH : cpu_arch_type := " + soc.CPU_ARCH.get() + ";\n")
  if soc.CPU_ARCH.get() == "ariane":
    fp.write("  constant GLOB_CPU_AXI : integer range 0 to 1 := 1;\n")
  else:
    fp.write("  constant GLOB_CPU_AXI : integer range 0 to 1 := 0;\n")
  if soc.CPU_ARCH.get() == "leon3":
    fp.write("  constant GLOB_CPU_RISCV : integer range 0 to 1 := 0;\n")
  else:
    fp.write("  constant GLOB_CPU_RISCV : integer range 0 to 1 := 1;\n")
  fp.write("\n")
  # RTL caches
  if soc.cache_rtl.get() == 1:
    fp.write("  constant CFG_CACHE_RTL   : integer := 1;\n")
  else:
    fp.write("  constant CFG_CACHE_RTL   : integer := 0;\n\n")

def print_constants(fp, soc, esp_config):
  fp.write("  ------ NoC parameters\n")
  fp.write("  constant CFG_XLEN : integer := " + str(soc.noc.cols) + ";\n")
  fp.write("  constant CFG_YLEN : integer := " + str(soc.noc.rows) + ";\n")
  fp.write("  constant CFG_TILES_NUM : integer := CFG_XLEN * CFG_YLEN;\n")

  fp.write("  ------ DMA memory allocation (contiguous buffer or scatter/gather\n")
  fp.write("  constant CFG_SCATTER_GATHER : integer range 0 to 1 := " + str(soc.transfers.get()) + ";\n")

  fp.write("  constant CFG_L2_SETS     : integer := " + str(soc.l2_sets.get()      ) +  ";\n")
  fp.write("  constant CFG_L2_WAYS     : integer := " + str(soc.l2_ways.get()      ) +  ";\n")
  fp.write("  constant CFG_LLC_SETS    : integer := " + str(soc.llc_sets.get()     ) +  ";\n")
  fp.write("  constant CFG_LLC_WAYS    : integer := " + str(soc.llc_ways.get()     ) +  ";\n")
  fp.write("  constant CFG_ACC_L2_SETS : integer := " + str(soc.acc_l2_sets.get()  ) +  ";\n")
  fp.write("  constant CFG_ACC_L2_WAYS : integer := " + str(soc.acc_l2_ways.get()  ) +  ";\n")

  fp.write("  ------ Monitors enable (requires proFPGA MMI64)\n")
  fp.write("  constant CFG_MON_DDR_EN : integer := " + str(soc.noc.monitor_ddr.get()) + ";\n")
  fp.write("  constant CFG_MON_MEM_EN : integer := " + str(soc.noc.monitor_mem.get()) + ";\n")
  fp.write("  constant CFG_MON_NOC_INJECT_EN : integer := " + str(soc.noc.monitor_inj.get()) + ";\n")
  fp.write("  constant CFG_MON_NOC_QUEUES_EN : integer := " + str(soc.noc.monitor_routers.get()) + ";\n")
  fp.write("  constant CFG_MON_ACC_EN : integer := " + str(soc.noc.monitor_accelerators.get()) + ";\n")
  fp.write("  constant CFG_MON_L2_EN : integer := " + str(soc.noc.monitor_l2.get()) + ";\n")
  fp.write("  constant CFG_MON_LLC_EN : integer := " + str(soc.noc.monitor_llc.get()) + ";\n")
  fp.write("  constant CFG_MON_DVFS_EN : integer := " + str(soc.noc.monitor_dvfs.get()) + ";\n\n")

  fp.write("  ------ Coherence enabled\n")
  if esp_config.coherence:
    fp.write("  constant CFG_L2_ENABLE   : integer := 1;\n")
    fp.write("  constant CFG_L2_DISABLE  : integer := 0;\n")
    fp.write("  constant CFG_LLC_ENABLE  : integer := 1;\n\n")
  else:
    fp.write("  constant CFG_L2_ENABLE   : integer := 0;\n")
    fp.write("  constant CFG_L2_DISABLE  : integer := 1;\n")
    fp.write("  constant CFG_LLC_ENABLE  : integer := 0;\n\n")

  #
  fp.write("  ------ Number of components\n")
  fp.write("  constant CFG_NCPU_TILE : integer := " + str(esp_config.ncpu) + ";\n")
  fp.write("  constant CFG_NMEM_TILE : integer := " + str(esp_config.nmem) + ";\n")
  fp.write("  constant CFG_NSLM_TILE : integer := " + str(esp_config.nslm) + ";\n")
  fp.write("  constant CFG_NL2 : integer := " + str(esp_config.nl2) + ";\n")
  fp.write("  constant CFG_NLLC : integer := " + str(esp_config.nllc) + ";\n")
  fp.write("  constant CFG_NLLC_COHERENT : integer := " + str(esp_config.ncdma) + ";\n")
  fp.write("  constant CFG_SLM_KBYTES : integer := " + str(esp_config.slm_kbytes) + ";\n\n")

  #
  fp.write("  ------ Local-port Synchronizers are always present)\n")
  fp.write("  constant CFG_HAS_SYNC : integer := 1;\n")
  if esp_config.has_dvfs:
    fp.write("  constant CFG_HAS_DVFS : integer := 1;\n")
  else:
    fp.write("  constant CFG_HAS_DVFS : integer := 0;\n")
  fp.write("\n")

  #
  # private cache physical interrupt line
  L2_CACHE_PIRQ = 3

  # Last-level cache physical interrupt line
  LLC_CACHE_PIRQ = 4

  fp.write("  ------ Caches interrupt line\n")
  fp.write("  constant CFG_SLD_LLC_CACHE_IRQ : integer := " + str(LLC_CACHE_PIRQ) + ";\n\n")
  fp.write("  constant CFG_SLD_L2_CACHE_IRQ : integer := " + str(L2_CACHE_PIRQ) + ";\n\n")

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

  fp = open('esp_global.vhd', 'w')

  print_header(fp, "esp_global")
  print_libs(fp, True)

  fp.write("package esp_global is\n\n")
  print_global_constants(fp, soc)
  print_constants(fp, soc, esp_config)

  fp.write("end esp_global;\n")
  fp.close()

  print("Created global constants definition into 'esp_global.vhd'")

if __name__ == "__main__":
  main(sys.argv)
