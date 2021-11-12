#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import sys
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *
from constants import *

def print_esplink_header(fp, esp_config, soc):

  # Get CPU base frequency
  with open("../../top.vhd") as top_fp:
    for line in top_fp:
      if line.find("constant CPU_FREQ : integer") != -1:
        line.strip();
        items = line.split()
        CPU_FREQ = 1000 * int(items[5].replace(";",""))
        top_fp.close()
        break

  fp.write("#ifndef __SOCMAP_H__\n")
  fp.write("#define __SOCMAP_H__\n")
  fp.write("\n")
  fp.write("#define EDCL_IP \"" + soc.IP_ADDR + "\"\n")
  fp.write("#define BASE_FREQ " + str(CPU_FREQ) + "\n")
  if soc.cache_spandex.get() == 1:
    fp.write("#define USE_SPANDEX\n")
  fp.write("#define BOOTROM_BASE_ADDR " + hex(RST_ADDR[esp_config.cpu_arch]) + "\n")
  fp.write("#define RODATA_START_ADDR " + hex(RODATA_ADDR[esp_config.cpu_arch]) + "\n")
  fp.write("#define DRAM_BASE_ADDR 0x" + format(DDR_HADDR[esp_config.cpu_arch], '03X') + "00000\n")
  if esp_config.nmem == 0:
    fp.write("#define OVERRIDE_DRAM_SIZE 0x" + format(esp_config.slm_tot_kbytes * 1024, '08X') + "\n")
  fp.write("#define ESPLINK_BASE_ADDR 0x" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03X') + "00400\n")
  fp.write("#define TARGET_BYTE_ORDER __ORDER_BIG_ENDIAN__\n")
  fp.write("\n")
  fp.write("#endif /* __SOCMAP_H__ */\n")

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

  fp = open('socmap.h', 'w')

  print_esplink_header(fp, esp_config, soc)

  fp.close()
  
  print("Created ESPLink header into 'socmap.h'")

if __name__ == "__main__":
  main(sys.argv)
