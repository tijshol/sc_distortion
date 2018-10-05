#define SC_INCLUDE_FX   //enable fixed point data types
#include <systemc>
using namespace sc_core;
using namespace sc_dt;

#include "tb_conv_neuron.h"

// Settings to operate with Vivado HLSRTL sim wrappers
#ifdef __RTL_SIMULATION__
#include "conv_neuron_rtl_wrapper.h"
#define conv_neuron conv_neuron_rtl_wrapper
#else
#include "conv_neuron.h"
#endif

ifstream input_file ("input_data.txt");
ifstream weight_file ("weights.txt");
ifstream bias_file ("bias.txt");
ofstream output_file ("output_data.txt");

tb_conv_neuron *tb0 = NULL;
conv_neuron *conv_neuron0 = NULL;

SC_MODULE (SYSTEM)
{
    // Module declaration

    // Local signal declaration

    sc_signal < bool > rst_sig;

    sc_signal<sc_uint<FEATURE_WIDTH> >  feature_stream_sig;
    sc_signal<bool >  feature_first_sig;
    sc_signal<bool >  feature_last_sig;
    sc_signal<sc_uint<WEIGHT_WIDTH> >  weight_stream_sig;
    sc_signal<bool >  weight_first_sig;
    sc_signal<bool >  weight_last_sig;
    sc_signal<sc_uint<OUTPUT_WIDTH> > output_stream_sig;
    sc_signal<bool > output_valid_sig;

    sc_clock clk_sig;

    SC_CTOR (SYSTEM):clk_sig ("clk_sig", 10, SC_NS)
    {
        // Module instance signal connections
        tb0 = new tb_conv_neuron ("tb0");

        tb0->clk (clk_sig);
        tb0->rst (rst_sig);

        tb0->feature_stream (feature_stream_sig);
        tb0->feature_first (feature_first_sig);
        tb0->feature_last (feature_last_sig);
        tb0->weight_stream (weight_stream_sig);
        tb0->weight_first (weight_first_sig);
        tb0->weight_last (weight_last_sig);
        tb0->output_stream (output_stream_sig);
        tb0->output_valid (output_valid_sig);


        conv_neuron0 = new conv_neuron ("conv_neuron0");

        conv_neuron0->clk (clk_sig);
        conv_neuron0->rst (rst_sig);

        conv_neuron0->feature_stream (feature_stream_sig);
        conv_neuron0->feature_first (feature_first_sig);
        conv_neuron0->feature_last (feature_last_sig);
        conv_neuron0->weight_stream (weight_stream_sig);
        conv_neuron0->weight_first (weight_first_sig);
        conv_neuron0->weight_last (weight_last_sig);
        conv_neuron0->output_stream (output_stream_sig);
        conv_neuron0->output_valid (output_valid_sig);


    }

    ~SYSTEM ()
    {
        // Destructor
        delete tb0;
        delete conv_neuron0;
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

    sc_trace (fp, conv_neuron0-> wmem[0], " wmem[0]");
    sc_trace (fp, conv_neuron0-> wmem[1], " wmem[1]");
    sc_trace (fp, conv_neuron0-> wmem[2], " wmem[2]");
    sc_trace (fp, conv_neuron0-> wmem[3], " wmem[3]");
    sc_trace (fp, conv_neuron0-> wmem[4], " wmem[4]");
    sc_trace (fp, conv_neuron0-> wmem[5], " wmem[5]");
    sc_trace (fp, conv_neuron0-> wmem[6], " wmem[6]");
    sc_trace (fp, conv_neuron0-> wmem[7], " wmem[7]");
    sc_trace (fp, conv_neuron0-> wmem[8], " wmem[8]");
    sc_trace (fp, conv_neuron0-> wmem[9], " wmem[9]");
    sc_trace (fp, conv_neuron0-> wmem[10], " wmem[10]");
    sc_trace (fp, conv_neuron0-> wmem[11], " wmem[11]");
    sc_trace (fp, conv_neuron0-> wmem[12], " wmem[12]");
    sc_trace (fp, conv_neuron0-> wmem[13], " wmem[13]");
    sc_trace (fp, conv_neuron0-> wmem[14], " wmem[14]");
    sc_trace (fp, conv_neuron0-> wmem[15], " wmem[15]");
    sc_trace (fp, conv_neuron0-> wmem[16], " wmem[16]");
    sc_trace (fp, conv_neuron0-> wmem[17], " wmem[17]");
    sc_trace (fp, conv_neuron0-> wmem[18], " wmem[18]");
    sc_trace (fp, conv_neuron0-> wmem[19], " wmem[19]");
    sc_trace (fp, conv_neuron0-> wmem[20], " wmem[20]");
    sc_trace (fp, conv_neuron0-> wmem[21], " wmem[21]");
    sc_trace (fp, conv_neuron0-> wmem[22], " wmem[22]");
    sc_trace (fp, conv_neuron0-> wmem[23], " wmem[23]");
    sc_trace (fp, conv_neuron0-> wmem[24], " wmem[24]");
    sc_trace (fp, conv_neuron0-> wmem[25], " wmem[25]");
    sc_trace (fp, conv_neuron0-> wmem[26], " wmem[26]");
    sc_trace (fp, conv_neuron0-> wmem[27], " wmem[27]");
    sc_trace (fp, conv_neuron0-> wmem[28], " wmem[28]");
    sc_trace (fp, conv_neuron0-> wmem[29], " wmem[29]");
    sc_trace (fp, conv_neuron0-> wmem[30], " wmem[30]");
    sc_trace (fp, conv_neuron0-> wmem[31], " wmem[31]");

    sc_trace (fp, conv_neuron0-> wmem_waddr, " wmem_waddr");
    sc_trace (fp, conv_neuron0-> wmem_raddr, " wmem_raddr");
    sc_trace (fp, conv_neuron0-> bias, " bias");
    sc_trace (fp, conv_neuron0-> wmem_writing, " wmem_writing");
    sc_trace (fp, conv_neuron0-> p, " p");
    sc_trace (fp, conv_neuron0-> acc, " acc");
    sc_trace (fp, conv_neuron0-> zeros, " zeros");
    sc_trace (fp, conv_neuron0-> acc_slv, " acc_slv");
    sc_trace (fp, conv_neuron0-> feature_stream_reg, " feature_stream_reg");
    sc_trace (fp, conv_neuron0-> feature_stream_r1, " feature_stream_r1");
    sc_trace (fp, conv_neuron0-> feature_stream_r2, " feature_stream_r2");
    sc_trace (fp, conv_neuron0-> feature_stream_r3, " feature_stream_r3");
    sc_trace (fp, conv_neuron0-> weight_reg, " weight_reg");
    sc_trace (fp, conv_neuron0-> weight_out, " weight_out");
    sc_trace (fp, conv_neuron0-> weight_out2, " weight_out2");
    sc_trace (fp, conv_neuron0-> feature_first_r1, " feature_first_r1");
    sc_trace (fp, conv_neuron0-> feature_first_r2, " feature_first_r2");
    sc_trace (fp, conv_neuron0-> feature_first_r3, " feature_first_r3");
    sc_trace (fp, conv_neuron0-> feature_first_r4, " feature_first_r4");
    sc_trace (fp, conv_neuron0-> feature_first_r5, " feature_first_r5");
    sc_trace (fp, conv_neuron0-> feature_last_r1, " feature_last_r1");
    sc_trace (fp, conv_neuron0-> feature_last_r2, " feature_last_r2");
    sc_trace (fp, conv_neuron0-> feature_last_r3, " feature_last_r3");
    sc_trace (fp, conv_neuron0-> feature_last_r4, " feature_last_r4");
    sc_trace (fp, conv_neuron0-> feature_last_r5, " feature_last_r5");
    sc_trace (fp, conv_neuron0-> feature_last_r6, " feature_last_r6");
    sc_trace (fp, conv_neuron0-> feature_last_r7, " feature_last_r7");
    sc_trace (fp, top->rst_sig, "rst");
    sc_trace (fp, top->feature_stream_sig, "feature_stream");
    sc_trace (fp, top->feature_first_sig, "feature_first");
    sc_trace (fp, top->feature_last_sig, "feature_last");
    sc_trace (fp, top->weight_stream_sig, "weight_stream");
    sc_trace (fp, top->weight_first_sig, "weight_first");
    sc_trace (fp, top->weight_last_sig, "weight_last");
    sc_trace (fp, top->output_stream_sig, "output_stream");
    sc_trace (fp, top->output_valid_sig, "output_valid");
#ifdef TRACE
#endif

    sc_start (30, SC_US);

    sc_close_vcd_trace_file (fp);

    return 0;

}
