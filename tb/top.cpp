//
// SimSoC Initial software, Copyright © INRIA 2007, 2008, 2009, 2010
// LGPL license version 3
//

#include <libsimsoc/context.hpp>
#include <libsimsoc/processors/arm/arm_processor.hpp>
#include <libsimsoc/processors/arm_v6/arm_v6_processor.hpp>
#include <libsimsoc/components/memory.hpp>
#include <libsimsoc/components/debug_console.hpp>
#include <libsimsoc/components/irqc.hpp>
#include <libsimsoc/components/bus.hpp>
#include <libsimsoc/components/dce_null_modem.hpp>
#include <libsimsoc/components/stop_box.hpp>
#include "libsimsoc/interfaces/tlm.hpp"
#include "libsimsoc/interfaces/tlm_signal.hpp"

#include "../sw/mmap.h"
#include "tb_init.h"
#include "axi_lite_master_transaction_model.h"
#include "AXIBridgeStructs.h"
#include "axiBridge.h"

#ifdef __RTL_SIMULATION__
#include "AXISlave_rtl_wrapper.h"
#define AXISlave AXISlave_rtl_wrapper
#elif MTI_SYSTEMC
#include "axi_slave_vhdl.h"
#else
#include "../synth/axi_slave.h"
#endif


using namespace std;
using namespace sc_core;
using namespace simsoc;

ParameterBool arm_v5("ARM processor", "-v5", "Simulate ARMv5 (instead of ARMv6)");

class SubSystem: public Module {
public:
    static const uint32_t MEM_BASE = SYS_MEM_BASE;
    static const uint32_t MEM_SIZE = SYS_MEM_SIZE;

    static const uint32_t INPUT_RAM_BASE = SYS_INPUT_RAM_BASE;
    static const uint32_t INPUT_RAM_SIZE = SYS_INPUT_RAM_SIZE*4;

    static const uint32_t OUTPUT_RAM_BASE = SYS_OUTPUT_RAM_BASE;
    static const uint32_t OUTPUT_RAM_SIZE = SYS_OUTPUT_RAM_SIZE*4;

    static const uint32_t CONS_BASE = SYS_CONS_BASE;
    static const uint32_t CONS_SIZE = SYS_CONS_SIZE;

    static const uint32_t IRQC_BASE = SYS_IRQC_BASE;
    static const uint32_t IRQC_SIZE = SYS_IRQC_SIZE;

    static const uint32_t AXI_BASE = SYS_AXI_BASE;
    static const uint32_t AXI_SIZE = SYS_AXI_SIZE;

    Bus bus;
    Memory mem;

    Memory input_mem;
    Memory output_mem;

    DebugConsole cons;
    Irqc irqc;
    Processor *proc;
    AXIBridge axiBridge;
    axi_lite_master_transaction_model axi_master;

    AXISlave axi_slave ;
    sc_in<bool> clk;
    sc_in<bool> resetn;
    AXI_Signals axiSignals;
    sc_signal<bool> stb;

    void start_of_simulation () {
        std::cout << " PRESTART CALLED" << endl;
        input_mem.read_file( "audio_data.txt", 0 , STR2FLOAT);
        output_mem.read_file( "audio_data_out.txt", 0 , STR2FLOAT);
    }

    void end_of_simulation () {
        std::cout << " PRESTOP CALLED" << endl;
        // input_mem.write_file( "input_data.out", 0 ,  input_mem.elem, 0xFF, FLOAT2STR);
        output_mem.write_file( "output_data.out", 0 ,  output_mem.elem, 0xFF, UINT2STR);
    }


