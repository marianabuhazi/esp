library ieee;
use ieee.std_logic_1164.all;

package pads_loc is

  constant dummy_pad_loc : std_logic := '0';
  constant erx_clk_pad_loc : std_logic := '0';
  constant erxd_pad_loc : std_logic_vector(3 downto 0) := "0000";
  constant erx_dv_pad_loc : std_logic := '0';
  constant etx_clk_pad_loc : std_logic := '0';
  constant ext_clk_acc0_pad_loc : std_logic := '0';
  constant ext_clk_acc1_pad_loc : std_logic := '0';
  constant ext_clk_cpu_pad_loc : std_logic := '0';
  constant fpga_clk_in_pad_loc : std_logic := '0';
  constant fpga_clk_out_pad_loc : std_logic := '0';
  constant fpga_credit_in_pad_loc : std_logic := '0';
  constant fpga_credit_out_pad_loc : std_logic := '0';
  constant fpga_data_pad_loc : std_logic_vector(63 downto 0) := "1111111111111111111110000000000000000000000000000000000000000000";
  constant fpga_valid_in_pad_loc : std_logic := '0';
  constant fpga_valid_out_pad_loc : std_logic := '0';
  constant iolink_clk_in_pad_loc : std_logic := '0';
  constant iolink_clk_out_pad_loc : std_logic := '0';
  constant iolink_credit_in_pad_loc : std_logic := '0';
  constant iolink_credit_out_pad_loc : std_logic := '0';
  constant iolink_data_pad_loc : std_logic_vector(15 downto 0) := "1111111111111111";
  constant iolink_valid_in_pad_loc : std_logic := '0';
  constant iolink_valid_out_pad_loc : std_logic := '0';
  constant reset_pad_loc : std_logic := '0';
  constant reset_o2_pad_loc : std_logic := '0';
  constant tclk_pad_loc : std_logic := '0';
  constant tdi_cpu_pad_loc : std_logic := '0';
  constant tdo_cpu_pad_loc : std_logic := '0';
  constant tms_pad_loc : std_logic := '0';
  constant ext_clk_pad_loc : std_logic := '1';
  constant clk_div_pad_loc : std_logic := '1';
  constant emdc_pad_loc : std_logic := '1';
  constant emdio_pad_loc : std_logic := '1';
  constant erx_col_pad_loc : std_logic := '1';
  constant erx_crs_pad_loc : std_logic := '1';
  constant erx_er_pad_loc : std_logic := '1';
  constant etxd_pad_loc : std_logic_vector(3 downto 0) := "1111";
  constant etx_en_pad_loc : std_logic := '1';
  constant etx_er_pad_loc : std_logic := '1';
  constant tdi_acc_pad_loc : std_logic := '1';
  constant tdi_io_pad_loc : std_logic := '1';
  constant tdi_mem_pad_loc : std_logic := '1';
  constant tdo_acc_pad_loc : std_logic := '1';
  constant tdo_io_pad_loc : std_logic := '1';
  constant tdo_mem_pad_loc : std_logic := '1';
  constant uart_ctsn_pad_loc : std_logic := '1';
  constant uart_rtsn_pad_loc : std_logic := '1';
  constant uart_rxd_pad_loc : std_logic := '1';
  constant uart_txd_pad_loc : std_logic := '1';

end package pads_loc;
