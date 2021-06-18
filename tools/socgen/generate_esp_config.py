#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import sys, os, shutil
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *

def write_config(soc):
  print("Writing backup configuration: \".esp_config-new.bak\"")
  fp = open('.esp_config-new.bak', 'w')
  #fp = open('.testing', 'w')
  has_dvfs = False;
  fp.write("CPU_ARCH = " + soc.CPU_ARCH.get() + "\n")
  fp.write("NCPU_TILE = " + str(soc.noc.get_cpu_num(soc)) + "\n")
  if soc.transfers.get() == 1:
    fp.write("CONFIG_HAS_SG = y\n")
  else:
    fp.write("#CONFIG_HAS_SG is not set\n")
  fp.write("CONFIG_NOC_ROWS = " + str(soc.noc.rows) + "\n")
  fp.write("CONFIG_NOC_COLS = " + str(soc.noc.cols) + "\n")
  if soc.cache_en.get() == 1:
    fp.write("CONFIG_CACHE_EN = y\n")
  else:
    fp.write("#CONFIG_CACHE_EN is not set\n")
  if soc.cache_rtl.get() == 1:
    fp.write("CONFIG_CACHE_RTL = y\n")
  else:
    fp.write("#CONFIG_CACHE_RTL is not set\n")
  fp.write("CONFIG_CPU_CACHES = " + str(soc.l2_sets.get()) + " " + str(soc.l2_ways.get()) + " " + str(soc.llc_sets.get()) + " " + str(soc.llc_ways.get()) + "\n")
  fp.write("CONFIG_ACC_CACHES = " + str(soc.acc_l2_sets.get()) + " " + str(soc.acc_l2_ways.get()) + "\n")
  fp.write("CONFIG_SLM_KBYTES = " + str(soc.slm_kbytes.get()) + "\n")
  if soc.noc.monitor_ddr.get() == 1:
    fp.write("CONFIG_MON_DDR = y\n")
  else:
    fp.write("#CONFIG_MON_DDR is not set\n")
  if soc.noc.monitor_mem.get() == 1:
    fp.write("CONFIG_MON_MEM = y\n")
  else:
    fp.write("#CONFIG_MON_MEM is not set\n")
  if soc.noc.monitor_inj.get() == 1:
    fp.write("CONFIG_MON_INJ = y\n")
  else:
    fp.write("#CONFIG_MON_INJ is not set\n")
  if soc.noc.monitor_routers.get() == 1:
    fp.write("CONFIG_MON_ROUTERS = y\n")
  else:
    fp.write("#CONFIG_MON_ROUTERS is not set\n")
  if soc.noc.monitor_accelerators.get() == 1:
    fp.write("CONFIG_MON_ACCELERATORS = y\n")
  else:
    fp.write("#CONFIG_MON_ACCELERATORS is not set\n")
  if soc.noc.monitor_l2.get() == 1:
    fp.write("CONFIG_MON_L2 = y\n")
  else:
    fp.write("#CONFIG_MON_L2 is not set\n")
  if soc.noc.monitor_llc.get() == 1:
    fp.write("CONFIG_MON_LLC = y\n")
  else:
    fp.write("#CONFIG_MON_LLC is not set\n")
  if soc.noc.monitor_dvfs.get() == 1:
    fp.write("CONFIG_MON_DVFS = y\n")
  else:
    fp.write("#CONFIG_MON_DVFS is not set\n")
  i = 0
  for y in range(0, soc.noc.rows):
    for x in range(0, soc.noc.cols):
      tile = soc.noc.topology[y][x]
      selection = tile.ip_type.get()
      is_accelerator = False
      fp.write("TILE_" + str(y) + "_" + str(x) + " = ")
      # Tile number
      fp.write(str(i) + " ")
      # Tile type
      if soc.IPs.PROCESSORS.count(selection):
        fp.write("cpu")
      elif soc.IPs.MISC.count(selection):
        fp.write("misc")
      elif soc.IPs.MEM.count(selection):
        fp.write("mem")
      elif soc.IPs.SLM.count(selection):
        fp.write("slm")
      elif soc.IPs.ACCELERATORS.count(selection):
        is_accelerator = True
        fp.write("acc")
      else:
        fp.write("empty")
      # Selected accelerator or tile type repeated
      fp.write(" " + selection)
      # Clock region info
      try:
        clk_region = tile.clk_region.get()
        fp.write(" " + str(clk_region))
        if clk_region != 0:
          has_dvfs = True;
      except:
        fp.write(" " + str(0))
      fp.write(" " + str(tile.has_pll.get()))
      fp.write(" " + str(tile.has_clkbuf.get()))
      if is_accelerator:
        fp.write(" " + str(tile.point.get()))
        fp.write(" " + str(tile.has_l2.get()))
        fp.write(" " + str(tile.vendor))
      fp.write("\n")
      i += 1
  if has_dvfs:
    fp.write("CONFIG_HAS_DVFS = y\n")
  else:
    fp.write("#CONFIG_HAS_DVFS is not set\n")
  fp.write("CONFIG_VF_POINTS = " + str(soc.noc.vf_points) + "\n")
  for y in range(soc.noc.rows):
    for x in range(soc.noc.cols):
      tile = soc.noc.topology[y][x]
      selection = tile.ip_type.get()
      fp.write("POWER_" + str(y) + "_" + str(x) + " = ")
      fp.write(selection + " ")
      if soc.IPs.ACCELERATORS.count(selection) == 0:
        for vf in range(soc.noc.vf_points):
          fp.write(str(0) + " " + str(0) + " " + str(0) + " ")
        fp.write("\n")
      else:
        for vf in range(soc.noc.vf_points):
          fp.write(str(tile.energy_values.vf_points[vf].voltage) + " " + str(tile.energy_values.vf_points[vf].frequency) + " " + str(tile.energy_values.vf_points[vf].energy) + " ")
        fp.write("\n")
  fp.close()

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
  write_config(soc)

  if os.path.isfile(".esp_config-new.bak") == True:
    shutil.copy2(".esp_config-new.bak", ".esp_config-new")

if __name__ == "__main__":
  main(sys.argv)
