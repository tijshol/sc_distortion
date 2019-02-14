#ifndef AXI_SLAVE_H
#define AXI_SLAVE_H

#include <systemc.h>
#include "ip.h"


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


SC_MODULE (AXISlave)
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

    // AXI Write Response Channel
    sc_out < sc_uint < 2 > >s_axi_bresp;
    sc_out < bool > s_axi_bvalid;
    sc_in < bool > s_axi_bready;
	sc_out < bool > interrupt_request;

    // Registered signals
    sc_signal < bool > s_axi_awready_r;
    sc_signal < bool > s_axi_wready_r;
    sc_signal < sc_uint < (S_AXI_AWADDR) > >s_axi_awaddr_reg_r;
    sc_signal < bool > s_axi_bvalid_r;
    sc_signal < sc_uint < (S_AXI_BRESP) > >s_axi_bresp_r;
    sc_signal < bool > s_axi_arready_r;
    sc_signal < sc_uint < (S_AXI_ARADDR) > >s_axi_araddr_reg_r;
    sc_signal < bool > s_axi_rvalid_r;
    sc_signal < bool > s_axi_arvalid_r;
    sc_signal < sc_uint < (S_AXI_RRESP) > >s_axi_rresp_r;
    sc_signal < sc_uint < (S_AXI_WDATA) > >s_axi_wdata_reg_r;
    sc_signal < sc_uint < (S_AXI_WSTRB) > >s_axi_wstrb_reg_r;
    sc_signal < sc_uint < (S_AXI_RDATA) > >s_axi_rdata_r;

    sc_signal < bool > s_axi_wvalid_r;
//    sc_signal < bool > s_axi_wvalid_r_r;
//    sc_signal < bool > s_axi_arready_r_r;
    // User Ports

    sc_signal < sc_uint < AXI_ADDR_WIDTH > >s_ip_raddr_r;
    sc_signal < sc_uint < 32 > >s_ip_rdata_r;
    sc_signal < sc_uint < AXI_ADDR_WIDTH > >s_ip_waddr_r;
    sc_signal < sc_uint < 4 > >s_ip_wstrb_r;
    sc_signal < sc_uint < 32 > >s_ip_wdata_r;

    sc_signal < bool > s_ip_wvalid_r;
    sc_signal < bool > s_ip_wvalid;
    sc_signal < bool > s_ip_rvalid_r;
    sc_signal < bool > s_ip_valid_r;
    sc_signal < bool > s_ip_rvalid;
    sc_signal < bool > s_ip_wready;
    sc_signal < bool > s_ip_rready;

    myip myip_1;

    int gen_select_mask();

	//processes
    void read_fsm (void);
    void write_fsm (void);
    void outputs (void);


    SC_CTOR(AXISlave) : myip_1("myip_1")
    {
        SC_CTHREAD (read_fsm, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);

        SC_CTHREAD (write_fsm, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);

        SC_METHOD (outputs);
		sensitive 	
					<< s_axi_awaddr_reg_r
        			<< s_axi_wvalid_r
					<< s_axi_awready_r
					<< s_axi_wdata_reg_r
					<< s_axi_wstrb_reg_r
        			<< s_axi_wready_r
        			<< s_axi_bvalid_r
        			<< s_axi_bresp_r
					<< s_axi_araddr_reg_r
        			<< s_axi_arvalid_r
        			<< s_axi_arready_r
					<< s_axi_rdata_r
					<< s_axi_rvalid_r
        			<< s_axi_rresp_r;
//					<< s_ip_wvalid_r
//					<< s_ip_rvalid_r

        // IP Ports
        myip_1.axi_aclk(axi_aclk);
        myip_1.axi_aresetn(axi_aresetn);
        myip_1.interrupt_request(interrupt_request);

        myip_1.s_ip_wvalid(s_ip_wvalid);
        myip_1.s_ip_rvalid(s_ip_rvalid);
        myip_1.s_ip_wready(s_ip_wready);
        myip_1.s_ip_rready(s_ip_rready);

        myip_1.s_ip_waddr(s_ip_waddr_r);
        myip_1.s_ip_wdata(s_ip_wdata_r);
        myip_1.s_ip_wstrb(s_ip_wstrb_r);
        myip_1.s_ip_raddr(s_ip_raddr_r);
        myip_1.s_ip_rdata(s_ip_rdata_r);
    }

    ~AXISlave ()
    {
    }


};

#endif
