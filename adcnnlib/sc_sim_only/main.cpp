#include "systemc.h"

#include "sim_anet_layer0.h"
#include "tb_init.h"


ifstream input_file ("input_data.txt");
ifstream weight_file ("places205CNN_conv1_filter.txt");
ifstream bias_file ("places205CNN_conv1_bias.txt");
ofstream output_file ("output_data.txt");
ofstream output_bin ("output_data.bin", ios::out | ios::binary);


sim_anet_layer0 *top = NULL;


int
sc_main (int argc, char *argv[])
{
    sc_clock s_clk ("clk_sig", 10, SC_NS);
    sc_signal<bool> s_reset;
    sc_signal<bool>	s_start;
    sc_signal<bool>	s_done;

    top = new sim_anet_layer0 ("top");
    tb_init		U_tb_init("U_tb_init");

    U_tb_init.clk(s_clk);
    U_tb_init.reset(s_reset);
    U_tb_init.done(s_done);
    U_tb_init.start(s_start);

    top->clk (s_clk);
    top->start (s_start);
    top->done (s_done);


    printf ("Test Bench for Convolutional Neuron Layer\n");
    printf ("Parameters: \n");
    printf ("Feature planes, mask size: ");
    printf ("%d", INPUT_NO_FEATURES);
    printf (" x ");
    printf ("%d", INPUT_MASK_HEIGHT);
    printf (" x ");
    printf ("%d", INPUT_MASK_WIDTH);
    printf (" = ");
    printf ("%d", NO_WEIGHTS);
    printf (" weights\n");
    printf ("Feature plane size: ");
    printf ("%d", INPUT_FEATURE_PLANE_HEIGHT);
    printf (" x ");
    printf ("%d\n", INPUT_FEATURE_PLANE_WIDTH);
    printf ("Feature plane stride: ");
    printf ("%d\n", INPUT_STRIDE);
    printf ("Feature bit_width: ");
    printf ("%d\n", INPUT_FEATURE_WIDTH);
    printf ("Weight bit_width: ");
    printf ("%d\n", WEIGHT_WIDTH);
    printf ("Output bit_width: ");
    printf ("%d\n", OUTPUT_WIDTH);
    printf ("Output scaling: 2^-");
    printf ("%d\n", OUTPUT_SHIFT);
    printf ("Layer size: ");
    printf ("%d\n", LAYER_SIZE);


    sc_start ();
    return 0;

}
