#ifndef IP_H
#define IP_H

#include <systemc.h>
#include "../sw/app.h"

#define    AXI_ADDR_WIDTH                     32	// width of the AXI address bus

#ifndef __SYNTHESIS__
extern unsigned registers[NUM_REG];

extern sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_input [INPUT_SIZE];
extern sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_output [OUTPUT_SIZE];
#endif


SC_MODULE (myip)
{
    // Clock and Reset
    sc_in < bool > axi_aclk;
    sc_in < bool > axi_aresetn;

    // User Ports
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_ip_waddr;
    sc_in < sc_uint < 32 > >s_ip_wdata;
    sc_in < sc_uint < 4 > >s_ip_wstrb;
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_ip_raddr;
    sc_in < bool > s_ip_wvalid;
    sc_in < bool > s_ip_rvalid;

    sc_out < sc_uint < 32 > >s_ip_rdata;
    sc_out < bool > s_ip_wready;
    sc_out < bool > s_ip_rready;

    sc_out < bool > interrupt_request;
	unsigned registers[16];
#ifdef __SYNTHESIS__
    sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_input [INPUT_SIZE];
    sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_output [OUTPUT_SIZE];
#endif

    sc_signal < bool > cnn_start;
    sc_signal < bool > cnn_done;

    void proc_ip();
    int gen_select_mask();
    int run_default();
    int run_clip();
    int run_overdrive();
    int run_fuzz();
    sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> float2fixed(unsigned input);
    unsigned fixed2float(sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> output);
    sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> expm1(sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> input, unsigned depth);

    SC_CTOR(myip)
    {
        SC_CTHREAD (proc_ip, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);
    }

    ~myip ()
    {
    }

};

#endif
