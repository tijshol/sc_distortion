#include "conv_neuron.h"

void
conv_neuron::conv_neuron_main ()
{
    sc_uint < OUTPUT_WIDTH > output_stream_sliced;

// reset code
    // reset internal variables

    wmem_raddr.write ((sc_uint < WEIGHT_MEM_ORDER >) (0));
    wmem_waddr.write ((sc_uint < WEIGHT_MEM_ORDER >) (0));
    wmem_writing.write ((bool) (0));

    // reset outputs
    output_stream.write ((sc_uint < OUTPUT_WIDTH >) (0));
    output_valid.write ((bool) (0));

    wait();
    wait();


    while (true) {
        if (weight_first.read () == 1)
        {
            wmem_writing.write ((bool) (1));
            wmem_waddr.write ((sc_uint < WEIGHT_MEM_ORDER >) (0));
            bias.write ((sc_uint < WEIGHT_WIDTH >) (weight_stream.read ()));
        }
        if (wmem_writing.read () == 1)
        {
            if (weight_last.read () == 1)
            {
                wmem_writing.write ((bool) (0));
            }
            wmem[wmem_waddr.read ()] .write( weight_stream.read ());
            wmem_waddr.write ((wmem_waddr.read () + 1));
        }
        if (feature_first.read () == 1)
        {
            wmem_raddr.write ((sc_uint < WEIGHT_MEM_ORDER >) (0));
        }
        else
        {
            wmem_raddr.write ((wmem_raddr.read () + 1));
        }

        weight_out.write ((sc_uint < WEIGHT_WIDTH >) (wmem[wmem_raddr.read ()].read()));

        weight_out2.write ((sc_uint < WEIGHT_WIDTH >) (weight_out.read ()));
        weight_reg.write ((sc_uint < WEIGHT_WIDTH >) (weight_out2.read ()));

        feature_stream_r1.write ((sc_uint < FEATURE_WIDTH >) (feature_stream.read ()));
        feature_stream_r2.write ((sc_uint < FEATURE_WIDTH >) (feature_stream_r1.read ()));
        feature_stream_r3.write ((sc_uint < FEATURE_WIDTH >) (feature_stream_r2.read ()));
        feature_stream_reg.write ((sc_uint < FEATURE_WIDTH >) (feature_stream_r3.read ()));

        feature_first_r1.write ((bool) (feature_first.read ()));
        feature_first_r2.write ((bool) (feature_first_r1.read ()));
        feature_first_r3.write ((bool) (feature_first_r2.read ()));
        feature_first_r4.write ((bool) (feature_first_r3.read ()));
        feature_first_r5.write ((bool) (feature_first_r4.read ()));

        feature_last_r1.write ((bool) (feature_last.read ()));
        feature_last_r2.write ((bool) (feature_last_r1.read ()));
        feature_last_r3.write ((bool) (feature_last_r2.read ()));
        feature_last_r4.write ((bool) (feature_last_r3.read ()));
        feature_last_r5.write ((bool) (feature_last_r4.read ()));
        feature_last_r6.write ((bool) (feature_last_r5.read ()));
        feature_last_r7.write ((bool) (feature_last_r6.read ()));

#define SIGN_EXTEND(to, from, value) ((sc_int < ACC_SIZE > )((sc_int < ACC_SIZE > )(((sc_int < ACC_SIZE > )value) << (to - from)) >> (to - from)))

        sc_int<WEIGHT_WIDTH>  weight_l;
        sc_bigint<P_SIZE>  weight_p;

        weight_l = weight_reg.read ();
        weight_p =SIGN_EXTEND (P_SIZE, WEIGHT_WIDTH, weight_l);

        p.write (weight_p * ((sc_uint<FEATURE_WIDTH>)feature_stream_reg.read ()));

        // p.write (((sc_int<WEIGHT_WIDTH>)weight_reg.read ()) * ((sc_uint<FEATURE_WIDTH>)feature_stream_reg.read ()));

        if (feature_first_r5.read () == 1)
        {
            sc_int<WEIGHT_WIDTH>  bias_l;
            sc_int < ACC_SIZE > bias_ll;

            sc_bigint < ACC_SIZE > p_l;
            sc_biguint < ACC_SIZE > bias_plus_p;

            bias_l = bias.read ();

            bias_ll =SIGN_EXTEND (ACC_SIZE, WEIGHT_WIDTH, bias_l);

            p_l = ((sc_bigint < ACC_SIZE >) p.read ());
            bias_plus_p = bias_ll + p_l;

            acc.write ( bias_plus_p );
        }
        else
        {
            acc.write ( acc.read () + (sc_biguint < ACC_SIZE >) p.read ());
        }

        acc_slv.write ((sc_biguint < ACC_SIZE >) (acc.read ()));

        if (feature_last_r7.read () == 1)
        {
            if (RELU)
            {
                if (acc_slv.read ()[(ACC_SIZE - 1)] == 1)
                {
                    output_stream_sliced = (sc_uint < OUTPUT_WIDTH >) (0);
                }
                else
                {
                    if (ACC_SIZE == (OUTPUT_WIDTH + OUTPUT_SHIFT))
                    {
                        output_stream_sliced = (sc_uint < OUTPUT_WIDTH >) (acc_slv.read ()((ACC_SIZE - 1), OUTPUT_SHIFT));
                    }
                    else
                    {
                        if (acc_slv.read ()((ACC_SIZE - 1), ((OUTPUT_WIDTH + OUTPUT_SHIFT) - 1)) !=
                                zeros.read ()((ACC_SIZE - 1), ((OUTPUT_WIDTH + OUTPUT_SHIFT) - 1)))
                        {
                            output_stream_sliced[(OUTPUT_WIDTH - 1)] = (sc_uint < OUTPUT_WIDTH >) (0);
                            output_stream_sliced.range (OUTPUT_WIDTH - 2, 0) = (sc_uint < OUTPUT_WIDTH >) (-1);
                        }
                        else
                        {
                            output_stream_sliced = (sc_uint < OUTPUT_WIDTH >) (acc_slv.read ()(((OUTPUT_WIDTH + OUTPUT_SHIFT) - 1), OUTPUT_SHIFT));
                        }
                    }
                }
            }
            else
            {  // NOT RELU
                output_stream_sliced = (sc_uint < OUTPUT_WIDTH >) (acc_slv.read ()(((OUTPUT_WIDTH + OUTPUT_SHIFT) - 1), OUTPUT_SHIFT));
            }
            output_valid.write ((bool) (1));
        }
        else
        {
            output_valid.write ((bool) (0));
        }

        output_stream.write (output_stream_sliced);

        wait();

    }
}


