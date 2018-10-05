#ifndef CONV_NEURON_LAYER_H
#define CONV_NEURON_LAYER_H

#include <systemc.h>
#include "conv_neuron.h"

#define LAYER_SIZE	48
#define LAYER_SIZE_ORDER	6
#define FEATURE_WIDTH	8
#define WEIGHT_WIDTH	8
#undef WEIGHT_MEM_ORDER
#define WEIGHT_MEM_ORDER	8
#define OUTPUT_WIDTH	8
#define OUTPUT_SHIFT	0
#define BIAS_SHIFT	0
#define RELU	1
#define NO_FEATURE_PLANES	3
#define MASK_WIDTH	7
#define MASK_HEIGHT	7
#define NO_WEIGHTS	(NO_FEATURE_PLANES * MASK_WIDTH * MASK_HEIGHT)

SC_MODULE (conv_neuron_layer)
{
    sc_in < bool > clk;
    sc_in < bool > rst;

    sc_in < sc_uint < FEATURE_WIDTH > >feature_stream;
    sc_in < bool > feature_first;
    sc_in < bool > feature_last;

    sc_in < sc_uint < WEIGHT_WIDTH > >weight_stream;
    sc_in < sc_uint < LAYER_SIZE_ORDER > >weight_id;
    sc_in < bool > weight_first;
    sc_in < bool > weight_last;

    sc_out < sc_uint < OUTPUT_WIDTH > >output_stream;
    sc_out < sc_uint < LAYER_SIZE_ORDER > >output_id;
    sc_out < bool > output_valid;

    typedef sc_uint < FEATURE_WIDTH > feature_fanout_type;
    typedef sc_uint < WEIGHT_WIDTH > weight_fanout_type;
    typedef sc_uint < LAYER_SIZE_ORDER > id_fanout_type;
    typedef sc_uint < OUTPUT_WIDTH > output_fanin_type;

    //sc_signal < feature_fanout_type > feature_fanout_sr[LAYER_SIZE +1];
    sc_signal < sc_uint < FEATURE_WIDTH > > feature_fanout_sr[LAYER_SIZE +1];
    sc_signal < weight_fanout_type > weight_fanout_sr[LAYER_SIZE +1];
    sc_signal < id_fanout_type > weight_id_fanout_sr[LAYER_SIZE +1];
    sc_signal < id_fanout_type > output_id_fanin_sr[LAYER_SIZE +1];
    sc_signal < output_fanin_type > output_fanin_sr[LAYER_SIZE +1];
    sc_signal < output_fanin_type > output_sig[LAYER_SIZE +1];

    sc_signal < bool > feature_first_sr  [ LAYER_SIZE  +1];
    sc_signal < bool > feature_last_sr  [ LAYER_SIZE  +1];
    sc_signal < bool > weight_first_sr  [ LAYER_SIZE  +1];
    sc_signal < bool > weight_last_sr  [ LAYER_SIZE  +1];
    sc_signal < bool > output_valid_sr  [ LAYER_SIZE  +1];
    sc_signal < bool > weight_first_valid  [ LAYER_SIZE  +1];
    sc_signal < bool > weight_last_valid  [ LAYER_SIZE  +1];
    // sc_signal < sc_uint < LAYER_SIZE  > >output_valid_sig;
    sc_signal < bool >output_valid_sig [ LAYER_SIZE  +1];


    void conv_neuron_layer_main ();
    void conv_neuron_layer_output ();

    conv_neuron conv_neuron_inst1,
    conv_neuron_inst2,
    conv_neuron_inst3,
    conv_neuron_inst4,
    conv_neuron_inst5,
    conv_neuron_inst6,
    conv_neuron_inst7,
    conv_neuron_inst8,
    conv_neuron_inst9,
    conv_neuron_inst10,
    conv_neuron_inst11,
    conv_neuron_inst12,
    conv_neuron_inst13,
    conv_neuron_inst14,
    conv_neuron_inst15,
    conv_neuron_inst16,
    conv_neuron_inst17,
    conv_neuron_inst18,
    conv_neuron_inst19,
    conv_neuron_inst20,
    conv_neuron_inst21,
    conv_neuron_inst22,
    conv_neuron_inst23,
    conv_neuron_inst24,
    conv_neuron_inst25,
    conv_neuron_inst26,
    conv_neuron_inst27,
    conv_neuron_inst28,
    conv_neuron_inst29,
    conv_neuron_inst30,
    conv_neuron_inst31,
    conv_neuron_inst32,
    conv_neuron_inst33,
    conv_neuron_inst34,
    conv_neuron_inst35,
    conv_neuron_inst36,
    conv_neuron_inst37,
    conv_neuron_inst38,
    conv_neuron_inst39,
    conv_neuron_inst40,
    conv_neuron_inst41,
    conv_neuron_inst42,
    conv_neuron_inst43,
    conv_neuron_inst44,
    conv_neuron_inst45,
    conv_neuron_inst46,
    conv_neuron_inst47,
    conv_neuron_inst48;

    SC_CTOR (conv_neuron_layer) :
            clk("clk"),
            rst("rst"),
            conv_neuron_inst1 ("conv_neuron_inst1"),
            conv_neuron_inst2 ("conv_neuron_inst2"),
            conv_neuron_inst3 ("conv_neuron_inst3"),
            conv_neuron_inst4 ("conv_neuron_inst4"),
            conv_neuron_inst5 ("conv_neuron_inst5"),
            conv_neuron_inst6 ("conv_neuron_inst6"),
            conv_neuron_inst7 ("conv_neuron_inst7"),
            conv_neuron_inst8 ("conv_neuron_inst8"),
            conv_neuron_inst9 ("conv_neuron_inst9"),
            conv_neuron_inst10 ("conv_neuron_inst10"),
            conv_neuron_inst11 ("conv_neuron_inst11"),
            conv_neuron_inst12 ("conv_neuron_inst12"),
            conv_neuron_inst13 ("conv_neuron_inst13"),
            conv_neuron_inst14 ("conv_neuron_inst14"),
            conv_neuron_inst15 ("conv_neuron_inst15"),
            conv_neuron_inst16 ("conv_neuron_inst16"),
            conv_neuron_inst17 ("conv_neuron_inst17"),
            conv_neuron_inst18 ("conv_neuron_inst18"),
            conv_neuron_inst19 ("conv_neuron_inst19"),
            conv_neuron_inst20 ("conv_neuron_inst20"),
            conv_neuron_inst21 ("conv_neuron_inst21"),
            conv_neuron_inst22 ("conv_neuron_inst22"),
            conv_neuron_inst23 ("conv_neuron_inst23"),
            conv_neuron_inst24 ("conv_neuron_inst24"),
            conv_neuron_inst25 ("conv_neuron_inst25"),
            conv_neuron_inst26 ("conv_neuron_inst26"),
            conv_neuron_inst27 ("conv_neuron_inst27"),
            conv_neuron_inst28 ("conv_neuron_inst28"),
            conv_neuron_inst29 ("conv_neuron_inst29"),
            conv_neuron_inst30 ("conv_neuron_inst30"),
            conv_neuron_inst31 ("conv_neuron_inst31"),
            conv_neuron_inst32 ("conv_neuron_inst32"),
            conv_neuron_inst33 ("conv_neuron_inst33"),
            conv_neuron_inst34 ("conv_neuron_inst34"),
            conv_neuron_inst35 ("conv_neuron_inst35"),
            conv_neuron_inst36 ("conv_neuron_inst36"),
            conv_neuron_inst37 ("conv_neuron_inst37"),
            conv_neuron_inst38 ("conv_neuron_inst38"),
            conv_neuron_inst39 ("conv_neuron_inst39"),
            conv_neuron_inst40 ("conv_neuron_inst40"),
            conv_neuron_inst41 ("conv_neuron_inst41"),
            conv_neuron_inst42 ("conv_neuron_inst42"),
            conv_neuron_inst43 ("conv_neuron_inst43"),
            conv_neuron_inst44 ("conv_neuron_inst44"),
            conv_neuron_inst45 ("conv_neuron_inst45"),
            conv_neuron_inst46 ("conv_neuron_inst46"),
            conv_neuron_inst47 ("conv_neuron_inst47"),
            conv_neuron_inst48 ("conv_neuron_inst48")
    {
        SC_CTHREAD (conv_neuron_layer_main, clk.pos()  );
        reset_signal_is (rst, true);
        SC_METHOD (conv_neuron_layer_output);
        //sensitive << output_valid_sr[0] << output_id_fanin_sr[0] << output_fanin_sr[0];
        sensitive << clk.pos();

        conv_neuron_inst1.clk		(clk);
        conv_neuron_inst1.rst		(rst);
        conv_neuron_inst1.feature_stream (feature_fanout_sr[1]);
        conv_neuron_inst1.feature_first  (feature_first_sr[1]);
        conv_neuron_inst1.feature_last   (feature_last_sr[1]);
        conv_neuron_inst1.weight_stream  (weight_fanout_sr[1]);
        conv_neuron_inst1.weight_first   (weight_first_valid[1]);
        conv_neuron_inst1.weight_last    (weight_last_valid[1]);
        conv_neuron_inst1.output_stream  (output_sig[1]);
        conv_neuron_inst1.output_valid   (output_valid_sig[1]);

        conv_neuron_inst2.clk		(clk);
        conv_neuron_inst2.rst		(rst);
        conv_neuron_inst2.feature_stream (feature_fanout_sr[2]);
        conv_neuron_inst2.feature_first  (feature_first_sr[2]);
        conv_neuron_inst2.feature_last   (feature_last_sr[2]);
        conv_neuron_inst2.weight_stream  (weight_fanout_sr[2]);
        conv_neuron_inst2.weight_first   (weight_first_valid[2]);
        conv_neuron_inst2.weight_last    (weight_last_valid[2]);
        conv_neuron_inst2.output_stream  (output_sig[2]);
        conv_neuron_inst2.output_valid   (output_valid_sig[2]);

        conv_neuron_inst3.clk		(clk);
        conv_neuron_inst3.rst		(rst);
        conv_neuron_inst3.feature_stream (feature_fanout_sr[3]);
        conv_neuron_inst3.feature_first  (feature_first_sr[3]);
        conv_neuron_inst3.feature_last   (feature_last_sr[3]);
        conv_neuron_inst3.weight_stream  (weight_fanout_sr[3]);
        conv_neuron_inst3.weight_first   (weight_first_valid[3]);
        conv_neuron_inst3.weight_last    (weight_last_valid[3]);
        conv_neuron_inst3.output_stream  (output_sig[3]);
        conv_neuron_inst3.output_valid   (output_valid_sig[3]);

        conv_neuron_inst4.clk		(clk);
        conv_neuron_inst4.rst		(rst);
        conv_neuron_inst4.feature_stream (feature_fanout_sr[4]);
        conv_neuron_inst4.feature_first  (feature_first_sr[4]);
        conv_neuron_inst4.feature_last   (feature_last_sr[4]);
        conv_neuron_inst4.weight_stream  (weight_fanout_sr[4]);
        conv_neuron_inst4.weight_first   (weight_first_valid[4]);
        conv_neuron_inst4.weight_last    (weight_last_valid[4]);
        conv_neuron_inst4.output_stream  (output_sig[4]);
        conv_neuron_inst4.output_valid   (output_valid_sig[4]);

        conv_neuron_inst5.clk		(clk);
        conv_neuron_inst5.rst		(rst);
        conv_neuron_inst5.feature_stream (feature_fanout_sr[5]);
        conv_neuron_inst5.feature_first  (feature_first_sr[5]);
        conv_neuron_inst5.feature_last   (feature_last_sr[5]);
        conv_neuron_inst5.weight_stream  (weight_fanout_sr[5]);
        conv_neuron_inst5.weight_first   (weight_first_valid[5]);
        conv_neuron_inst5.weight_last    (weight_last_valid[5]);
        conv_neuron_inst5.output_stream  (output_sig[5]);
        conv_neuron_inst5.output_valid   (output_valid_sig[5]);

        conv_neuron_inst6.clk		(clk);
        conv_neuron_inst6.rst		(rst);
        conv_neuron_inst6.feature_stream (feature_fanout_sr[6]);
        conv_neuron_inst6.feature_first  (feature_first_sr[6]);
        conv_neuron_inst6.feature_last   (feature_last_sr[6]);
        conv_neuron_inst6.weight_stream  (weight_fanout_sr[6]);
        conv_neuron_inst6.weight_first   (weight_first_valid[6]);
        conv_neuron_inst6.weight_last    (weight_last_valid[6]);
        conv_neuron_inst6.output_stream  (output_sig[6]);
        conv_neuron_inst6.output_valid   (output_valid_sig[6]);

        conv_neuron_inst7.clk		(clk);
        conv_neuron_inst7.rst		(rst);
        conv_neuron_inst7.feature_stream (feature_fanout_sr[7]);
        conv_neuron_inst7.feature_first  (feature_first_sr[7]);
        conv_neuron_inst7.feature_last   (feature_last_sr[7]);
        conv_neuron_inst7.weight_stream  (weight_fanout_sr[7]);
        conv_neuron_inst7.weight_first   (weight_first_valid[7]);
        conv_neuron_inst7.weight_last    (weight_last_valid[7]);
        conv_neuron_inst7.output_stream  (output_sig[7]);
        conv_neuron_inst7.output_valid   (output_valid_sig[7]);

        conv_neuron_inst8.clk		(clk);
        conv_neuron_inst8.rst		(rst);
        conv_neuron_inst8.feature_stream (feature_fanout_sr[8]);
        conv_neuron_inst8.feature_first  (feature_first_sr[8]);
        conv_neuron_inst8.feature_last   (feature_last_sr[8]);
        conv_neuron_inst8.weight_stream  (weight_fanout_sr[8]);
        conv_neuron_inst8.weight_first   (weight_first_valid[8]);
        conv_neuron_inst8.weight_last    (weight_last_valid[8]);
        conv_neuron_inst8.output_stream  (output_sig[8]);
        conv_neuron_inst8.output_valid   (output_valid_sig[8]);

        conv_neuron_inst9.clk		(clk);
        conv_neuron_inst9.rst		(rst);
        conv_neuron_inst9.feature_stream (feature_fanout_sr[9]);
        conv_neuron_inst9.feature_first  (feature_first_sr[9]);
        conv_neuron_inst9.feature_last   (feature_last_sr[9]);
        conv_neuron_inst9.weight_stream  (weight_fanout_sr[9]);
        conv_neuron_inst9.weight_first   (weight_first_valid[9]);
        conv_neuron_inst9.weight_last    (weight_last_valid[9]);
        conv_neuron_inst9.output_stream  (output_sig[9]);
        conv_neuron_inst9.output_valid   (output_valid_sig[9]);

        conv_neuron_inst10.clk		(clk);
        conv_neuron_inst10.rst		(rst);
        conv_neuron_inst10.feature_stream (feature_fanout_sr[10]);
        conv_neuron_inst10.feature_first  (feature_first_sr[10]);
        conv_neuron_inst10.feature_last   (feature_last_sr[10]);
        conv_neuron_inst10.weight_stream  (weight_fanout_sr[10]);
        conv_neuron_inst10.weight_first   (weight_first_valid[10]);
        conv_neuron_inst10.weight_last    (weight_last_valid[10]);
        conv_neuron_inst10.output_stream  (output_sig[10]);
        conv_neuron_inst10.output_valid   (output_valid_sig[10]);

        conv_neuron_inst11.clk		(clk);
        conv_neuron_inst11.rst		(rst);
        conv_neuron_inst11.feature_stream (feature_fanout_sr[11]);
        conv_neuron_inst11.feature_first  (feature_first_sr[11]);
        conv_neuron_inst11.feature_last   (feature_last_sr[11]);
        conv_neuron_inst11.weight_stream  (weight_fanout_sr[11]);
        conv_neuron_inst11.weight_first   (weight_first_valid[11]);
        conv_neuron_inst11.weight_last    (weight_last_valid[11]);
        conv_neuron_inst11.output_stream  (output_sig[11]);
        conv_neuron_inst11.output_valid   (output_valid_sig[11]);

        conv_neuron_inst12.clk		(clk);
        conv_neuron_inst12.rst		(rst);
        conv_neuron_inst12.feature_stream (feature_fanout_sr[12]);
        conv_neuron_inst12.feature_first  (feature_first_sr[12]);
        conv_neuron_inst12.feature_last   (feature_last_sr[12]);
        conv_neuron_inst12.weight_stream  (weight_fanout_sr[12]);
        conv_neuron_inst12.weight_first   (weight_first_valid[12]);
        conv_neuron_inst12.weight_last    (weight_last_valid[12]);
        conv_neuron_inst12.output_stream  (output_sig[12]);
        conv_neuron_inst12.output_valid   (output_valid_sig[12]);

        conv_neuron_inst13.clk		(clk);
        conv_neuron_inst13.rst		(rst);
        conv_neuron_inst13.feature_stream (feature_fanout_sr[13]);
        conv_neuron_inst13.feature_first  (feature_first_sr[13]);
        conv_neuron_inst13.feature_last   (feature_last_sr[13]);
        conv_neuron_inst13.weight_stream  (weight_fanout_sr[13]);
        conv_neuron_inst13.weight_first   (weight_first_valid[13]);
        conv_neuron_inst13.weight_last    (weight_last_valid[13]);
        conv_neuron_inst13.output_stream  (output_sig[13]);
        conv_neuron_inst13.output_valid   (output_valid_sig[13]);

        conv_neuron_inst14.clk		(clk);
        conv_neuron_inst14.rst		(rst);
        conv_neuron_inst14.feature_stream (feature_fanout_sr[14]);
        conv_neuron_inst14.feature_first  (feature_first_sr[14]);
        conv_neuron_inst14.feature_last   (feature_last_sr[14]);
        conv_neuron_inst14.weight_stream  (weight_fanout_sr[14]);
        conv_neuron_inst14.weight_first   (weight_first_valid[14]);
        conv_neuron_inst14.weight_last    (weight_last_valid[14]);
        conv_neuron_inst14.output_stream  (output_sig[14]);
        conv_neuron_inst14.output_valid   (output_valid_sig[14]);

        conv_neuron_inst15.clk		(clk);
        conv_neuron_inst15.rst		(rst);
        conv_neuron_inst15.feature_stream (feature_fanout_sr[15]);
        conv_neuron_inst15.feature_first  (feature_first_sr[15]);
        conv_neuron_inst15.feature_last   (feature_last_sr[15]);
        conv_neuron_inst15.weight_stream  (weight_fanout_sr[15]);
        conv_neuron_inst15.weight_first   (weight_first_valid[15]);
        conv_neuron_inst15.weight_last    (weight_last_valid[15]);
        conv_neuron_inst15.output_stream  (output_sig[15]);
        conv_neuron_inst15.output_valid   (output_valid_sig[15]);

        conv_neuron_inst16.clk		(clk);
        conv_neuron_inst16.rst		(rst);
        conv_neuron_inst16.feature_stream (feature_fanout_sr[16]);
        conv_neuron_inst16.feature_first  (feature_first_sr[16]);
        conv_neuron_inst16.feature_last   (feature_last_sr[16]);
        conv_neuron_inst16.weight_stream  (weight_fanout_sr[16]);
        conv_neuron_inst16.weight_first   (weight_first_valid[16]);
        conv_neuron_inst16.weight_last    (weight_last_valid[16]);
        conv_neuron_inst16.output_stream  (output_sig[16]);
        conv_neuron_inst16.output_valid   (output_valid_sig[16]);

        conv_neuron_inst17.clk		(clk);
        conv_neuron_inst17.rst		(rst);
        conv_neuron_inst17.feature_stream (feature_fanout_sr[17]);
        conv_neuron_inst17.feature_first  (feature_first_sr[17]);
        conv_neuron_inst17.feature_last   (feature_last_sr[17]);
        conv_neuron_inst17.weight_stream  (weight_fanout_sr[17]);
        conv_neuron_inst17.weight_first   (weight_first_valid[17]);
        conv_neuron_inst17.weight_last    (weight_last_valid[17]);
        conv_neuron_inst17.output_stream  (output_sig[17]);
        conv_neuron_inst17.output_valid   (output_valid_sig[17]);

        conv_neuron_inst18.clk		(clk);
        conv_neuron_inst18.rst		(rst);
        conv_neuron_inst18.feature_stream (feature_fanout_sr[18]);
        conv_neuron_inst18.feature_first  (feature_first_sr[18]);
        conv_neuron_inst18.feature_last   (feature_last_sr[18]);
        conv_neuron_inst18.weight_stream  (weight_fanout_sr[18]);
        conv_neuron_inst18.weight_first   (weight_first_valid[18]);
        conv_neuron_inst18.weight_last    (weight_last_valid[18]);
        conv_neuron_inst18.output_stream  (output_sig[18]);
        conv_neuron_inst18.output_valid   (output_valid_sig[18]);

        conv_neuron_inst19.clk		(clk);
        conv_neuron_inst19.rst		(rst);
        conv_neuron_inst19.feature_stream (feature_fanout_sr[19]);
        conv_neuron_inst19.feature_first  (feature_first_sr[19]);
        conv_neuron_inst19.feature_last   (feature_last_sr[19]);
        conv_neuron_inst19.weight_stream  (weight_fanout_sr[19]);
        conv_neuron_inst19.weight_first   (weight_first_valid[19]);
        conv_neuron_inst19.weight_last    (weight_last_valid[19]);
        conv_neuron_inst19.output_stream  (output_sig[19]);
        conv_neuron_inst19.output_valid   (output_valid_sig[19]);

        conv_neuron_inst20.clk		(clk);
        conv_neuron_inst20.rst		(rst);
        conv_neuron_inst20.feature_stream (feature_fanout_sr[20]);
        conv_neuron_inst20.feature_first  (feature_first_sr[20]);
        conv_neuron_inst20.feature_last   (feature_last_sr[20]);
        conv_neuron_inst20.weight_stream  (weight_fanout_sr[20]);
        conv_neuron_inst20.weight_first   (weight_first_valid[20]);
        conv_neuron_inst20.weight_last    (weight_last_valid[20]);
        conv_neuron_inst20.output_stream  (output_sig[20]);
        conv_neuron_inst20.output_valid   (output_valid_sig[20]);

        conv_neuron_inst21.clk		(clk);
        conv_neuron_inst21.rst		(rst);
        conv_neuron_inst21.feature_stream (feature_fanout_sr[21]);
        conv_neuron_inst21.feature_first  (feature_first_sr[21]);
        conv_neuron_inst21.feature_last   (feature_last_sr[21]);
        conv_neuron_inst21.weight_stream  (weight_fanout_sr[21]);
        conv_neuron_inst21.weight_first   (weight_first_valid[21]);
        conv_neuron_inst21.weight_last    (weight_last_valid[21]);
        conv_neuron_inst21.output_stream  (output_sig[21]);
        conv_neuron_inst21.output_valid   (output_valid_sig[21]);

        conv_neuron_inst22.clk		(clk);
        conv_neuron_inst22.rst		(rst);
        conv_neuron_inst22.feature_stream (feature_fanout_sr[22]);
        conv_neuron_inst22.feature_first  (feature_first_sr[22]);
        conv_neuron_inst22.feature_last   (feature_last_sr[22]);
        conv_neuron_inst22.weight_stream  (weight_fanout_sr[22]);
        conv_neuron_inst22.weight_first   (weight_first_valid[22]);
        conv_neuron_inst22.weight_last    (weight_last_valid[22]);
        conv_neuron_inst22.output_stream  (output_sig[22]);
        conv_neuron_inst22.output_valid   (output_valid_sig[22]);

        conv_neuron_inst23.clk		(clk);
        conv_neuron_inst23.rst		(rst);
        conv_neuron_inst23.feature_stream (feature_fanout_sr[23]);
        conv_neuron_inst23.feature_first  (feature_first_sr[23]);
        conv_neuron_inst23.feature_last   (feature_last_sr[23]);
        conv_neuron_inst23.weight_stream  (weight_fanout_sr[23]);
        conv_neuron_inst23.weight_first   (weight_first_valid[23]);
        conv_neuron_inst23.weight_last    (weight_last_valid[23]);
        conv_neuron_inst23.output_stream  (output_sig[23]);
        conv_neuron_inst23.output_valid   (output_valid_sig[23]);

        conv_neuron_inst24.clk		(clk);
        conv_neuron_inst24.rst		(rst);
        conv_neuron_inst24.feature_stream (feature_fanout_sr[24]);
        conv_neuron_inst24.feature_first  (feature_first_sr[24]);
        conv_neuron_inst24.feature_last   (feature_last_sr[24]);
        conv_neuron_inst24.weight_stream  (weight_fanout_sr[24]);
        conv_neuron_inst24.weight_first   (weight_first_valid[24]);
        conv_neuron_inst24.weight_last    (weight_last_valid[24]);
        conv_neuron_inst24.output_stream  (output_sig[24]);
        conv_neuron_inst24.output_valid   (output_valid_sig[24]);

        conv_neuron_inst25.clk		(clk);
        conv_neuron_inst25.rst		(rst);
        conv_neuron_inst25.feature_stream (feature_fanout_sr[25]);
        conv_neuron_inst25.feature_first  (feature_first_sr[25]);
        conv_neuron_inst25.feature_last   (feature_last_sr[25]);
        conv_neuron_inst25.weight_stream  (weight_fanout_sr[25]);
        conv_neuron_inst25.weight_first   (weight_first_valid[25]);
        conv_neuron_inst25.weight_last    (weight_last_valid[25]);
        conv_neuron_inst25.output_stream  (output_sig[25]);
        conv_neuron_inst25.output_valid   (output_valid_sig[25]);

        conv_neuron_inst26.clk		(clk);
        conv_neuron_inst26.rst		(rst);
        conv_neuron_inst26.feature_stream (feature_fanout_sr[26]);
        conv_neuron_inst26.feature_first  (feature_first_sr[26]);
        conv_neuron_inst26.feature_last   (feature_last_sr[26]);
        conv_neuron_inst26.weight_stream  (weight_fanout_sr[26]);
        conv_neuron_inst26.weight_first   (weight_first_valid[26]);
        conv_neuron_inst26.weight_last    (weight_last_valid[26]);
        conv_neuron_inst26.output_stream  (output_sig[26]);
        conv_neuron_inst26.output_valid   (output_valid_sig[26]);

        conv_neuron_inst27.clk		(clk);
        conv_neuron_inst27.rst		(rst);
        conv_neuron_inst27.feature_stream (feature_fanout_sr[27]);
        conv_neuron_inst27.feature_first  (feature_first_sr[27]);
        conv_neuron_inst27.feature_last   (feature_last_sr[27]);
        conv_neuron_inst27.weight_stream  (weight_fanout_sr[27]);
        conv_neuron_inst27.weight_first   (weight_first_valid[27]);
        conv_neuron_inst27.weight_last    (weight_last_valid[27]);
        conv_neuron_inst27.output_stream  (output_sig[27]);
        conv_neuron_inst27.output_valid   (output_valid_sig[27]);

        conv_neuron_inst28.clk		(clk);
        conv_neuron_inst28.rst		(rst);
        conv_neuron_inst28.feature_stream (feature_fanout_sr[28]);
        conv_neuron_inst28.feature_first  (feature_first_sr[28]);
        conv_neuron_inst28.feature_last   (feature_last_sr[28]);
        conv_neuron_inst28.weight_stream  (weight_fanout_sr[28]);
        conv_neuron_inst28.weight_first   (weight_first_valid[28]);
        conv_neuron_inst28.weight_last    (weight_last_valid[28]);
        conv_neuron_inst28.output_stream  (output_sig[28]);
        conv_neuron_inst28.output_valid   (output_valid_sig[28]);

        conv_neuron_inst29.clk		(clk);
        conv_neuron_inst29.rst		(rst);
        conv_neuron_inst29.feature_stream (feature_fanout_sr[29]);
        conv_neuron_inst29.feature_first  (feature_first_sr[29]);
        conv_neuron_inst29.feature_last   (feature_last_sr[29]);
        conv_neuron_inst29.weight_stream  (weight_fanout_sr[29]);
        conv_neuron_inst29.weight_first   (weight_first_valid[29]);
        conv_neuron_inst29.weight_last    (weight_last_valid[29]);
        conv_neuron_inst29.output_stream  (output_sig[29]);
        conv_neuron_inst29.output_valid   (output_valid_sig[29]);

        conv_neuron_inst30.clk		(clk);
        conv_neuron_inst30.rst		(rst);
        conv_neuron_inst30.feature_stream (feature_fanout_sr[30]);
        conv_neuron_inst30.feature_first  (feature_first_sr[30]);
        conv_neuron_inst30.feature_last   (feature_last_sr[30]);
        conv_neuron_inst30.weight_stream  (weight_fanout_sr[30]);
        conv_neuron_inst30.weight_first   (weight_first_valid[30]);
        conv_neuron_inst30.weight_last    (weight_last_valid[30]);
        conv_neuron_inst30.output_stream  (output_sig[30]);
        conv_neuron_inst30.output_valid   (output_valid_sig[30]);

        conv_neuron_inst31.clk		(clk);
        conv_neuron_inst31.rst		(rst);
        conv_neuron_inst31.feature_stream (feature_fanout_sr[31]);
        conv_neuron_inst31.feature_first  (feature_first_sr[31]);
        conv_neuron_inst31.feature_last   (feature_last_sr[31]);
        conv_neuron_inst31.weight_stream  (weight_fanout_sr[31]);
        conv_neuron_inst31.weight_first   (weight_first_valid[31]);
        conv_neuron_inst31.weight_last    (weight_last_valid[31]);
        conv_neuron_inst31.output_stream  (output_sig[31]);
        conv_neuron_inst31.output_valid   (output_valid_sig[31]);

        conv_neuron_inst32.clk		(clk);
        conv_neuron_inst32.rst		(rst);
        conv_neuron_inst32.feature_stream (feature_fanout_sr[32]);
        conv_neuron_inst32.feature_first  (feature_first_sr[32]);
        conv_neuron_inst32.feature_last   (feature_last_sr[32]);
        conv_neuron_inst32.weight_stream  (weight_fanout_sr[32]);
        conv_neuron_inst32.weight_first   (weight_first_valid[32]);
        conv_neuron_inst32.weight_last    (weight_last_valid[32]);
        conv_neuron_inst32.output_stream  (output_sig[32]);
        conv_neuron_inst32.output_valid   (output_valid_sig[32]);

        conv_neuron_inst33.clk		(clk);
        conv_neuron_inst33.rst		(rst);
        conv_neuron_inst33.feature_stream (feature_fanout_sr[33]);
        conv_neuron_inst33.feature_first  (feature_first_sr[33]);
        conv_neuron_inst33.feature_last   (feature_last_sr[33]);
        conv_neuron_inst33.weight_stream  (weight_fanout_sr[33]);
        conv_neuron_inst33.weight_first   (weight_first_valid[33]);
        conv_neuron_inst33.weight_last    (weight_last_valid[33]);
        conv_neuron_inst33.output_stream  (output_sig[33]);
        conv_neuron_inst33.output_valid   (output_valid_sig[33]);

        conv_neuron_inst34.clk		(clk);
        conv_neuron_inst34.rst		(rst);
        conv_neuron_inst34.feature_stream (feature_fanout_sr[34]);
        conv_neuron_inst34.feature_first  (feature_first_sr[34]);
        conv_neuron_inst34.feature_last   (feature_last_sr[34]);
        conv_neuron_inst34.weight_stream  (weight_fanout_sr[34]);
        conv_neuron_inst34.weight_first   (weight_first_valid[34]);
        conv_neuron_inst34.weight_last    (weight_last_valid[34]);
        conv_neuron_inst34.output_stream  (output_sig[34]);
        conv_neuron_inst34.output_valid   (output_valid_sig[34]);

        conv_neuron_inst35.clk		(clk);
        conv_neuron_inst35.rst		(rst);
        conv_neuron_inst35.feature_stream (feature_fanout_sr[35]);
        conv_neuron_inst35.feature_first  (feature_first_sr[35]);
        conv_neuron_inst35.feature_last   (feature_last_sr[35]);
        conv_neuron_inst35.weight_stream  (weight_fanout_sr[35]);
        conv_neuron_inst35.weight_first   (weight_first_valid[35]);
        conv_neuron_inst35.weight_last    (weight_last_valid[35]);
        conv_neuron_inst35.output_stream  (output_sig[35]);
        conv_neuron_inst35.output_valid   (output_valid_sig[35]);

        conv_neuron_inst36.clk		(clk);
        conv_neuron_inst36.rst		(rst);
        conv_neuron_inst36.feature_stream (feature_fanout_sr[36]);
        conv_neuron_inst36.feature_first  (feature_first_sr[36]);
        conv_neuron_inst36.feature_last   (feature_last_sr[36]);
        conv_neuron_inst36.weight_stream  (weight_fanout_sr[36]);
        conv_neuron_inst36.weight_first   (weight_first_valid[36]);
        conv_neuron_inst36.weight_last    (weight_last_valid[36]);
        conv_neuron_inst36.output_stream  (output_sig[36]);
        conv_neuron_inst36.output_valid   (output_valid_sig[36]);

        conv_neuron_inst37.clk		(clk);
        conv_neuron_inst37.rst		(rst);
        conv_neuron_inst37.feature_stream (feature_fanout_sr[37]);
        conv_neuron_inst37.feature_first  (feature_first_sr[37]);
        conv_neuron_inst37.feature_last   (feature_last_sr[37]);
        conv_neuron_inst37.weight_stream  (weight_fanout_sr[37]);
        conv_neuron_inst37.weight_first   (weight_first_valid[37]);
        conv_neuron_inst37.weight_last    (weight_last_valid[37]);
        conv_neuron_inst37.output_stream  (output_sig[37]);
        conv_neuron_inst37.output_valid   (output_valid_sig[37]);

        conv_neuron_inst38.clk		(clk);
        conv_neuron_inst38.rst		(rst);
        conv_neuron_inst38.feature_stream (feature_fanout_sr[38]);
        conv_neuron_inst38.feature_first  (feature_first_sr[38]);
        conv_neuron_inst38.feature_last   (feature_last_sr[38]);
        conv_neuron_inst38.weight_stream  (weight_fanout_sr[38]);
        conv_neuron_inst38.weight_first   (weight_first_valid[38]);
        conv_neuron_inst38.weight_last    (weight_last_valid[38]);
        conv_neuron_inst38.output_stream  (output_sig[38]);
        conv_neuron_inst38.output_valid   (output_valid_sig[38]);

        conv_neuron_inst39.clk		(clk);
        conv_neuron_inst39.rst		(rst);
        conv_neuron_inst39.feature_stream (feature_fanout_sr[39]);
        conv_neuron_inst39.feature_first  (feature_first_sr[39]);
        conv_neuron_inst39.feature_last   (feature_last_sr[39]);
        conv_neuron_inst39.weight_stream  (weight_fanout_sr[39]);
        conv_neuron_inst39.weight_first   (weight_first_valid[39]);
        conv_neuron_inst39.weight_last    (weight_last_valid[39]);
        conv_neuron_inst39.output_stream  (output_sig[39]);
        conv_neuron_inst39.output_valid   (output_valid_sig[39]);

        conv_neuron_inst40.clk		(clk);
        conv_neuron_inst40.rst		(rst);
        conv_neuron_inst40.feature_stream (feature_fanout_sr[40]);
        conv_neuron_inst40.feature_first  (feature_first_sr[40]);
        conv_neuron_inst40.feature_last   (feature_last_sr[40]);
        conv_neuron_inst40.weight_stream  (weight_fanout_sr[40]);
        conv_neuron_inst40.weight_first   (weight_first_valid[40]);
        conv_neuron_inst40.weight_last    (weight_last_valid[40]);
        conv_neuron_inst40.output_stream  (output_sig[40]);
        conv_neuron_inst40.output_valid   (output_valid_sig[40]);

        conv_neuron_inst41.clk		(clk);
        conv_neuron_inst41.rst		(rst);
        conv_neuron_inst41.feature_stream (feature_fanout_sr[41]);
        conv_neuron_inst41.feature_first  (feature_first_sr[41]);
        conv_neuron_inst41.feature_last   (feature_last_sr[41]);
        conv_neuron_inst41.weight_stream  (weight_fanout_sr[41]);
        conv_neuron_inst41.weight_first   (weight_first_valid[41]);
        conv_neuron_inst41.weight_last    (weight_last_valid[41]);
        conv_neuron_inst41.output_stream  (output_sig[41]);
        conv_neuron_inst41.output_valid   (output_valid_sig[41]);

        conv_neuron_inst42.clk		(clk);
        conv_neuron_inst42.rst		(rst);
        conv_neuron_inst42.feature_stream (feature_fanout_sr[42]);
        conv_neuron_inst42.feature_first  (feature_first_sr[42]);
        conv_neuron_inst42.feature_last   (feature_last_sr[42]);
        conv_neuron_inst42.weight_stream  (weight_fanout_sr[42]);
        conv_neuron_inst42.weight_first   (weight_first_valid[42]);
        conv_neuron_inst42.weight_last    (weight_last_valid[42]);
        conv_neuron_inst42.output_stream  (output_sig[42]);
        conv_neuron_inst42.output_valid   (output_valid_sig[42]);

        conv_neuron_inst43.clk		(clk);
        conv_neuron_inst43.rst		(rst);
        conv_neuron_inst43.feature_stream (feature_fanout_sr[43]);
        conv_neuron_inst43.feature_first  (feature_first_sr[43]);
        conv_neuron_inst43.feature_last   (feature_last_sr[43]);
        conv_neuron_inst43.weight_stream  (weight_fanout_sr[43]);
        conv_neuron_inst43.weight_first   (weight_first_valid[43]);
        conv_neuron_inst43.weight_last    (weight_last_valid[43]);
        conv_neuron_inst43.output_stream  (output_sig[43]);
        conv_neuron_inst43.output_valid   (output_valid_sig[43]);

        conv_neuron_inst44.clk		(clk);
        conv_neuron_inst44.rst		(rst);
        conv_neuron_inst44.feature_stream (feature_fanout_sr[44]);
        conv_neuron_inst44.feature_first  (feature_first_sr[44]);
        conv_neuron_inst44.feature_last   (feature_last_sr[44]);
        conv_neuron_inst44.weight_stream  (weight_fanout_sr[44]);
        conv_neuron_inst44.weight_first   (weight_first_valid[44]);
        conv_neuron_inst44.weight_last    (weight_last_valid[44]);
        conv_neuron_inst44.output_stream  (output_sig[44]);
        conv_neuron_inst44.output_valid   (output_valid_sig[44]);

        conv_neuron_inst45.clk		(clk);
        conv_neuron_inst45.rst		(rst);
        conv_neuron_inst45.feature_stream (feature_fanout_sr[45]);
        conv_neuron_inst45.feature_first  (feature_first_sr[45]);
        conv_neuron_inst45.feature_last   (feature_last_sr[45]);
        conv_neuron_inst45.weight_stream  (weight_fanout_sr[45]);
        conv_neuron_inst45.weight_first   (weight_first_valid[45]);
        conv_neuron_inst45.weight_last    (weight_last_valid[45]);
        conv_neuron_inst45.output_stream  (output_sig[45]);
        conv_neuron_inst45.output_valid   (output_valid_sig[45]);

        conv_neuron_inst46.clk		(clk);
        conv_neuron_inst46.rst		(rst);
        conv_neuron_inst46.feature_stream (feature_fanout_sr[46]);
        conv_neuron_inst46.feature_first  (feature_first_sr[46]);
        conv_neuron_inst46.feature_last   (feature_last_sr[46]);
        conv_neuron_inst46.weight_stream  (weight_fanout_sr[46]);
        conv_neuron_inst46.weight_first   (weight_first_valid[46]);
        conv_neuron_inst46.weight_last    (weight_last_valid[46]);
        conv_neuron_inst46.output_stream  (output_sig[46]);
        conv_neuron_inst46.output_valid   (output_valid_sig[46]);

        conv_neuron_inst47.clk		(clk);
        conv_neuron_inst47.rst		(rst);
        conv_neuron_inst47.feature_stream (feature_fanout_sr[47]);
        conv_neuron_inst47.feature_first  (feature_first_sr[47]);
        conv_neuron_inst47.feature_last   (feature_last_sr[47]);
        conv_neuron_inst47.weight_stream  (weight_fanout_sr[47]);
        conv_neuron_inst47.weight_first   (weight_first_valid[47]);
        conv_neuron_inst47.weight_last    (weight_last_valid[47]);
        conv_neuron_inst47.output_stream  (output_sig[47]);
        conv_neuron_inst47.output_valid   (output_valid_sig[47]);

        conv_neuron_inst48.clk		(clk);
        conv_neuron_inst48.rst		(rst);
        conv_neuron_inst48.feature_stream (feature_fanout_sr[48]);
        conv_neuron_inst48.feature_first  (feature_first_sr[48]);
        conv_neuron_inst48.feature_last   (feature_last_sr[48]);
        conv_neuron_inst48.weight_stream  (weight_fanout_sr[48]);
        conv_neuron_inst48.weight_first   (weight_first_valid[48]);
        conv_neuron_inst48.weight_last    (weight_last_valid[48]);
        conv_neuron_inst48.output_stream  (output_sig[48]);
        conv_neuron_inst48.output_valid   (output_valid_sig[48]);



    }


};


#endif	/*  */
