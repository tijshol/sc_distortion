#define SC_INCLUDE_FX   //enable fixed point data types
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#include "tb_conv_neuron_layer.h"

// Settings to operate with Vivado HLSRTL sim wrappers
#ifdef __RTL_SIMULATION__
#include "conv_neuron_layer_rtl_wrapper.h"
#define conv_neuron_layer conv_neuron_layer_rtl_wrapper
#else
#include "conv_neuron_layer.h"
#endif

tb_conv_neuron_layer *tb0 = NULL;
conv_neuron_layer *conv_neuron_layer0 = NULL;

ifstream input_file ("input_data.txt");
ifstream weight_file ("weights.txt");
ifstream bias_file ("bias.txt");
ofstream output_file ("output_data.txt");


SC_MODULE (SYSTEM)
{
    // Module declaration

    // Local signal declaration

    sc_clock clk_sig;
    sc_signal < bool > rst_sig;

    sc_signal < sc_uint < FEATURE_WIDTH  > >feature_stream_sig;
    sc_signal < bool > feature_first_sig;
    sc_signal < bool > feature_last_sig;

    sc_signal < sc_uint < WEIGHT_WIDTH  > >weight_stream_sig;
    sc_signal < sc_uint < LAYER_SIZE_ORDER  > >weight_id_sig;
    sc_signal < bool > weight_first_sig;
    sc_signal < bool > weight_last_sig;

    sc_signal < sc_uint < OUTPUT_WIDTH  > >output_stream_sig;
    sc_signal < sc_uint < LAYER_SIZE_ORDER  > >output_id_sig;
    sc_signal < bool > output_valid_sig;



    SC_CTOR (SYSTEM):clk_sig ("clk_sig", 10, SC_NS)
    {
        // Module instance signal connections
        tb0 = new tb_conv_neuron_layer ("tb0");

        tb0->clk (clk_sig);
        tb0->rst (rst_sig);

        tb0->feature_stream (feature_stream_sig);
        tb0->feature_first (feature_first_sig);
        tb0->feature_last (feature_last_sig);

        tb0->weight_stream (weight_stream_sig);
        tb0->weight_id (weight_id_sig);
        tb0->weight_first (weight_first_sig);
        tb0->weight_last (weight_last_sig);

        tb0->output_stream (output_stream_sig);
        tb0->output_id (output_id_sig);
        tb0->output_valid (output_valid_sig);


        conv_neuron_layer0 = new conv_neuron_layer ("conv_neuron_layer0");

        conv_neuron_layer0->clk (clk_sig);
        conv_neuron_layer0->rst (rst_sig);

        conv_neuron_layer0->feature_stream (feature_stream_sig);
        conv_neuron_layer0->feature_first (feature_first_sig);
        conv_neuron_layer0->feature_last (feature_last_sig);

        conv_neuron_layer0->weight_stream (weight_stream_sig);
        conv_neuron_layer0->weight_id (weight_id_sig);
        conv_neuron_layer0->weight_first (weight_first_sig);
        conv_neuron_layer0->weight_last (weight_last_sig);

        conv_neuron_layer0->output_stream (output_stream_sig);
        conv_neuron_layer0->output_id (output_id_sig);
        conv_neuron_layer0->output_valid (output_valid_sig);


    }

    ~SYSTEM ()
    {
        // Destructor
        delete tb0;
        delete conv_neuron_layer0;
    }


};


SYSTEM *top = NULL;

