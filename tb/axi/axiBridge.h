#ifndef AXIBRIDGE_H_
#define AXIBRIDGE_H_

#include <cmath>
#include <deque>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>
#include <libsimsoc/interfaces/tlm.hpp>
#include <libsimsoc/interfaces/tlm_signal.hpp>
#include <libsimsoc/interfaces/RS232.hpp>
#include <libsimsoc/module.hpp>
#include "AXIBridgeStructs.h"
#include "axi_lite_master_transaction_model.h"

using namespace sc_core;
using namespace sc_dt;
using namespace simsoc;


class AXIBridge: public simsoc::Module {
public:
    typedef AXIBridge SC_CURRENT_USER_MODULE;
    AXIBridge(sc_core::sc_module_name name, bool big_endian, sc_time &clock_cycle);

    void b_transport(tlm::tlm_generic_payload &payload,
            sc_core::sc_time &delay_time);

    sc_in<bool> clk;
    sc_in<bool> resetn;
    sc_in<bool> interrupt_request;

    sc_out<bool > go;
    sc_out<bool > rnw;
    sc_in<bool > busy;
    sc_in<bool > done;
    sc_out<sc_uint<32> > address;
    sc_out<sc_uint<32> > write_data;
    sc_in<sc_uint<32> > read_data;

    SignalInitiatorPort<bool> it_signal;

    static const int AXISIZE ; // = 0xFFFFFFFF - 0xc0000000;
    static const int SIZE ; //  = 0x0000FFFF;

    tlm_utils::simple_target_socket<AXIBridge> rw_socket;
protected:
    void axiBusHandling();
    void updateirq();

    sc_event request;
    sc_event request_done;
    bool big_endian;
    sc_time clk_cycle;
    tlm::tlm_generic_payload *pl;

};

#endif
