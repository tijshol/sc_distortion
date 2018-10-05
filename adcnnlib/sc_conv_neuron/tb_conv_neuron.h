#ifndef TB_CONV_NEURON_H
#define TB_CONV_NEURON_H

#include <systemc.h>
#include "conv_neuron.h"

#define NO_FEATURE_PLANES  3
#define MASK_WIDTH 3
#define MASK_HEIGHT 3
#define NO_WEIGHTS ((NO_FEATURE_PLANES * MASK_WIDTH) * MASK_HEIGHT)

SC_MODULE(tb_conv_neuron) {

    sc_in <bool>  clk;
    sc_out <bool>  rst;
    sc_out<sc_uint<FEATURE_WIDTH > > feature_stream;
    sc_out<bool > feature_first;
    sc_out<bool > feature_last;
    sc_out<sc_uint<WEIGHT_WIDTH > > weight_stream;
    sc_out<bool > weight_first;
    sc_out<bool > weight_last;
    sc_in<sc_uint<OUTPUT_WIDTH > > output_stream;
    sc_in<bool > output_valid;

    void source ();
    void sink();

    SC_CTOR (tb_conv_neuron)
    {
        //SC_CTHREAD(source, clk.pos());
        SC_CTHREAD(source, clk.neg());
        SC_CTHREAD(sink,  clk.pos()) ;
    }

};
#endif
