############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2014 Xilinx Inc. All rights reserved.
############################################################
open_project proj_conv_neuron
set_top conv_neuron
add_files conv_neuron.h
add_files conv_neuron.cpp
add_files -tb main.cpp
add_files -tb tb_conv_neuron.cpp
add_files -tb tb_conv_neuron.h
open_solution "solution1"
#set_part {xc7z100ffg1156-1}
set_part {xc7z020clg484-3}
create_clock -period 50 -name default
#source "directives.tcl"
csim_design -clean
csynth_design
#cosim_design -trace_level all -rtl vhdl -tool modelsim
#export_design -format ip_catalog
exit