    SubSystem(sc_module_name name, StopBox *sb, sc_time &clock_cycle) :
            Module(name),
            // module instantiation
            bus("BUS"),
            mem("MEMORY", MEM_SIZE),
            input_mem("INPUT_DATA_MEM", INPUT_RAM_SIZE),
            output_mem("OUTPUT_DATA_MEM", OUTPUT_RAM_SIZE),
            cons("CONSOLE", sb),
            irqc("IRQC"),
            proc(NULL),
            axiBridge("AXIBridge", main_context().is_big_endian(), clock_cycle),
            axi_master("axi_master"),
#ifdef MTI_SYSTEMC
            axi_slave("axi_slave","work.AXISlave"),
#else
            axi_slave("axi_slave"),
#endif
            clk("CLK"), resetn("RESETN"), stb("STB")
    {
        if (arm_v5.get())
            proc = new ARM_Processor("ARMv5");
        else
            proc = new ARMv6_Processor("ARMv6");

        bus.print_mmap(std::cout);

        proc->get_rw_port()(bus.target_sockets);

        axiBridge.it_signal(irqc.in_signals[0]);
        irqc.out_signal(proc->get_it_port());

        bus.bind_target(mem.rw_socket, MEM_BASE, MEM_SIZE);

        bus.bind_target(input_mem.rw_socket, INPUT_RAM_BASE, INPUT_RAM_SIZE);
        bus.bind_target(output_mem.rw_socket, OUTPUT_RAM_BASE, OUTPUT_RAM_SIZE);

        bus.bind_target(cons.target_socket, CONS_BASE, CONS_SIZE);
        bus.bind_target(irqc.rw_port, IRQC_BASE, IRQC_SIZE);
        bus.bind_target(axiBridge.rw_socket, AXI_BASE, AXI_SIZE);
        cons.set_big_endian(main_context().is_big_endian());
        cons.initiator_socket(proc->debug_port);

        //AXI Bridge
        axiBridge.clk(clk);
        axiBridge.resetn(resetn);
        axiBridge.interrupt(axiSignals.interrupt);
        axiBridge.go(axiSignals.go);
        axiBridge.rnw(axiSignals.rnw);
        axiBridge.busy(axiSignals.busy);
        axiBridge.done(axiSignals.done);
        axiBridge.address(axiSignals.address);
        axiBridge.write_data(axiSignals.write_data);
        axiBridge.read_data(axiSignals.read_data);


        //AXI Master
        axi_master.go(axiSignals.go);
        axi_master.rnw(axiSignals.rnw);
        axi_master.busy(axiSignals.busy);
        axi_master.done(axiSignals.done);
        axi_master.address(axiSignals.address);
        axi_master.write_data(axiSignals.write_data);
        axi_master.read_data(axiSignals.read_data);

        axi_master.m_axi_lite_aclk(clk);
        axi_master.m_axi_lite_aresetn(resetn);
        axi_master.m_axi_lite_arready(axiSignals.axi_lite_arready);
        axi_master.m_axi_lite_arvalid(axiSignals.axi_lite_arvalid);
        axi_master.m_axi_lite_araddr(axiSignals.axi_lite_araddr);
        axi_master.m_axi_lite_rready(axiSignals.axi_lite_rready);
        axi_master.m_axi_lite_rvalid(axiSignals.axi_lite_rvalid);
        axi_master.m_axi_lite_rdata(axiSignals.axi_lite_rdata);
        axi_master.m_axi_lite_rresp(axiSignals.axi_lite_rresp);
        axi_master.m_axi_lite_awready(axiSignals.axi_lite_awready);
        axi_master.m_axi_lite_awvalid(axiSignals.axi_lite_awvalid);
        axi_master.m_axi_lite_awaddr(axiSignals.axi_lite_awaddr);
        axi_master.m_axi_lite_wready(axiSignals.axi_lite_wready);
        axi_master.m_axi_lite_wvalid(axiSignals.axi_lite_wvalid);
        axi_master.m_axi_lite_wdata(axiSignals.axi_lite_wdata);
        axi_master.m_axi_lite_wstrb(axiSignals.axi_lite_wstrb);
        axi_master.m_axi_lite_bready(axiSignals.axi_lite_bready);
        axi_master.m_axi_lite_bvalid(axiSignals.axi_lite_bvalid);
        axi_master.m_axi_lite_bresp(axiSignals.axi_lite_bresp);


        //AXI Slave
        axi_slave.s_axi_aclk(clk);
        axi_slave.s_axi_aresetn(resetn);
        axi_slave.interrupt(axiSignals.interrupt);
        axi_slave.s_axi_arready(axiSignals.axi_lite_arready);
        axi_slave.s_axi_arvalid(axiSignals.axi_lite_arvalid);
        axi_slave.s_axi_araddr(axiSignals.axi_lite_araddr);
        axi_slave.s_axi_arprot(axiSignals.axi_lite_arprot);
        axi_slave.s_axi_rready(axiSignals.axi_lite_rready);
        axi_slave.s_axi_rvalid(axiSignals.axi_lite_rvalid);
        axi_slave.s_axi_rdata(axiSignals.axi_lite_rdata);
        axi_slave.s_axi_rresp(axiSignals.axi_lite_rresp);
        axi_slave.s_axi_awaddr(axiSignals.axi_lite_awaddr);
        axi_slave.s_axi_awprot(axiSignals.axi_lite_awprot);
        axi_slave.s_axi_awvalid(axiSignals.axi_lite_awvalid);
        axi_slave.s_axi_awready(axiSignals.axi_lite_awready);
        axi_slave.s_axi_wdata(axiSignals.axi_lite_wdata);
        axi_slave.s_axi_wstrb(axiSignals.axi_lite_wstrb);
        axi_slave.s_axi_wvalid(axiSignals.axi_lite_wvalid);
        axi_slave.s_axi_wready(axiSignals.axi_lite_wready);
        axi_slave.s_axi_bresp(axiSignals.axi_lite_bresp);
        axi_slave.s_axi_bvalid(axiSignals.axi_lite_bvalid);
        axi_slave.s_axi_bready(axiSignals.axi_lite_bready);

    }
};


