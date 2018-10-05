#ifndef IP_H
#define IP_H

#include <systemc.h>
#include "../sw/app.h"

#define    AXI_ADDR_WIDTH                     32	// width of the AXI address bus


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
    sc_out < sc_uint < 32 > >s_ip_rdata;

    sc_in < bool > s_ip_wvalid;
    sc_in < bool > s_ip_rvalid;
    sc_out < bool > s_ip_wready;
    sc_out < bool > s_ip_rready;

    sc_out < bool > interrupt;

    void run();
    int gen_select_mask();
    int run_cnn();

    SC_CTOR(myip)
    {
        SC_METHOD (run) ;
        sensitive << axi_aclk.pos();
        sensitive << axi_aresetn ;
    }

    ~myip ()
    {
    }

};

#endif
