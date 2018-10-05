#ifndef AXI_SLAVE_H
#define AXI_SLAVE_H

#include <systemc.h>

#include "axi_if.h"
#include "ip.h"

#define c_s_axi_data_width 32
#define c_s_axi_addr_width 32
#define opt_mem_addr_bits  2

SC_MODULE(AXISlave) {

public:
    sc_in<bool >  s_axi_aclk;
    sc_in<bool >  s_axi_aresetn;
    sc_in<sc_uint<c_s_axi_addr_width> >  s_axi_awaddr;
    sc_in<sc_uint<3> >  s_axi_awprot;
    sc_in<bool >  s_axi_awvalid;
    sc_out<bool > s_axi_awready;
    sc_in<sc_uint<c_s_axi_data_width> >  s_axi_wdata;
    sc_in<sc_uint<((c_s_axi_data_width / 8))> >  s_axi_wstrb;
    sc_in<bool >  s_axi_wvalid;
    sc_out<bool > s_axi_wready;
    sc_out<sc_uint<2> > s_axi_bresp;
    sc_out<bool > s_axi_bvalid;
    sc_in<bool >  s_axi_bready;
    sc_in<sc_uint<c_s_axi_addr_width> >  s_axi_araddr;
    sc_in<sc_uint<3> >  s_axi_arprot;
    sc_in<bool >  s_axi_arvalid;
    sc_out<bool > s_axi_arready;
    sc_out<sc_uint<c_s_axi_data_width> > s_axi_rdata;
    sc_out<sc_uint<2> > s_axi_rresp;
    sc_out<bool > s_axi_rvalid;
    sc_in<bool >  s_axi_rready;

    sc_out<bool > interrupt;

    sc_signal<sc_uint<c_s_axi_addr_width> > axi_awaddr;
    sc_signal<bool > axi_awready;
    sc_signal<bool > axi_wready;
    sc_signal<sc_uint<2> > axi_bresp;
    sc_signal<bool > axi_bvalid;
    sc_signal<sc_uint<c_s_axi_addr_width> > axi_araddr;
    sc_signal<bool > axi_arready;
    sc_signal<sc_uint<c_s_axi_data_width> > axi_rdata;
    sc_signal<sc_uint<2> > axi_rresp;
    sc_signal<bool > axi_rvalid;
    sc_signal<sc_uint<c_s_axi_data_width> > slv_reg[16];
    sc_signal<bool > slv_reg_rden;
    sc_signal<bool > slv_reg_wren;
    sc_signal<sc_uint<c_s_axi_data_width> > reg_data_out;
    sc_signal<int > byte_index;


    // User Signals
    sc_signal <  sc_uint < 2 >  > s_axi_rstate;
    sc_signal <  sc_uint < 2 >  > s_axi_rnstate;
    sc_signal <  sc_uint < 3 >  > s_axi_wstate;
    sc_signal <  sc_uint < 3 >  > s_axi_wnstate;

    sc_signal < sc_uint < AXI_ADDR_WIDTH > >s_ip_waddr;
    sc_signal < sc_uint < 32 > >s_ip_wdata;
    sc_signal < sc_uint < 4 > >s_ip_wstrb;
    sc_signal < sc_uint < AXI_ADDR_WIDTH > >s_ip_raddr;
    sc_signal < sc_uint < 32 > >s_ip_rdata;

    sc_signal<bool > s_ip_wvalid;
    sc_signal<bool > s_ip_rvalid;
    sc_signal<bool > s_ip_wready;
    sc_signal<bool > s_ip_rready;

    axi_if  atr;
    myip ip0;

    AXISlave(sc_module_name name) : sc_module(name) , atr("Axi_If"), ip0("ip0")
    {
        // Clock and Reset
        atr.axi_aclk(s_axi_aclk);
        atr.axi_aresetn(s_axi_aresetn);

        // AXI Write Address Channel
        atr.s_axi_awaddr(s_axi_awaddr);
        atr.s_axi_awprot(s_axi_awprot);
        atr.s_axi_awvalid(s_axi_awvalid);
        atr.s_axi_awready(s_axi_awready);

        // AXI Write Data Channel
        atr.s_axi_wdata(s_axi_wdata);
        atr.s_axi_wstrb(s_axi_wstrb);
        atr.s_axi_wvalid(s_axi_wvalid);
        atr.s_axi_wready(s_axi_wready);
        atr.s_axi_wnstate(s_axi_wnstate);
        atr.s_axi_wstate(s_axi_wstate);

        // AXI Read Address Channel
        atr.s_axi_araddr(s_axi_araddr);
        atr.s_axi_arprot(s_axi_arprot);
        atr.s_axi_arvalid(s_axi_arvalid);
        atr.s_axi_arready(s_axi_arready);

        // AXI Read Data Channel
        atr.s_axi_rdata(s_axi_rdata);
        atr.s_axi_rresp(s_axi_rresp);
        atr.s_axi_rvalid(s_axi_rvalid);
        atr.s_axi_rready(s_axi_rready);
        atr.s_axi_rnstate(s_axi_rnstate);
        atr.s_axi_rstate(s_axi_rstate);

        // AXI Write Response Channel
        atr.s_axi_bresp(s_axi_bresp);
        atr.s_axi_bvalid(s_axi_bvalid);
        atr.s_axi_bready(s_axi_bready);

        // User Ports
        atr.s_ip_waddr(s_ip_waddr);
        atr.s_ip_wdata(s_ip_wdata);
        atr.s_ip_wstrb(s_ip_wstrb);
        atr.s_ip_raddr(s_ip_raddr);
        atr.s_ip_rdata(s_ip_rdata);

        atr.s_ip_wvalid(s_ip_wvalid);
        atr.s_ip_rvalid(s_ip_rvalid);
        atr.s_ip_wready(s_ip_wready);
        atr.s_ip_rready(s_ip_rready);

        // IP Ports
        ip0.axi_aclk(s_axi_aclk);
        ip0.axi_aresetn(s_axi_aresetn);
        ip0.interrupt(interrupt);

        ip0.s_ip_wvalid(s_ip_wvalid);
        ip0.s_ip_rvalid(s_ip_rvalid);
        ip0.s_ip_wready(s_ip_wready);
        ip0.s_ip_rready(s_ip_rready);

        ip0.s_ip_waddr(s_ip_waddr);
        ip0.s_ip_wdata(s_ip_wdata);
        ip0.s_ip_wstrb(s_ip_wstrb);
        ip0.s_ip_raddr(s_ip_raddr);
        ip0.s_ip_rdata(s_ip_rdata);
    }

    ~AXISlave() {
    }
    SC_HAS_PROCESS(AXISlave);

};


#endif