class Top: public Module {
public:
    sc_time clock_cycle;
    StopBox sb;
    SubSystem soc0;
    sc_clock clk;
    GenReset resetGenerator;
    sc_signal<bool> resetn;


    Top(sc_module_name name, sc_time &clock_cycle) : Module(name), clk("CLK", clock_cycle), sb("STOPBOX", 1), soc0("SOC0", &sb, clock_cycle), resetGenerator( "ResetGenerator") {
        resetGenerator.clk.bind(clk);
        resetGenerator.reset.bind(resetn);
        soc0.clk.bind(clk);
        soc0.resetn.bind(resetn);
    }
};


#ifdef MTI_SYSTEMC
//int sc_argc();
//const char* const* sc_argv();


SC_MODULE(arm_axi_soc)
{
    sc_time clock_cycle;
    int Iargc; //  = sc_argc();
    const char * const * Iargv ; // = sc_argv();

    // vsim -sc_arg "arg1" -sc_arg "arg2"

    SC_CTOR (arm_axi_soc) : clock_cycle(10, SC_NS)
    {
        Top top("TOP", clock_cycle);
        Iargc = sc_argc();
        Iargv = sc_argv();
        simsoc_init(Iargc, (char **)Iargv);
    }
} ;

SC_MODULE_EXPORT(arm_axi_soc);

