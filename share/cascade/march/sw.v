`ifndef __SHARE_CASCADE_MARCH_SW_V
`define __SHARE_CASCADE_MARCH_SW_V

`include "share/cascade/stdlib/stdlib.v"

(*__target="sw;verilator64"*)
Root root();

Clock clock();

(*__loc="/tmp/fpga_socket"*)
Reset reset();

(*__loc="/tmp/fpga_socket"*)
Pad#(4) pad();

(*__loc="/tmp/fpga_socket"*)
Led#(8) led();

`endif
