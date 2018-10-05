set_top AXISlave
add_files -tb tb/axi/AXIBridgeStructs.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/AXIParameters.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axiBridge.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axiBridge.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_address_control_channel_model.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_address_control_channel_model.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_lite_master_transaction_model.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_lite_master_transaction_model.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_read_data_channel_model.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_read_data_channel_model.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_write_data_channel_model.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_write_data_channel_model.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_write_data_response_channel_model.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb tb/axi/axi_write_data_response_channel_model.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/input_data.txt -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/places205CNN_conv1_bias.txt -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/places205CNN_conv1_filter.txt -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/tb_init.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/tb_init.h -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files -tb  tb/top.cpp -cflags "-I/opt/eds/systemc/simsoc/include -I/opt/eds/systemc/simsoc/include/libsimsoc -I/opt/eds/systemc/sysc/systemc-2.3.0/include -m64 -DSC_INCLUDE_DYNAMIC_PROCESSES"
add_files   synth/ip.h -cflags "-m64"
add_files   synth/ip.cpp -cflags "-m64"
add_files   synth/axi_if.h -cflags "-m64"
add_files   synth/axi_if.cpp -cflags "-m64"
add_files   synth/axi_slave.h -cflags "-m64"
add_files   synth/axi_slave.cpp -cflags "-m64"
#
open_solution "solution1"
set_part {xc7vx485tffg1761-2}
#set_part {xc6slx150tfgg484-3}
#set_part {xc3s2000fg676-4}
#set_part xc6slx150tfgg676-3
#set_part xc6vlx240tlff1156-1
#set_part {xc7vx550tffg1927-2}
create_clock -period 10 -name default
source "./directives.tcl"