#else
int sc_main(int argc, char *argv[]) {

    sc_time clock_cycle(10, SC_NS);

    simsoc_init(argc, argv);
    Top top("TOP", clock_cycle);

    sc_trace_file* fp( sc_create_vcd_trace_file( "tr" ) );
    fp->set_time_unit(1,SC_NS);

    sc_trace(fp, top.clk, "clk" );
    sc_trace(fp, top.resetn, "resetn" );
    sc_trace(fp, top.soc0.axiSignals.interrupt, "interrupt" );
#ifdef TRACE
    sc_trace(fp, top.soc0.axiSignals.go, "go" );
    sc_trace(fp, top.soc0.axiSignals.rnw, "rnw" );
    sc_trace(fp, top.soc0.axiSignals.busy, "busy" );
    sc_trace(fp, top.soc0.axiSignals.done, "done" );
    sc_trace(fp, top.soc0.axiSignals.address, "address" );
    sc_trace(fp, top.soc0.axiSignals.write_data, "write_data" );
    sc_trace(fp, top.soc0.axiSignals.read_data, "read_data" );
    //sc_trace(fp, ((sc_uint<32>)top.soc0.axi_master.current_state.read()), "current_state" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_arready, "axi_lite_arready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_arvalid, "axi_lite_arvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_araddr, "axi_lite_araddr" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rready, "axi_lite_rready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rvalid, "axi_lite_rvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rdata, "axi_lite_rdata" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_rresp, "axi_lite_rresp" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awready, "axi_lite_awready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awvalid, "axi_lite_awvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_awaddr, "axi_lite_awaddr" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wready, "axi_lite_wready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wvalid, "axi_lite_wvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wdata, "axi_lite_wdata" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_wstrb, "axi_lite_wstrb" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bready, "axi_lite_bready" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bvalid, "axi_lite_bvalid" );
    sc_trace(fp, top.soc0.axiSignals.axi_lite_bresp, "axi_lite_bresp" );


    sc_trace(fp, top.soc0.axi_master.current_state, "current_state");
    sc_trace(fp, top.soc0.axi_master.next_state, "next_state");
    sc_trace(fp, top.soc0.axi_master.read_channel_data, "read_channel_data");
    sc_trace(fp, top.soc0.axi_master.write_channel_data, "write_channel_data");
    sc_trace(fp, top.soc0.axi_master.transaction_address, "transaction_address");
    sc_trace(fp, top.soc0.axi_master.start_read_transaction, "start_read_transaction");
    sc_trace(fp, top.soc0.axi_master.start_write_transaction, "start_write_transaction");
    sc_trace(fp, top.soc0.axi_master.read_address_finished, "read_address_finished");
    sc_trace(fp, top.soc0.axi_master.read_transaction_finished, "read_transaction_finished");
    sc_trace(fp, top.soc0.axi_master.send_write_data, "send_write_data");
    sc_trace(fp, top.soc0.axi_master.write_data_sent, "write_data_sent");


    sc_trace(fp, top.soc0.axiBridge.go, "Bridge_go");
    sc_trace(fp, top.soc0.axiBridge.rnw, "Bridge_rnw");
    sc_trace(fp, top.soc0.axiBridge.busy, "Bridge_busy");
    sc_trace(fp, top.soc0.axiBridge.done, "Bridge_done");
    sc_trace(fp, top.soc0.axiBridge.address, "Bridge_address");
    sc_trace(fp, top.soc0.axiBridge.write_data, "Bridge_write_data");
    sc_trace(fp, top.soc0.axiBridge.read_data, "Bridge_read_data");

#endif

    // User Ports
#ifndef __RTL_SIMULATION__
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_waddr, "s_ip_waddr");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_wdata, "s_ip_wdata");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_wstrb, "s_ip_wstrb");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_raddr, "s_ip_raddr");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_rdata, "s_ip_rdata");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_wvalid, "s_ip_wvalid");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_rvalid, "s_ip_rvalid");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_wready, "s_ip_wready");
    sc_trace(fp, top.soc0.axi_slave.ip0.s_ip_rready, "s_ip_rready");
#endif

    //sc_trace(fp, top.soc0.axiBridge.it_signal, "it_signal");
    //sc_trace(fp, top.soc0.irqc.in_signals[0], "irqc_in_signals");

    sc_start();
    sc_close_vcd_trace_file( fp );
    return 0;
}

#endif
