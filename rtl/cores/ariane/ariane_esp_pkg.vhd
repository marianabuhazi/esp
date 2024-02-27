-- Copyright (c) 2011-2023 Columbia University, System Level Design Group
-- SPDX-License-Identifier: Apache-2.0

library ieee;
  use ieee.std_logic_1164.all;
  use work.esp_global.all;
  use work.amba.all;

package ariane_esp_pkg is

  component ariane_axi_wrap is
    generic (
      nmst             : integer;
      nslv             : integer;
      rombase          : std_logic_vector(63 downto 0);
      romlength        : std_logic_vector(63 downto 0);
      apbbase          : std_logic_vector(63 downto 0);
      apblength        : std_logic_vector(63 downto 0);
      clintbase        : std_logic_vector(63 downto 0);
      clintlength      : std_logic_vector(63 downto 0);
      slmbase          : std_logic_vector(63 downto 0);
      slmlength        : std_logic_vector(63 downto 0);
      slmddrbase       : std_logic_vector(63 downto 0);
      slmddrlength     : std_logic_vector(63 downto 0);
      drambase         : std_logic_vector(63 downto 0);
      dramlength       : std_logic_vector(63 downto 0);
      dramcachedlength : std_logic_vector(63 downto 0)
    );
    port (
      clk        : in    std_logic;
      rstn       : in    std_logic;
      hart_id    : in    std_logic_vector(63 downto 0);
      irq        : in    std_logic_vector(1 downto 0);
      timer_irq  : in    std_logic;
      ipi        : in    std_logic;
      romi       : out   axi_mosi_type;
      romo       : in    axi_somi_type;
      drami      : out   axi_mosi_type;
      dramo      : in    axi_somi_type;
      clinti     : out   axi_mosi_type;
      clinto     : in    axi_somi_type;
      slmi       : out   axi_mosi_type;
      slmo       : in    axi_somi_type;
      slmddri    : out   axi_mosi_type;
      slmddro    : in    axi_somi_type;
      ace_req    : in    ace_req_type;
      ace_resp   : out   ace_resp_type;
      apbi       : out   apb_slv_in_type;
      apbo       : in    apb_slv_out_vector;
      apb_req    : out   std_ulogic;
      apb_ack    : in    std_ulogic;
      fence_l2   : out   std_logic_vector(1 downto 0);
      flush_l1   : in    std_logic;
      flush_done : out   std_logic
    );
  end component ariane_axi_wrap;

  component riscv_plic_apb_wrap is
    generic (
      pindex    : integer;
      pconfig   : apb_config_type;
      nharts    : integer;
      nirq_srcs : integer
    );
    port (
      clk         : in    std_ulogic;
      rstn        : in    std_ulogic;
      irq_sources : in    std_logic_vector(NIRQ_SRCS - 1 downto 0);
      irq         : out   std_logic_vector(NHARTS * 2 - 1 downto 0);
      apbi        : in    apb_slv_in_type;
      apbo        : out   apb_slv_out_type;
      pready      : out   std_ulogic;
      pslverr     : out   std_ulogic
    );
  end component riscv_plic_apb_wrap;

  component riscv_clint_ahb_wrap is
    generic (
      hindex  : integer;
      hconfig : ahb_config_type;
      nharts  : integer
    );
    port (
      clk       : in    std_ulogic;
      rstn      : in    std_ulogic;
      timer_irq : out   std_logic_vector(NHARTS - 1 downto 0);
      ipi       : out   std_logic_vector(NHARTS - 1 downto 0);
      ahbsi     : in    ahb_slv_in_type;
      ahbso     : out   ahb_slv_out_type
    );
  end component riscv_clint_ahb_wrap;

end package ariane_esp_pkg;