int
sc_main (int argc, char *argv[])
{
    // Remove simulation warnings
    sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
    sc_report_handler::set_actions( SC_ID_LOGIC_X_TO_BOOL_, SC_LOG);
    sc_report_handler::set_actions( SC_ID_VECTOR_CONTAINS_LOGIC_VALUE_, SC_LOG);

    top = new SYSTEM ("top");

    sc_trace_file *fp (sc_create_vcd_trace_file ("tr"));

    sc_trace (fp, top->clk_sig, "clk");
    sc_trace (fp, top->rst_sig, "rst");
    sc_trace (fp, top->feature_stream_sig, "feature_stream");
    sc_trace (fp, top->feature_first_sig, "feature_first");
    sc_trace (fp, top->feature_last_sig, "feature_last");
    sc_trace (fp, top->weight_stream_sig, "weight_stream");
    sc_trace (fp, top->weight_first_sig, "weight_first");
    sc_trace (fp, top->weight_last_sig, "weight_last");
    sc_trace (fp, top->output_stream_sig, "output_stream");
    sc_trace (fp, top->output_id_sig, "output_id");
    sc_trace (fp, top->output_valid_sig, "output_valid");

    sc_trace (fp, conv_neuron_layer0-> output_valid_sr[0], "output_valid_sr(0)");
    sc_trace (fp, conv_neuron_layer0-> output_id_fanin_sr[0], "output_id_fanin_sr(0)");
    sc_trace (fp, conv_neuron_layer0-> output_fanin_sr[0], "output_fanin_sr(0)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.feature_stream ,"feature_fanout_sr(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.feature_first  ,"feature_first_sr(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.feature_last   ,"feature_last_sr(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.weight_stream  ,"weight_fanout_sr(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.weight_first   ,"weight_first_valid(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.weight_last    ,"weight_last_valid(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.output_stream  ,"output_sig(1)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst1.output_valid   ,"output_valid_sig(1)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.feature_stream ,"feature_fanout_sr(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.feature_first  ,"feature_first_sr(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.feature_last   ,"feature_last_sr(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.weight_stream  ,"weight_fanout_sr(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.weight_first   ,"weight_first_valid(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.weight_last    ,"weight_last_valid(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.output_stream  ,"output_sig(2)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst2.output_valid   ,"output_valid_sig(2)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.feature_stream ,"feature_fanout_sr(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.feature_first  ,"feature_first_sr(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.feature_last   ,"feature_last_sr(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.weight_stream  ,"weight_fanout_sr(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.weight_first   ,"weight_first_valid(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.weight_last    ,"weight_last_valid(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.output_stream  ,"output_sig(3)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst3.output_valid   ,"output_valid_sig(3)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.feature_stream ,"feature_fanout_sr(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.feature_first  ,"feature_first_sr(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.feature_last   ,"feature_last_sr(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.weight_stream  ,"weight_fanout_sr(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.weight_first   ,"weight_first_valid(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.weight_last    ,"weight_last_valid(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.output_stream  ,"output_sig(4)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst4.output_valid   ,"output_valid_sig(4)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.feature_stream ,"feature_fanout_sr(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.feature_first  ,"feature_first_sr(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.feature_last   ,"feature_last_sr(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.weight_stream  ,"weight_fanout_sr(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.weight_first   ,"weight_first_valid(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.weight_last    ,"weight_last_valid(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.output_stream  ,"output_sig(5)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst5.output_valid   ,"output_valid_sig(5)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.feature_stream ,"feature_fanout_sr(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.feature_first  ,"feature_first_sr(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.feature_last   ,"feature_last_sr(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.weight_stream  ,"weight_fanout_sr(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.weight_first   ,"weight_first_valid(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.weight_last    ,"weight_last_valid(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.output_stream  ,"output_sig(6)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst6.output_valid   ,"output_valid_sig(6)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.feature_stream ,"feature_fanout_sr(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.feature_first  ,"feature_first_sr(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.feature_last   ,"feature_last_sr(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.weight_stream  ,"weight_fanout_sr(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.weight_first   ,"weight_first_valid(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.weight_last    ,"weight_last_valid(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.output_stream  ,"output_sig(7)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst7.output_valid   ,"output_valid_sig(7)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.feature_stream ,"feature_fanout_sr(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.feature_first  ,"feature_first_sr(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.feature_last   ,"feature_last_sr(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.weight_stream  ,"weight_fanout_sr(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.weight_first   ,"weight_first_valid(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.weight_last    ,"weight_last_valid(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.output_stream  ,"output_sig(8)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst8.output_valid   ,"output_valid_sig(8)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.feature_stream ,"feature_fanout_sr(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.feature_first  ,"feature_first_sr(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.feature_last   ,"feature_last_sr(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.weight_stream  ,"weight_fanout_sr(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.weight_first   ,"weight_first_valid(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.weight_last    ,"weight_last_valid(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.output_stream  ,"output_sig(9)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst9.output_valid   ,"output_valid_sig(9)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.feature_stream ,"feature_fanout_sr(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.feature_first  ,"feature_first_sr(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.feature_last   ,"feature_last_sr(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.weight_stream  ,"weight_fanout_sr(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.weight_first   ,"weight_first_valid(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.weight_last    ,"weight_last_valid(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.output_stream  ,"output_sig(10)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst10.output_valid   ,"output_valid_sig(10)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.feature_stream ,"feature_fanout_sr(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.feature_first  ,"feature_first_sr(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.feature_last   ,"feature_last_sr(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.weight_stream  ,"weight_fanout_sr(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.weight_first   ,"weight_first_valid(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.weight_last    ,"weight_last_valid(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.output_stream  ,"output_sig(11)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst11.output_valid   ,"output_valid_sig(11)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.feature_stream ,"feature_fanout_sr(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.feature_first  ,"feature_first_sr(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.feature_last   ,"feature_last_sr(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.weight_stream  ,"weight_fanout_sr(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.weight_first   ,"weight_first_valid(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.weight_last    ,"weight_last_valid(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.output_stream  ,"output_sig(12)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst12.output_valid   ,"output_valid_sig(12)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.feature_stream ,"feature_fanout_sr(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.feature_first  ,"feature_first_sr(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.feature_last   ,"feature_last_sr(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.weight_stream  ,"weight_fanout_sr(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.weight_first   ,"weight_first_valid(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.weight_last    ,"weight_last_valid(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.output_stream  ,"output_sig(13)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst13.output_valid   ,"output_valid_sig(13)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.feature_stream ,"feature_fanout_sr(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.feature_first  ,"feature_first_sr(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.feature_last   ,"feature_last_sr(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.weight_stream  ,"weight_fanout_sr(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.weight_first   ,"weight_first_valid(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.weight_last    ,"weight_last_valid(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.output_stream  ,"output_sig(14)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst14.output_valid   ,"output_valid_sig(14)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.feature_stream ,"feature_fanout_sr(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.feature_first  ,"feature_first_sr(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.feature_last   ,"feature_last_sr(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.weight_stream  ,"weight_fanout_sr(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.weight_first   ,"weight_first_valid(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.weight_last    ,"weight_last_valid(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.output_stream  ,"output_sig(15)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst15.output_valid   ,"output_valid_sig(15)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.feature_stream ,"feature_fanout_sr(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.feature_first  ,"feature_first_sr(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.feature_last   ,"feature_last_sr(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.weight_stream  ,"weight_fanout_sr(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.weight_first   ,"weight_first_valid(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.weight_last    ,"weight_last_valid(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.output_stream  ,"output_sig(16)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst16.output_valid   ,"output_valid_sig(16)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.feature_stream ,"feature_fanout_sr(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.feature_first  ,"feature_first_sr(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.feature_last   ,"feature_last_sr(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.weight_stream  ,"weight_fanout_sr(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.weight_first   ,"weight_first_valid(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.weight_last    ,"weight_last_valid(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.output_stream  ,"output_sig(17)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst17.output_valid   ,"output_valid_sig(17)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.feature_stream ,"feature_fanout_sr(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.feature_first  ,"feature_first_sr(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.feature_last   ,"feature_last_sr(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.weight_stream  ,"weight_fanout_sr(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.weight_first   ,"weight_first_valid(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.weight_last    ,"weight_last_valid(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.output_stream  ,"output_sig(18)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst18.output_valid   ,"output_valid_sig(18)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.feature_stream ,"feature_fanout_sr(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.feature_first  ,"feature_first_sr(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.feature_last   ,"feature_last_sr(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.weight_stream  ,"weight_fanout_sr(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.weight_first   ,"weight_first_valid(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.weight_last    ,"weight_last_valid(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.output_stream  ,"output_sig(19)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst19.output_valid   ,"output_valid_sig(19)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.feature_stream ,"feature_fanout_sr(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.feature_first  ,"feature_first_sr(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.feature_last   ,"feature_last_sr(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.weight_stream  ,"weight_fanout_sr(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.weight_first   ,"weight_first_valid(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.weight_last    ,"weight_last_valid(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.output_stream  ,"output_sig(20)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst20.output_valid   ,"output_valid_sig(20)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.feature_stream ,"feature_fanout_sr(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.feature_first  ,"feature_first_sr(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.feature_last   ,"feature_last_sr(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.weight_stream  ,"weight_fanout_sr(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.weight_first   ,"weight_first_valid(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.weight_last    ,"weight_last_valid(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.output_stream  ,"output_sig(21)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst21.output_valid   ,"output_valid_sig(21)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.feature_stream ,"feature_fanout_sr(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.feature_first  ,"feature_first_sr(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.feature_last   ,"feature_last_sr(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.weight_stream  ,"weight_fanout_sr(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.weight_first   ,"weight_first_valid(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.weight_last    ,"weight_last_valid(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.output_stream  ,"output_sig(22)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst22.output_valid   ,"output_valid_sig(22)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.feature_stream ,"feature_fanout_sr(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.feature_first  ,"feature_first_sr(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.feature_last   ,"feature_last_sr(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.weight_stream  ,"weight_fanout_sr(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.weight_first   ,"weight_first_valid(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.weight_last    ,"weight_last_valid(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.output_stream  ,"output_sig(23)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst23.output_valid   ,"output_valid_sig(23)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.feature_stream ,"feature_fanout_sr(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.feature_first  ,"feature_first_sr(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.feature_last   ,"feature_last_sr(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.weight_stream  ,"weight_fanout_sr(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.weight_first   ,"weight_first_valid(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.weight_last    ,"weight_last_valid(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.output_stream  ,"output_sig(24)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst24.output_valid   ,"output_valid_sig(24)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.feature_stream ,"feature_fanout_sr(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.feature_first  ,"feature_first_sr(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.feature_last   ,"feature_last_sr(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.weight_stream  ,"weight_fanout_sr(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.weight_first   ,"weight_first_valid(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.weight_last    ,"weight_last_valid(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.output_stream  ,"output_sig(25)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst25.output_valid   ,"output_valid_sig(25)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.feature_stream ,"feature_fanout_sr(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.feature_first  ,"feature_first_sr(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.feature_last   ,"feature_last_sr(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.weight_stream  ,"weight_fanout_sr(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.weight_first   ,"weight_first_valid(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.weight_last    ,"weight_last_valid(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.output_stream  ,"output_sig(26)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst26.output_valid   ,"output_valid_sig(26)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.feature_stream ,"feature_fanout_sr(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.feature_first  ,"feature_first_sr(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.feature_last   ,"feature_last_sr(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.weight_stream  ,"weight_fanout_sr(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.weight_first   ,"weight_first_valid(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.weight_last    ,"weight_last_valid(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.output_stream  ,"output_sig(27)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst27.output_valid   ,"output_valid_sig(27)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.feature_stream ,"feature_fanout_sr(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.feature_first  ,"feature_first_sr(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.feature_last   ,"feature_last_sr(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.weight_stream  ,"weight_fanout_sr(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.weight_first   ,"weight_first_valid(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.weight_last    ,"weight_last_valid(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.output_stream  ,"output_sig(28)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst28.output_valid   ,"output_valid_sig(28)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.feature_stream ,"feature_fanout_sr(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.feature_first  ,"feature_first_sr(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.feature_last   ,"feature_last_sr(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.weight_stream  ,"weight_fanout_sr(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.weight_first   ,"weight_first_valid(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.weight_last    ,"weight_last_valid(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.output_stream  ,"output_sig(29)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst29.output_valid   ,"output_valid_sig(29)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.feature_stream ,"feature_fanout_sr(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.feature_first  ,"feature_first_sr(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.feature_last   ,"feature_last_sr(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.weight_stream  ,"weight_fanout_sr(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.weight_first   ,"weight_first_valid(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.weight_last    ,"weight_last_valid(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.output_stream  ,"output_sig(30)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst30.output_valid   ,"output_valid_sig(30)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.feature_stream ,"feature_fanout_sr(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.feature_first  ,"feature_first_sr(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.feature_last   ,"feature_last_sr(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.weight_stream  ,"weight_fanout_sr(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.weight_first   ,"weight_first_valid(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.weight_last    ,"weight_last_valid(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.output_stream  ,"output_sig(31)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst31.output_valid   ,"output_valid_sig(31)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.feature_stream ,"feature_fanout_sr(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.feature_first  ,"feature_first_sr(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.feature_last   ,"feature_last_sr(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.weight_stream  ,"weight_fanout_sr(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.weight_first   ,"weight_first_valid(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.weight_last    ,"weight_last_valid(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.output_stream  ,"output_sig(32)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst32.output_valid   ,"output_valid_sig(32)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.feature_stream ,"feature_fanout_sr(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.feature_first  ,"feature_first_sr(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.feature_last   ,"feature_last_sr(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.weight_stream  ,"weight_fanout_sr(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.weight_first   ,"weight_first_valid(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.weight_last    ,"weight_last_valid(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.output_stream  ,"output_sig(33)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst33.output_valid   ,"output_valid_sig(33)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.feature_stream ,"feature_fanout_sr(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.feature_first  ,"feature_first_sr(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.feature_last   ,"feature_last_sr(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.weight_stream  ,"weight_fanout_sr(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.weight_first   ,"weight_first_valid(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.weight_last    ,"weight_last_valid(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.output_stream  ,"output_sig(34)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst34.output_valid   ,"output_valid_sig(34)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.feature_stream ,"feature_fanout_sr(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.feature_first  ,"feature_first_sr(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.feature_last   ,"feature_last_sr(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.weight_stream  ,"weight_fanout_sr(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.weight_first   ,"weight_first_valid(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.weight_last    ,"weight_last_valid(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.output_stream  ,"output_sig(35)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst35.output_valid   ,"output_valid_sig(35)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.feature_stream ,"feature_fanout_sr(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.feature_first  ,"feature_first_sr(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.feature_last   ,"feature_last_sr(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.weight_stream  ,"weight_fanout_sr(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.weight_first   ,"weight_first_valid(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.weight_last    ,"weight_last_valid(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.output_stream  ,"output_sig(36)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst36.output_valid   ,"output_valid_sig(36)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.feature_stream ,"feature_fanout_sr(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.feature_first  ,"feature_first_sr(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.feature_last   ,"feature_last_sr(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.weight_stream  ,"weight_fanout_sr(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.weight_first   ,"weight_first_valid(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.weight_last    ,"weight_last_valid(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.output_stream  ,"output_sig(37)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst37.output_valid   ,"output_valid_sig(37)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.feature_stream ,"feature_fanout_sr(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.feature_first  ,"feature_first_sr(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.feature_last   ,"feature_last_sr(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.weight_stream  ,"weight_fanout_sr(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.weight_first   ,"weight_first_valid(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.weight_last    ,"weight_last_valid(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.output_stream  ,"output_sig(38)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst38.output_valid   ,"output_valid_sig(38)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.feature_stream ,"feature_fanout_sr(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.feature_first  ,"feature_first_sr(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.feature_last   ,"feature_last_sr(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.weight_stream  ,"weight_fanout_sr(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.weight_first   ,"weight_first_valid(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.weight_last    ,"weight_last_valid(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.output_stream  ,"output_sig(39)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst39.output_valid   ,"output_valid_sig(39)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.feature_stream ,"feature_fanout_sr(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.feature_first  ,"feature_first_sr(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.feature_last   ,"feature_last_sr(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.weight_stream  ,"weight_fanout_sr(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.weight_first   ,"weight_first_valid(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.weight_last    ,"weight_last_valid(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.output_stream  ,"output_sig(40)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst40.output_valid   ,"output_valid_sig(40)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.feature_stream ,"feature_fanout_sr(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.feature_first  ,"feature_first_sr(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.feature_last   ,"feature_last_sr(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.weight_stream  ,"weight_fanout_sr(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.weight_first   ,"weight_first_valid(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.weight_last    ,"weight_last_valid(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.output_stream  ,"output_sig(41)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst41.output_valid   ,"output_valid_sig(41)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.feature_stream ,"feature_fanout_sr(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.feature_first  ,"feature_first_sr(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.feature_last   ,"feature_last_sr(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.weight_stream  ,"weight_fanout_sr(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.weight_first   ,"weight_first_valid(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.weight_last    ,"weight_last_valid(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.output_stream  ,"output_sig(42)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst42.output_valid   ,"output_valid_sig(42)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.feature_stream ,"feature_fanout_sr(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.feature_first  ,"feature_first_sr(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.feature_last   ,"feature_last_sr(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.weight_stream  ,"weight_fanout_sr(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.weight_first   ,"weight_first_valid(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.weight_last    ,"weight_last_valid(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.output_stream  ,"output_sig(43)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst43.output_valid   ,"output_valid_sig(43)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.feature_stream ,"feature_fanout_sr(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.feature_first  ,"feature_first_sr(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.feature_last   ,"feature_last_sr(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.weight_stream  ,"weight_fanout_sr(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.weight_first   ,"weight_first_valid(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.weight_last    ,"weight_last_valid(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.output_stream  ,"output_sig(44)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst44.output_valid   ,"output_valid_sig(44)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.feature_stream ,"feature_fanout_sr(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.feature_first  ,"feature_first_sr(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.feature_last   ,"feature_last_sr(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.weight_stream  ,"weight_fanout_sr(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.weight_first   ,"weight_first_valid(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.weight_last    ,"weight_last_valid(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.output_stream  ,"output_sig(45)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst45.output_valid   ,"output_valid_sig(45)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.feature_stream ,"feature_fanout_sr(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.feature_first  ,"feature_first_sr(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.feature_last   ,"feature_last_sr(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.weight_stream  ,"weight_fanout_sr(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.weight_first   ,"weight_first_valid(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.weight_last    ,"weight_last_valid(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.output_stream  ,"output_sig(46)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst46.output_valid   ,"output_valid_sig(46)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.feature_stream ,"feature_fanout_sr(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.feature_first  ,"feature_first_sr(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.feature_last   ,"feature_last_sr(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.weight_stream  ,"weight_fanout_sr(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.weight_first   ,"weight_first_valid(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.weight_last    ,"weight_last_valid(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.output_stream  ,"output_sig(47)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst47.output_valid   ,"output_valid_sig(47)");

    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.feature_stream ,"feature_fanout_sr(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.feature_first  ,"feature_first_sr(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.feature_last   ,"feature_last_sr(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.weight_stream  ,"weight_fanout_sr(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.weight_first   ,"weight_first_valid(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.weight_last    ,"weight_last_valid(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.output_stream  ,"output_sig(48)");
    sc_trace (fp, conv_neuron_layer0->conv_neuron_inst48.output_valid   ,"output_valid_sig(48)");



#ifdef TRACE
#endif

    sc_start (300, SC_US);

    sc_close_vcd_trace_file (fp);

    return 0;

}
