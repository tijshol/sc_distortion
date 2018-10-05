#ifdef __RTL_SIMULATION__
#include "AXISlave_rtl_wrapper.h"
#define AXISlave AXISlave_rtl_wrapper
#elif MTI_SYSTEMC
#include "axi_slave_vhdl.h"
#else
#include "../synth/axi_slave.h"
#endif
