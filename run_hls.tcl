open_project VLSIProject
source "config.tcl"
#csim_design -clean
csynth_design
#file copy   -force fir.h proj_fir/solution1/syn/vhdl
#file copy   -force simcore_mac_0.h proj_fir/solution1/syn/vhdl
#export_design 
close_project
open_project VLSIProject
source "config.tcl"
catch { cosim_design -setup -O -ldflags {-m64 -L/opt/eds/systemc/simsoc/lib-linux64-gcc-4.4.7 -lsimsoc} -argv {../../sw/wbs32_test} -trace_level all -rtl vhdl -tool modelsim  -compiled_library_dir /opt/eds/modeltech/10.6/xilinx }
#file copy   -force run_cosim.sh proj_fir/solution1/sim/vhdl
#exec /usr/bin/sed -i {/OUT.*:=/s/:=.*;/;/} proj_fir/solution1/sim/vhdl/fir.vhd
exit
