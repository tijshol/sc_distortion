#ifndef CONV_NEURON_H
#define CONV_NEURON_H

#define FEATURE_WIDTH	8
#define WEIGHT_WIDTH	8
#define WEIGHT_MEM_ORDER	5
#define OUTPUT_WIDTH	8
#define OUTPUT_SHIFT	0
#define BIAS_SHIFT	0
#define RELU	1

#define     WEIGHT_MEM_SIZE  (20 * WEIGHT_MEM_ORDER)
#define     ACC_SIZE ( FEATURE_WIDTH + WEIGHT_WIDTH + WEIGHT_MEM_ORDER)
#define     P_SIZE ( FEATURE_WIDTH + WEIGHT_WIDTH + 1)

#include <systemc.h>

SC_MODULE(conv_neuron) {
    sc_in<bool >  clk;
    sc_in<bool >  rst;
    sc_in<sc_uint<FEATURE_WIDTH> >  feature_stream;
    sc_in<bool >  feature_first;
    sc_in<bool >  feature_last;
    sc_in<sc_uint<WEIGHT_WIDTH> >  weight_stream;
    sc_in<bool >  weight_first;
    sc_in<bool >  weight_last;
    sc_out<sc_uint<OUTPUT_WIDTH> > output_stream;
    sc_out<bool > output_valid;

    typedef sc_uint<WEIGHT_WIDTH> WEIGHT_MEM ;
    sc_signal<WEIGHT_MEM > wmem[WEIGHT_MEM_SIZE];

    sc_signal<sc_uint<WEIGHT_MEM_ORDER> > wmem_waddr;
    sc_signal<sc_uint<WEIGHT_MEM_ORDER> > wmem_raddr;
    sc_signal<sc_uint<WEIGHT_WIDTH> > bias;
    sc_signal<bool > wmem_writing;

    sc_signal<sc_bigint<P_SIZE> > p;
    sc_signal<sc_bigint<ACC_SIZE> > acc;
    sc_signal<sc_uint<64> > zeros;
    sc_signal<sc_biguint<ACC_SIZE> > acc_slv;

    sc_signal<sc_uint<FEATURE_WIDTH> > feature_stream_reg;
    sc_signal<sc_uint<FEATURE_WIDTH> > feature_stream_r1;
    sc_signal<sc_uint<FEATURE_WIDTH> > feature_stream_r2;
    sc_signal<sc_uint<FEATURE_WIDTH> > feature_stream_r3;
    sc_signal<sc_uint<WEIGHT_WIDTH> > weight_reg;
    sc_signal<sc_uint<WEIGHT_WIDTH> > weight_out;
    sc_signal<sc_uint<WEIGHT_WIDTH> > weight_out2;

    sc_signal<bool > feature_first_r1;
    sc_signal<bool > feature_first_r2;
    sc_signal<bool > feature_first_r3;
    sc_signal<bool > feature_first_r4;
    sc_signal<bool > feature_first_r5;
    sc_signal<bool > feature_last_r1;
    sc_signal<bool > feature_last_r2;
    sc_signal<bool > feature_last_r3;
    sc_signal<bool > feature_last_r4;
    sc_signal<bool > feature_last_r5;
    sc_signal<bool > feature_last_r6;
    sc_signal<bool > feature_last_r7;

    void conv_neuron_main();

    SC_CTOR (conv_neuron)
    {
        SC_CTHREAD(conv_neuron_main, clk.pos()  );

        reset_signal_is (rst, true);
    }

};
#endif
