#ifndef TB_CONV_NEURON_LAYER_H
#define TB_CONV_NEURON_LAYER_H
#include <iostream>
#include <fstream>

#include <systemc.h>
#include "conv_neuron_layer.h"

extern ifstream input_file;
extern ifstream weight_file;
extern ifstream bias_file;
extern ofstream output_file;

SC_MODULE (tb_conv_neuron_layer)
{
    sc_in < bool > clk;
    sc_out <bool>  rst;


    sc_out < sc_uint < FEATURE_WIDTH  > >feature_stream;
    sc_out < bool > feature_first;
    sc_out < bool > feature_last;

    sc_out < sc_uint < WEIGHT_WIDTH  > >weight_stream;
    sc_out < sc_uint < LAYER_SIZE_ORDER  > >weight_id;
    sc_out < bool > weight_first;
    sc_out < bool > weight_last;

    sc_in < sc_uint < OUTPUT_WIDTH  > >output_stream;
    sc_in < sc_uint < LAYER_SIZE_ORDER  > >output_id;
    sc_in < bool > output_valid;

    void source ();
    void sink ();


    SC_CTOR (tb_conv_neuron_layer)
    {
        SC_CTHREAD (source, clk.neg());
        //SC_CTHREAD (source, clk.pos());
        SC_CTHREAD (sink, clk.pos());
    }

};


#endif	/*  */
