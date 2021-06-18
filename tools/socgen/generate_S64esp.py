#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import sys
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *
from constants import *

def print_load_script(fp, soc, esp_config):
  fp.write("cd /opt/drivers-esp\n")
  fp.write("insmod contig_alloc.ko ")
  nmem = esp_config.nmem
  ddr_size = int(str(DDR_SIZE)) * int(0x100000)
  if nmem > 0:
    size = int(ddr_size / nmem)
  sizes = []
  starts = []
  start = int(str(DDR_HADDR[soc.CPU_ARCH.get()])) * int(0x100000)
  nddr = 0
  line_size = int(0x10)
  
  end = start + ddr_size
  if soc.CPU_ARCH.get() == "ariane" or soc.CPU_ARCH.get() == "ibex":
    sp = int(0xa0200000) - line_size 
    if esp_config.nthirdparty > 0:        
        end = int(0xb0000000)
  else: 
    sp = int(str(soc.LEON3_STACK), 16)

  addr = start
  for _ in range(nmem):
    if addr >= (sp + line_size) and addr < end:
        starts.append(hex(addr))
        if addr + size <= end: 
            sizes.append(hex(size))
        else:
            size.append(hex(end - addr))
        nddr += 1
    elif (addr + size) > (sp + line_size) and addr < end:
        starts.append(hex(sp + line_size))
        if addr + size <= end:
            sizes.append(hex((addr + size) - (sp + line_size)))
        else:
            sizes.append(hex(end - (sp + line_size)))
        nddr += 1
    addr += size

  fp.write("nddr=" + str(nddr) + " ")
  fp.write("start=")
  for i in range(nddr):
    fp.write(starts[i])
    if i != nddr - 1:
        fp.write(",")
            
  fp.write(" size=")
  for i in range(nddr):
    fp.write(sizes[i])
    if i != nddr - 1:
        fp.write(",")
    
  fp.write(" chunk_log=20\n")
  fp.write("insmod esp_cache.ko\n")
  fp.write("insmod esp_private_cache.ko\n")
  fp.write("insmod esp.ko")
  fp.write(" line_bytes=16")
  fp.write(" l2_sets=" + str(soc.l2_sets.get()))
  fp.write(" l2_ways=" + str(soc.l2_ways.get()))
  fp.write(" llc_sets=" + str(soc.llc_sets.get()))
  fp.write(" llc_ways=" + str(soc.llc_ways.get()))
  fp.write(" llc_banks=" + str(nmem))
  fp.write(" rtl_cache=" + str(soc.cache_rtl.get()))
  
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

  fp = open('S64esp', 'w')
  print_load_script(fp, soc, esp_config)
  fp.close()
  print("Created kernel module load script into 'S64esp'")

if __name__ == "__main__":
  main(sys.argv)
