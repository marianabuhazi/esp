#!/usr/bin/env python3

# Copyright (c) 2011-2021 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0

import sys
import soc as soclib
import socmap_gen as socgen
import NoCConfiguration as noclib

from tkinter import *
from constants import *

def print_devtree(fp, esp_config):
  # Get CPU base frequency
  with open("../../top.vhd") as top_fp:
    for line in top_fp:
      if line.find("constant CPU_FREQ : integer") != -1:
        line.strip();
        items = line.split()
        CPU_FREQ = int(items[5].replace(";",""))
        top_fp.close()
        break

  fp.write("/dts-v1/;\n")
  fp.write("\n")
  fp.write("/ {\n")
  fp.write("  #address-cells = <2>;\n")
  fp.write("  #size-cells = <2>;\n")
  if esp_config.cpu_arch == "ariane":
    fp.write("  compatible = \"eth,ariane-bare-dev\";\n")
    fp.write("  model = \"eth,ariane-bare\";\n")
  elif esp_config.cpu_arch == "ibex":
    fp.write("  compatible = \"lowrisc,ibex\";\n")
    fp.write("  model = \"lowrisc,ibex-small\";\n")
  fp.write("  chosen {\n")
  fp.write("    stdout-path = \"/soc/uart@" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "00100:38400\";\n")
  fp.write("  };\n")
  fp.write("  cpus {\n")
  fp.write("    #address-cells = <1>;\n")
  fp.write("    #size-cells = <0>;\n")
  fp.write("    timebase-frequency = <" + str(int((CPU_FREQ * 1000) / 2)) + ">; // CPU_FREQ / 2\n")
  for i in range(esp_config.ncpu):
    fp.write("    CPU" + str(i) + ": cpu@" + str(i) + " {\n")
    fp.write("      clock-frequency = <" + str(CPU_FREQ) + "000>;\n")
    fp.write("      device_type = \"cpu\";\n")
    fp.write("      reg = <" + str(i) + ">;\n")
    fp.write("      status = \"okay\";\n")
    if esp_config.cpu_arch == "ariane":
      fp.write("      compatible = \"eth, ariane\", \"riscv\";\n")
      fp.write("      riscv,isa = \"rv64imafdc\";\n")
      fp.write("      mmu-type = \"riscv,sv39\";\n")
      fp.write("      tlb-split;\n")
    elif esp_config.cpu_arch == "ibex":
      fp.write("      compatible = \"lowrisc, ibex\", \"riscv\";\n")
      fp.write("      riscv,isa = \"rv32imc\";\n")
    fp.write("      // HLIC - hart local interrupt controller\n")
    fp.write("      CPU" + str(i) + "_intc: interrupt-controller {\n")
    fp.write("        #interrupt-cells = <1>;\n")
    fp.write("        interrupt-controller;\n")
    fp.write("        compatible = \"riscv,cpu-intc\";\n")
    fp.write("      };\n")
    fp.write("    };\n")
  fp.write("  };\n")
  fp.write("  memory@" + format(DDR_HADDR[esp_config.cpu_arch], '03x') + "00000 {\n")
  fp.write("    device_type = \"memory\";\n")
  # TODO: increase memory address space.
  if esp_config.nmem != 0:
    fp.write("    reg = <0x0 0x" + format(DDR_HADDR[esp_config.cpu_arch], '03x') + "00000 0x0 0x20000000>;\n")
  else:
    # Use SLM as main memory
    fp.write("    reg = <0x0 0x" + format(DDR_HADDR[esp_config.cpu_arch], '03x') + "00000 0x0 0x" + format(esp_config.slm_tot_kbytes * 1024, '08x') + ">;\n")
  fp.write("  };\n")

  # OS reserved memory regions
  if esp_config.nmem != 0:
    fp.write("  reserved-memory {\n")
    fp.write("    #address-cells = <2>;\n")
    fp.write("    #size-cells = <2>;\n")
    fp.write("    ranges;\n")
    fp.write("\n")
    fp.write("    greth_reserved: buffer@a0000000 {\n")
    fp.write("      compatible = \"shared-dma-pool\";\n")
    fp.write("      no-map;\n")
    fp.write("      reg = <0x0 0xa0000000 0x0 0x200000>;\n")
    fp.write("    };\n")

    # Add only one memory region for all third-party accelerator instances
    acc_mem_address = format(ACC_MEM_RESERVED_START_ADDR, "08x")
    acc_mem_size = format(ACC_MEM_RESERVED_TOTAL_SIZE, "08x")
    if esp_config.nacc > esp_config.nthirdparty:
      tp_mem_address = format(THIRDPARTY_MEM_RESERVED_ADDR, "08x")
      tp_mem_size = format(THIRDPARTY_MEM_RESERVED_SIZE, "08x")
    else:
      tp_mem_address = format(ACC_MEM_RESERVED_START_ADDR, "08x")
      tp_mem_size = format(ACC_MEM_RESERVED_TOTAL_SIZE, "08x")

    if esp_config.nthirdparty > 0:
      acc_mem_size = format(ACC_MEM_RESERVED_TOTAL_SIZE - THIRDPARTY_MEM_RESERVED_SIZE, "08x")

    if esp_config.nacc > esp_config.nthirdparty:
      fp.write("\n")
      fp.write("    accelerator_reserved: buffer@" + acc_mem_address + " {\n")
      fp.write("      compatible = \"shared-dma-pool\";\n")
      fp.write("      no-map;\n")
      fp.write("      reg = <0x0 0x" + acc_mem_address + " 0x0 0x" + acc_mem_size + ">;\n")
      fp.write("    };\n")
    if esp_config.nthirdparty > 0:
      fp.write("\n")
      fp.write("    thirdparty_reserved: buffer@" + tp_mem_address + " {\n")
      fp.write("      compatible = \"shared-dma-pool\";\n")
      fp.write("      no-map;\n")
      fp.write("      reg = <0x0 0x" + tp_mem_address + " 0x0 0x" + tp_mem_size + ">;\n")
      fp.write("    };\n")
    fp.write("  };\n")

  fp.write("  L26: soc {\n")
  fp.write("    #address-cells = <2>;\n")
  fp.write("    #size-cells = <2>;\n")
  if esp_config.cpu_arch == "ariane":
    fp.write("    compatible = \"eth,ariane-bare-soc\", \"simple-bus\";\n")
  elif esp_config.cpu_arch == "ibex":
    fp.write("    compatible = \"lowrisc,lowrisc-soc\", \"simple-bus\";\n")
  fp.write("    ranges;\n")
  # TODO: make clint/plic remote devices w/ remote AXI proxy and variable address to be passed over
  fp.write("    clint@2000000 {\n")
  fp.write("      compatible = \"riscv,clint0\";\n")
  fp.write("      interrupts-extended = <\n")
  for i in range(esp_config.ncpu):
    fp.write("                             &CPU" + str(i) + "_intc 3 &CPU" + str(i) + "_intc 7\n")
  fp.write("                            >;\n")
  fp.write("      reg = <0x0 0x2000000 0x0 0xc0000>;\n")
  fp.write("      reg-names = \"control\";\n")
  fp.write("    };\n")
  fp.write("    PLIC0: interrupt-controller@6c000000 {\n")
  fp.write("      #address-cells = <0>;\n")
  fp.write("      #interrupt-cells = <1>;\n")
  fp.write("      compatible = \"riscv,plic0\";\n")
  fp.write("      interrupt-controller;\n")
  fp.write("      interrupts-extended = <\n")
  for i in range(esp_config.ncpu):
    fp.write("                             &CPU" + str(i) + "_intc 11 &CPU" + str(i) + "_intc 9\n")
  fp.write("                            >;\n")
  fp.write("      reg = <0x0 0x6c000000 0x0 0x4000000>;\n")
  fp.write("      riscv,max-priority = <7>;\n")
  fp.write("      riscv,ndev = <16>;\n")
  fp.write("    };\n")
  # TODO add GPTIMER/Accelerators/Caches/SVGA/DVFS to devtree (and remove leon3 IRQ from socmap
  fp.write("    uart@" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "00100 {\n")
  fp.write("      compatible = \"gaisler,apbuart\";\n")
  fp.write("      reg = <0x0 0x" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "00100 0x0 0x100>;\n")
  fp.write("      freq = <" + str(CPU_FREQ) + "000>;\n")
  fp.write("      interrupt-parent = <&PLIC0>;\n")
  fp.write("      interrupts = <3>;\n")
  fp.write("      reg-shift = <2>; // regs are spaced on 32 bit boundary\n")
  fp.write("      reg-io-width = <4>; // only 32-bit access are supported\n")
  fp.write("    };\n")
  fp.write("    eth: greth@" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "80000 {\n")
  fp.write("      #address-cells = <1>;\n")
  fp.write("      #size-cells = <1>;\n")
  fp.write("      compatible = \"gaisler,ethmac\";\n")
  fp.write("      device_type = \"network\";\n")
  fp.write("      interrupt-parent = <&PLIC0>;\n")
  fp.write("      interrupts = <13 0>;\n")
  # Use randomly generated MAC address
  mac = " ".join(esp_config.linux_mac[i:i+2] for i in range(0, len(esp_config.linux_mac), 2))
  fp.write("      local-mac-address = [" + mac + "];\n")
  fp.write("      reg = <0x0 0x" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "80000 0x0 0x10000>;\n")
  fp.write("      phy-handle = <&phy0>;\n")
  fp.write("      phy-connection-type = \"sgmii\";\n")
  if esp_config.nmem != 0:
    fp.write("      memory-region = <&greth_reserved>;\n")
  fp.write("\n")
  fp.write("      phy0: mdio@60001000 {\n")
  fp.write("            #address-cells = <1>;\n")
  fp.write("            #size-cells = <0>;\n")
  fp.write("            compatible = \"gaisler,sgmii\";\n")
  fp.write("            reg = <0x0 0x" + format(AHB2APB_HADDR[esp_config.cpu_arch], '03x') + "01000 0x0 0x1000>;\n")
  fp.write("            interrupt-parent = <&PLIC0>;\n")
  fp.write("            interrupts = <12 0>;\n")
  fp.write("      };\n")
  fp.write("    };\n")

  # ESP L2 caches
  base = AHB2APB_HADDR[esp_config.cpu_arch] << 20
  for i in range(esp_config.nl2):
    l2 = esp_config.l2s[i]
    if l2.idx != -1:
      address = base + 0xD000 + (l2.idx << 8)
      address_str = format(address, "x")
      size_str = "100"
      fp.write("    espl2cache" + str(l2.id) + "@" + address_str + " {\n")
      fp.write("      compatible = \"sld,l2_cache\";\n")
      fp.write("      reg = <0x0 0x" + address_str + " 0x0 0x" + size_str + ">;\n")
      fp.write("      reg-shift = <2>; // regs are spaced on 32 bit boundary\n")
      fp.write("      reg-io-width = <4>; // only 32-bit access are supported\n")
      fp.write("    };\n")

  # ESP LLC caches
  base = AHB2APB_HADDR[esp_config.cpu_arch] << 20
  for i in range(esp_config.nllc):
    llc = esp_config.llcs[i]
    if llc.idx != -1:
      address = base + 0xD000 + (llc.idx << 8)
      address_str = format(address, "x")
      size_str = "100"
      fp.write("    espllccache" + str(llc.id) + "@" + address_str + " {\n")
      fp.write("      compatible = \"sld,llc_cache\";\n")
      fp.write("      reg = <0x0 0x" + address_str + " 0x0 0x" + size_str + ">;\n")
      fp.write("      reg-shift = <2>; // regs are spaced on 32 bit boundary\n")
      fp.write("      reg-io-width = <4>; // only 32-bit access are supported\n")
      fp.write("    };\n")

  # Reset all THIRDPARTY accelerators counters
  THIRDPARTY_N = 0
  
  for i in range(esp_config.nacc):
    acc = esp_config.accelerators[i]
    base = AHB2APB_HADDR[esp_config.cpu_arch] << 20
    if acc.vendor == "sld":
      address = base + ((SLD_APB_ADDR + acc.id) << 8)
      size = 0x100
    else:
      n = THIRDPARTY_N
      # Compute base address
      if THIRDPARTY_APB_EXT_ADDRESS == 0:
        # Use part of standard APB address space
        address = base + THIRDPARTY_APB_ADDRESS + n * THIRDPARTY_APB_ADDRESS_SIZE
        size = THIRDPARTY_APB_ADDRESS_SIZE
      else:
        # Use extended APB address space (large number of registers)
        address = base + THIRDPARTY_APB_EXT_ADDRESS + n * THIRDPARTY_APB_EXT_ADDRESS_SIZE
        size = THIRDPARTY_APB_EXT_ADDRESS_SIZE

      # Increment count
      THIRDPARTY_N = n + 1;

    address_str = format(address, "x")
    size_str = format(size, "x")

    fp.write("    " + acc.lowercase_name + "@" + address_str + " {\n")
    if acc.vendor == "sld":
      fp.write("      compatible = \"" + acc.vendor + "," + acc.lowercase_name + "\";\n")
    else:
      fp.write("      compatible = \"" + acc.vendor + "," + THIRDPARTY_COMPATIBLE[acc.lowercase_name] + "\";\n")
    fp.write("      reg = <0x0 0x" + address_str + " 0x0 0x" + size_str + ">;\n")
    fp.write("      interrupt-parent = <&PLIC0>;\n")
    fp.write("      interrupts = <" + str(acc.irq + 1) + ">;\n")
    fp.write("      reg-shift = <2>; // regs are spaced on 32 bit boundary\n")
    fp.write("      reg-io-width = <4>; // only 32-bit access are supported\n")
    if acc.vendor != "sld" and esp_config.nmem != 0:
      fp.write("      memory-region = <&thirdparty_reserved>;\n")
    fp.write("    };\n")
  fp.write("  };\n")
  fp.write("};\n")
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

  esp_config = socgen.soc_config(soc)

  fp = open('riscv-new.dts', 'w')
  print_devtree(fp, esp_config)
  fp.close()
  print("Created device-tree into 'riscv-new.dts'")

if __name__ == "__main__":
  main(sys.argv)
