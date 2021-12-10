from thirdparty import *
# Maximum number of AHB and APB slaves can also be increased, but Leon3 utility
# mklinuximg, GRLIB AMBA package and bare-metal probe constants must be updated.
# The related constants are defined in
# <mklinuximg>/include/ambapp.h
# <esp>/rtl/include/grlib/amba/amba.vhd
# <esp>/soft/leon3/include/esp_probe.h
NAPBS = 128
NAHBS = 16
# Physical interrupt lines
IRQ_LINES = 32
# Maximum number of components is not an actual limitation in ESP
# We simply set some value consistently with RTL, but these limits
# can be increased by editing nocpackage accordingly.
# <esp>/rtl/include/sld/noc/nocpackage.vhd
NCPU_MAX = 4
NMEM_MAX = 4
NSLM_MAX = 16
# Caches are provisioning 4 bits for IDs and 16 bits for sharers
# This can be changed here:
# <esp>/systemc/common/caches/cache_consts.h
# <esp>/rtl/include/sld/caches/cachepackage.vhd
NFULL_COHERENT_MAX = 16
NLLC_COHERENT_MAX = 64
# The NoC routers are using 3 bits for both Y and X coordinates.
# The 34-bits header can host up to 5 bits if necessary.
# <esp>/rtl/[include|src]/sld/noc/*.vhd
NTILE_MAX = 64
# The number of accelerators depends on how many I/O devices can be addressed.
# This can be changed by updating the constant NAPBS as explained above, as well
# as the corresponding constant in
# <esp>/rtl/include/sld/noc/nocpackage.vhd
# The following indices  are reserved:
# 0 - BOOT ROM memory controller
# 1 - UART
# 2 - Interrupt controller
# 3 - Timer
# 4 - ESPLink
# 5-8 - DVFS controller
# 9-12 - Processors' private cache controller (must change with NCPU_MAX)
# 13 - SVGA controller
# 14 - Ethernet MAC controller
# 15 - Ethernet SGMII PHY controller
# 16-19 - LLC cache controller (must change with NMEM_MAX)
# 20-83 - Distributed monitors (equal to the number of tiles NTILE_MAX)
# 84-(NAPBS-1) - Accelerators
NACC_MAX = NAPBS - 2 * NCPU_MAX - NMEM_MAX - NTILE_MAX - 8


# Default device mapping
RST_ADDR = dict()
RST_ADDR["leon3"] = 0x0
RST_ADDR["ariane"] = 0x10000
RST_ADDR["ibex"] = 0x80

# Default start address for device tree
RODATA_ADDR = dict()
RODATA_ADDR["leon3"] = 0x0
RODATA_ADDR["ariane"] = 0x10400
RODATA_ADDR["ibex"] = 0x500

# Boot ROM slave index (With Leon3 this exists in simulation only for now)
AHBROM_HINDEX = 0

# Memory-mapped registers slave index
AHB2APB_HINDEX = 1

# Memory-mapped registers base address (includes peripherals and accelerators)
AHB2APB_HADDR = dict()
AHB2APB_HADDR["leon3"] = 0x800
AHB2APB_HADDR["ariane"] = 0x600
AHB2APB_HADDR["ibex"] = 0x600

# RISC-V CPU Local Interruptor index
RISCV_CLINT_HINDEX = 2

# Memory controller slave index
DDR_HINDEX = [4, 5, 6, 7]

# First shared-local memory slave index
SLM_HINDEX = 8

# Main memory area (12 MSBs)
DDR_HADDR = dict()
DDR_HADDR["leon3"] = 0x400
DDR_HADDR["ariane"] = 0x800
DDR_HADDR["ibex"] = 0x800

# SLM base address
SLM_HADDR = 0x040
SLMDDR_HADDR = 0xC00

# Main memory size (12 MSBs)
DDR_SIZE = 0x400

# Frame-buffer index
FB_HINDEX = 12

# CPU tile power manager I/O-bus slave index
DVFS_PINDEX = [5, 6, 7, 8]

# private cache physical interrupt line
L2_CACHE_PIRQ = 3

# Private cache I/O-bus slave indices (more indices can be reserved if necessary)
L2_CACHE_PINDEX = [9, 10, 11, 12]

# Last-level cache physical interrupt line
LLC_CACHE_PIRQ = 4

# Last-level cache I/O-bus slave indices (more indices can be reserved if necessary)
LLC_CACHE_PINDEX = [16, 17, 18, 19]

# ESP Tile CSRs APB indices
CSR_PINDEX = list(range(20, 20 + NTILE_MAX))

# I/O memory area offset for CSRs
CSR_APB_ADDR = 0x900
CSR_APB_ADDR_MSK = 0xffe

# First I/O-bus index for accelerators
SLD_APB_PINDEX = 20 + NTILE_MAX

# I/O memory area offset for accelerators (address bits 19-8)
SLD_APB_ADDR = 0x100

# default mask for accelerators' registers base address (256 Bytes regions per accelerator)
SLD_APB_ADDR_MSK = 0xfff

###########
# Constants for third-party accelerators

# third-party accelerators counter
THIRDPARTY_N = 0

# third-party APB address and mask
# If APB EXT ADDR SIZE is not zero, then APB mask will be applied to
# the extended address. Each device instance will reserve EXT SIZE
# bytes in the address space, even if a signle instance would take
# less. This is to simplify (hence speedup) APB decode.
# APB EXT ADDR most significant hex digit (i.e. digit 7) must be 0
THIRDPARTY_APB_ADDRESS          = 0x00000000
THIRDPARTY_APB_ADDRESS_SIZE     = 0x00040000
THIRDPARTY_APB_EXT_ADDRESS      = 0x00400000
THIRDPARTY_APB_EXT_ADDRESS_SIZE = 0x00100000

# Memory reserved for accelerators
ACC_MEM_RESERVED_START_ADDR = 0xA0200000
ACC_MEM_RESERVED_TOTAL_SIZE = 0x1FE00000
THIRDPARTY_MEM_RESERVED_ADDR = 0xB0000000
THIRDPARTY_MEM_RESERVED_SIZE = 0x10000000
