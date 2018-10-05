project new -name "AXISlave"
#solution file add    synth/ip.h 
#solution file add    synth/ip.cpp 
solution file add    synth/axi_if.h 
solution file add    synth/axi_if.cpp 
#solution file add    synth/axi_slave.h 
#solution file add    synth/axi_slave.cpp 
project save
directive set -REGISTER_IDLE_SIGNAL false
directive set -IDLE_SIGNAL {}
directive set -TRANSACTION_DONE_SIGNAL false
directive set -DONE_FLAG {}
directive set -START_FLAG {}
directive set -FSM_ENCODING none
directive set -REG_MAX_FANOUT 0
directive set -NO_X_ASSIGNMENTS false
directive set -SAFE_FSM false
directive set -RESET_CLEARS_ALL_REGS true
directive set -ASSIGN_OVERHEAD 0
directive set -DESIGN_GOAL area
directive set -OLD_SCHED false
directive set -PIPELINE_RAMP_UP true
directive set -COMPGRADE fast
directive set -SPECULATE true
directive set -MERGEABLE true
directive set -REGISTER_THRESHOLD 256
directive set -MEM_MAP_THRESHOLD 32
directive set -UNROLL no
directive set -CLOCK_OVERHEAD 20.000000
directive set -OPT_CONST_MULTS -1
project save
solution library add mgc_Xilinx-VIRTEX-7-3_beh -- -rtlsyntool SynplifyPro -manufacturer Xilinx -family VIRTEX-7 -speed -3 -part xc7vx485tffg1157-3
solution library add Xilinx_RAMS
solution library add Xilinx_ROMS
project save
go analyze
directive set -CLOCKS {clk {-CLOCK_PERIOD 10 -CLOCK_HIGH_TIME 5 -CLOCK_OFFSET 0.000000 -CLOCK_UNCERTAINTY 0.0}}
go compile
#go assembly
go architect
go allocate
go extract
project save
