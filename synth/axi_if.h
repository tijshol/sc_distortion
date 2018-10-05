#ifndef ARM_TEST_REGS_H
#define ARM_TEST_REGS_H

#include <systemc.h>

#define    AXI_ADDR_WIDTH                     32	// width of the AXI address bus

#define S_AXI_AWADDR 32
#define S_AXI_ARADDR 32

#define S_AXI_WDATA 32
#define S_AXI_RDATA 32

#define S_AXI_BRESP 2
#define S_AXI_RRESP 2

#define S_AXI_WSTRB 4

#define  AXI_OKAY  "0b00"
#define  AXI_DECERR  "0b11"

SC_MODULE (axi_if)
{

    // Clock and Reset
    sc_in < bool > axi_aclk;
    sc_in < bool > axi_aresetn;

    // AXI Write Address Channel
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_axi_awaddr;
    sc_in < sc_uint < 3 > >s_axi_awprot;
    sc_in < bool > s_axi_awvalid;
    sc_out < bool > s_axi_awready;

    // AXI Write Data Channel
    sc_in < sc_uint < 32 > >s_axi_wdata;
    sc_in < sc_uint < 4 > >s_axi_wstrb;
    sc_in < bool > s_axi_wvalid;
    sc_out < bool > s_axi_wready;
    sc_out <  sc_uint < 3 >  > s_axi_wstate;
    sc_out <  sc_uint < 3 >  > s_axi_wnstate;

    // AXI Read Address Channel
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_axi_araddr;
    sc_in < sc_uint < 3 > >s_axi_arprot;
    sc_in < bool > s_axi_arvalid;
    sc_out < bool > s_axi_arready;

    // AXI Read Data Channel
    sc_out < sc_uint < 32 > >s_axi_rdata;
    sc_out < sc_uint < 2 > >s_axi_rresp;
    sc_out < bool > s_axi_rvalid;
    sc_in < bool > s_axi_rready;
    sc_out <  sc_uint < 2 >  > s_axi_rstate;
    sc_out <  sc_uint < 2 >  > s_axi_rnstate;

    // AXI Write Response Channel
    sc_out < sc_uint < 2 > >s_axi_bresp;
    sc_out < bool > s_axi_bvalid;
    sc_in < bool > s_axi_bready;

    // User Ports
    sc_out < sc_uint < AXI_ADDR_WIDTH > >s_ip_waddr;
    sc_out < sc_uint < 32 > >s_ip_wdata;
    sc_out < sc_uint < 4 > >s_ip_wstrb;

    sc_out < bool > s_ip_wvalid;
    sc_out < bool > s_ip_rvalid;
    sc_in < bool > s_ip_wready;
    sc_in < bool > s_ip_rready;

    sc_out < sc_uint < AXI_ADDR_WIDTH > >s_ip_raddr;
    sc_in < sc_uint < 32 > >s_ip_rdata;

    // Registered signals
    sc_signal < bool > s_axi_awready_r;
    sc_signal < bool > s_axi_wready_r;
    sc_signal < sc_uint < (S_AXI_AWADDR) > >s_axi_awaddr_reg_r;
    sc_signal < bool > s_axi_bvalid_r;
    sc_signal < sc_uint < (S_AXI_BRESP) > >s_axi_bresp_r;
    sc_signal < bool > s_axi_arready_r;
    sc_signal < sc_uint < (S_AXI_ARADDR) > >s_axi_araddr_reg_r;
    sc_signal < bool > s_axi_rvalid_r;
    sc_signal < sc_uint < (S_AXI_RRESP) > >s_axi_rresp_r;
    sc_signal < sc_uint < (S_AXI_WDATA) > >s_axi_wdata_reg_r;
    sc_signal < sc_uint < (S_AXI_WSTRB) > >s_axi_wstrb_reg_r;
    sc_signal < sc_uint < (S_AXI_RDATA) > >s_axi_rdata_r;

    // User-defined registers
    sc_uint < 8 >   v_byte[4];

    sc_signal < sc_uint<2> > s_rstate;
    sc_signal < sc_uint<2> > s_rnstate;
    sc_signal < sc_uint<3> > s_wstate;
    sc_signal < sc_uint<3> > s_wnstate;

    void receive();
    void send();
    int gen_select_mask();

    void read_state (void);
    void read_nstate (void);
    void write_state (void);
    void write_nstate (void);
    void outputs (void);


    SC_CTOR(axi_if)
    {
        SC_CTHREAD (read_state, axi_aclk.neg ());
        reset_signal_is (axi_aresetn, false);

        SC_CTHREAD (read_nstate, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);

        SC_CTHREAD (write_state, axi_aclk.neg ());
        reset_signal_is (axi_aresetn, false);

        SC_CTHREAD (write_nstate, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);


        SC_METHOD (outputs);
        sensitive	<< s_axi_awready_r
        << s_axi_wready_r
        << s_axi_bvalid_r
        << s_axi_bresp_r
        << s_axi_arready_r
        << s_axi_rvalid_r
        << s_axi_rdata_r
        << s_axi_rresp_r
        << s_rstate
        << s_rnstate
        << s_wstate
        << s_wnstate;
    }

    ~axi_if ()
    {
    }

    //SC_HAS_PROCESS (axi_if);

};

#endif
