#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <systemc.h>

#include "conv_neuron_layer.h"
#include "tb_conv_neuron_layer.h"
using namespace std;
void
tb_conv_neuron_layer::source ()
{
    int int_rand;
    int int_file;
    string line;

    printf ("Test Bench for Convolutional Neuron\n");
    printf ("Parameters: \n");
    printf ("Feature planes, mask size: ");
    printf ("%d", NO_FEATURE_PLANES);
    printf (" x ");
    printf ("%d", MASK_HEIGHT);
    printf (" x ");
    printf ("%d", MASK_WIDTH);
    printf (" = ");
    printf ("%d", NO_WEIGHTS);
    printf (" weights\n");
    printf ("Feature bit_width: ");
    printf ("%d\n", FEATURE_WIDTH);
    printf ("Weight bit_width: ");
    printf ("%d\n", WEIGHT_WIDTH);
    printf ("Output bit_width: ");
    printf ("%d\n", OUTPUT_WIDTH);
    printf ("Output scaling: 2^-");
    printf ("%d\n", OUTPUT_SHIFT);
    if (RELU)
    {
        printf ("Using ReLU non-linearity\n");
    }

    // Reset

    weight_first.write ((bool) (0));
    weight_last.write ((bool) (0));
    weight_stream.write ((sc_biguint < WEIGHT_WIDTH >) (0));
    weight_id.write ((sc_biguint < WEIGHT_WIDTH >) (0));
    feature_first.write ((bool) (0));
    feature_last.write ((bool) (0));
    feature_stream.write ((sc_biguint < FEATURE_WIDTH >) (0));
    rst.write (1);
    wait ();
    rst.write (0);
    wait ();

    for (int n = 1; n <= LAYER_SIZE; n++)
    {
        printf ("Generating weights, for neuron #");
        printf ("%d\n", n);
        weight_id.write ((sc_biguint < LAYER_SIZE_ORDER >) (n));
        weight_first.write ((bool) (1));

        int_rand = 177;
        if (bias_file.is_open ())
        {
            getline (bias_file, line);
            sscanf (line.c_str(), "%d", &int_file);
        }
        else
        {
            printf ("Input file too small\n");
        }

        weight_stream.write ((sc_biguint < WEIGHT_WIDTH >) (int_file));
        printf ("Bias: ");
        printf ("%d\n", int_file);
        wait ();
        weight_first.write ((bool) (0));
        int_rand = 1;

        for (int j = 0; j < MASK_HEIGHT; j++)
        {
            for (int k = 0; k < MASK_WIDTH; k++)
            {
                for (int i = 0; i < NO_FEATURE_PLANES; i++)
                {
                    if (weight_file.is_open ()) {
                        getline (weight_file, line);
                        sscanf (line.c_str(), "%d", &int_file);
                    }
                    else {
                        printf ("Input file too small\n");
                    }
                    weight_stream.write ((sc_biguint < WEIGHT_WIDTH >) (int_file));
                    int_rand++;
                    if (((i == NO_FEATURE_PLANES-1) && (j == MASK_HEIGHT-1)) && (k == MASK_WIDTH-1))
                    {
                        weight_last.write ((bool) (1));
                    }
                    wait();
                }
            }
        }
        weight_last.write ((bool) (0));
        wait ();  // one tick between last first
    }
    for (int repeats = 0; repeats < 100; repeats++)
    {
        feature_last.write ((bool) (0));
        wait (); // one tick last first
        int_rand = 1;
        for (int i = 0; i < NO_FEATURE_PLANES; i++)
        {
            for (int j = 0; j < MASK_HEIGHT; j++)
            {
                for (int k = 0; k < MASK_WIDTH; k++)
                {
                    if (input_file.is_open ()) {
                        getline (input_file, line);
                        sscanf (line.c_str(), "%d", &int_file);
                    }
                    else {
                        printf ("Input file too small\n");
                    }
                    feature_stream.write ((sc_biguint < FEATURE_WIDTH >) (int_file));
                    int_rand++;

                    if (((i == 0) && (j == 0)) && (k == 0))
                    {
                        feature_first.write ((bool) (1));
                    }
                    else
                    {
                        feature_first.write ((bool) (0));
                    }
                    if (((i == NO_FEATURE_PLANES-1) && (j == MASK_HEIGHT-1)) && (k == MASK_WIDTH-1))
                    {
                        feature_last.write ((bool) (1));
                    }
                    wait ();
                }
            }
        }
    }
    feature_last.write ((bool) (0));
}

void
tb_conv_neuron_layer::sink ()
{
    int int_output;
    for (;;)
    {
        wait ();
        {
            if (output_valid.read () == 1)
            {
                printf ("Output(");
                int_output = output_id.read ();
                printf ("Id: %d", int_output);
                if (RELU)
                {
                    int_output = output_stream.read ();
                }
                else
                {
                    int_output = output_stream.read ().to_int ();
                }
                printf (") ");
                printf ("Streamdata :%d\n", int_output);
                output_file << int_output << endl;
            }
        }
    }
}
