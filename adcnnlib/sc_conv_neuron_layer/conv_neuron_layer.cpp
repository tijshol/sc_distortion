#include "conv_neuron_layer.h"

void
conv_neuron_layer::conv_neuron_layer_main ()
{

    id_fanout_type i;

// reset code
    // reset internal variables

    // reset outputs

    while (true) {

        {
            feature_fanout_sr[0].write( feature_stream.read ());
            feature_first_sr[0].write (feature_first.read ());
            feature_last_sr[0].write (feature_last.read ());
            weight_fanout_sr[0].write (weight_stream.read ());
            weight_id_fanout_sr[0].write (weight_id.read ());
            weight_first_sr[0].write (weight_first.read ());
            weight_last_sr[0].write (weight_last.read ());

            for (unsigned i = 1; i <= LAYER_SIZE; i++)
            {
                feature_fanout_sr[i].write ( feature_fanout_sr[(i - 1)].read());
                feature_first_sr[i].write (feature_first_sr[(i - 1)].read());
                feature_last_sr[i].write (feature_last_sr[(i - 1)].read());
                weight_fanout_sr[i].write( weight_fanout_sr[(i - 1)].read());
                weight_first_sr[i].write  (weight_first_sr[(i - 1)].read());
                weight_last_sr[i].write (weight_last_sr[(i - 1)].read());
                weight_id_fanout_sr[i].write( weight_id_fanout_sr[(i - 1)].read());

                if ((weight_id_fanout_sr[(i - 1)].read()) == i)
                {
                    weight_first_valid[i].write (weight_first_sr[(i - 1)].read());
                    weight_last_valid[i].write (weight_last_sr[(i - 1)].read());
                }
                else
                {
                    weight_first_valid[i].write (0);
                    weight_last_valid[i].write  (0);
                }
                if (output_valid_sig[i].read() == 1)
                {
                    output_valid_sr[(i - 1)].write (1);
                    output_fanin_sr[(i - 1)].write( output_sig[i].read());
                    output_id_fanin_sr[(i - 1)].write( i);
                }
                else
                {
                    output_valid_sr[(i - 1)].write ( (output_valid_sr[i]).read());
                    output_fanin_sr[(i - 1)].write( output_fanin_sr[i].read());
                    output_id_fanin_sr[(i - 1)].write( output_id_fanin_sr[i].read());
                }
            }
        }
        wait();
    }
}

void
conv_neuron_layer::conv_neuron_layer_output ()
{
    output_valid.write(  ((bool) (output_valid_sr[0].read())));
    output_id.write(  ((output_id_fanin_sr[0].read())));
    output_stream.write(  ((sc_uint < OUTPUT_WIDTH  >) (output_fanin_sr[0].read())));
}
