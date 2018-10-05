#include <iostream>
#include <fstream>
using namespace std;

#include "sim_anet_layer0.h"

void
sim_anet_layer0::process_line106 ()
{

    int outcount = 0;
    int int_output = 0;
    int int_file;
    int weight_pos;

    int start_row;
    int start_col;
    string line ;

    sc_int < INPUT_FEATURE_WIDTH > signed_feature;
    sc_int < (INPUT_FEATURE_WIDTH + WEIGHT_WIDTH) > neuron_product;
    sc_int < (NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH + 1 > neuron_acc;
    sc_uint < OUTPUT_WIDTH > neuron_out;


    //
    //Initialize Neurons with weights
    //

    for (int n = 0; n < LAYER_SIZE; n++) {
        printf ("Reading in weights, for neuron #");
        printf ("%d\n", n);

        if (bias_file.is_open ())
        {
            getline (bias_file, line);
            sscanf (line.c_str(), "%d", &int_file);
        }
        else
        {
            printf ("Input file too small\n");
        }

        int_file = (int_file / BIAS_SCALING);
        bias[n] = int_file;
        printf ("Bias: ");
        printf ("%d\n", int_file);
        weight_pos = 0;

        for (int j = 0; j < INPUT_MASK_HEIGHT; j++) {
            for (int k = 0; k < INPUT_MASK_WIDTH; k++) {
                for (int i = 0; i < INPUT_NO_FEATURES; i++) {
                    if (weight_file.is_open ()) {
                        getline (weight_file, line);
                        sscanf (line.c_str(), "%d", &int_file);
                    }
                    else {
                        printf ("Input file too small\n");
                    }
                    int_file = (int_file / WEIGHT_SCALING);
                    if (i == 0)
                    {
                        weights[n][(weight_pos + 2)] = (sc_int<WEIGHT_WIDTH>)int_file;
                    }
                    else if (i == 1)
                    {
                        weights[n][weight_pos] = (sc_int<WEIGHT_WIDTH>)int_file;
                    }
                    else
                    {
                        weights[n][(weight_pos - 2)] = (sc_int<WEIGHT_WIDTH>)int_file;
                    }
                    weight_pos = (weight_pos + 1);
                    //printf ("[%d][%d][%d]w[%d](%d) \n", j, k, i, weight_pos, int_file);
                }
                //printf (" : \n");
            }
            //printf (" \n");
        }
    }

    //
    // Read image data in layer input
    //

    //for (int batch = 0; batch < 1; batch++) {
    printf ("Reading File\n");
    for (int i = 0; i < INPUT_FEATURE_PLANE_HEIGHT; i++) {
        for (int j = 0; j < INPUT_FEATURE_PLANE_WIDTH; j++) {
            for (int k = 0; k < INPUT_NO_FEATURES; k++) {

                if (input_file.is_open ()) {
                    getline (input_file, line);
                    sscanf (line.c_str(), "%d", &int_file);
                }
                else {
                    printf ("Input file too small\n");
                }

                int_file = (int_file - INPUT_BIAS);
                input_tensor[i][j][k] = int_file;
            }
        }
    }


    //
    // Scan across input image
    //

    printf ("Running Neuron layer\n");
    for (int i = 0; i < POST_FILTER_HEIGHT; i++) {
        printf ("Line #");
        printf ("%d\n", i);
        for (int j = 0; j < POST_FILTER_WIDTH; j++) {
            //start_row = ((INPUT_STRIDE * (i+1)) - 5);
            //start_col = ((INPUT_STRIDE * (j+1)) - 5);
            start_row = ((INPUT_STRIDE * (i)) - 1 );
            start_col = ((INPUT_STRIDE * (j)) - 1 );
            for (int ii = 0; ii < INPUT_MASK_HEIGHT; ii++) {
                for (int jj = 0; jj < INPUT_MASK_WIDTH; jj++) {
                    for (int kk = 0; kk < INPUT_NO_FEATURES; kk++) {
                        if (((((start_row + ii) >= 0) && ((start_row + ii) < INPUT_FEATURE_PLANE_HEIGHT))
                           && ((start_col + jj) >= 0)) && ((start_col + jj) < INPUT_FEATURE_PLANE_WIDTH)) {
                            mask_region_tensor[ii][jj][kk] = input_tensor[(start_row + ii)][(start_col + jj)][kk];
                        }
                        else {
                            mask_region_tensor[ii][jj][kk] = 0;
                        }
//        printf ("start_row = %d, start_col = %d,  mask_region_tensor[%d][%d][%d] = %d\n", start_row, start_col, ii, jj, kk, (int)mask_region_tensor[ii][jj][kk] );
                    }
                }
            }

            for (int n = 0; n < LAYER_SIZE; n++) {
                neuron_acc = (bias[n]) <<BIAS_SHIFT;
                weight_pos = 0;
                for (int ii = 0; ii < INPUT_MASK_HEIGHT; ii++) {
                    for (int jj = 0; jj < INPUT_MASK_WIDTH; jj++) {
                        for (int kk = 0; kk < INPUT_NO_FEATURES; kk++) {
                            signed_feature = mask_region_tensor[ii][jj][kk];
                            neuron_product = (signed_feature * weights[n][weight_pos]);
                            weight_pos = (weight_pos + 1);
                            neuron_acc = neuron_acc + neuron_product;
                        }
                    }
                }

                if (neuron_acc [((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH)] == 1) {
                    neuron_out = 0;
                }
                else {
                    if ((OUTPUT_SHIFT + OUTPUT_WIDTH) < (((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) +
                                                          WEIGHT_WIDTH) - 1)) {
                        if (neuron_acc ((((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH) - 1),
                                        (OUTPUT_SHIFT + OUTPUT_WIDTH)) != 0) {
                            neuron_out = -1;
                        }
                        else {
                            neuron_out = neuron_acc (((OUTPUT_SHIFT + OUTPUT_WIDTH) - 1), OUTPUT_SHIFT);
                        }
                    }
                    else {
                        neuron_out = neuron_acc (((OUTPUT_SHIFT + OUTPUT_WIDTH) - 1), OUTPUT_SHIFT);
                    }
                }
                post_filter_tensor[i][j][n] = neuron_out;
            }
        }
    }



    for (int i = 0; i < OUTPUT_TENSOR_HEIGHT; i++) {
        printf ("Line #");
        printf ("%d\n", i);
        for (int j = 0; j < OUTPUT_TENSOR_WIDTH; j++) {
            for (int kk = 0; kk < LAYER_SIZE; kk++) {
                output_tensor[i][j][kk] = post_filter_tensor[i][j][kk];
            }
        }
    }

    printf ("Writing output to File\n");
    for (int i = 0; i < OUTPUT_TENSOR_HEIGHT; i++) {
        printf ("Line #");
        printf ("%d\n", i);
        for (int j = 0; j < OUTPUT_TENSOR_WIDTH; j++) {
            for (int kk = 0; kk < LAYER_SIZE; kk++) {
                int_output = (output_tensor[i][j][kk]).to_uint() & 0xff;
                output_file << int_output << endl;
                output_bin.write ((char *)&int_output, 1);
                outcount = (outcount + 1);
            }
        }
    }
    printf ("Closing file: ");
    printf ("%d\n", outcount);
    done.write(true);

    sc_stop ();
}
