#ifndef SIM_ANET_LAYER0_H
#define SIM_ANET_LAYER0_H

#include <iostream>
#include <fstream>
#include <systemc.h>

#define INPUT_FEATURE_WIDTH	8
#define INPUT_NO_FEATURES	3
#define INPUT_FEATURE_PLANE_WIDTH	224
#define INPUT_FEATURE_PLANE_HEIGHT	224
#define INPUT_MASK_WIDTH	11
#define INPUT_MASK_HEIGHT	11
#define INPUT_STRIDE	4
#define LAYER_SIZE	96
#define WEIGHT_WIDTH	8
#define OUTPUT_WIDTH	8
#define OUTPUT_SHIFT	6
#define BIAS_SHIFT	3
#define POST_FILTER_WIDTH	110
#define POST_FILTER_HEIGHT	110
#define MAXPOOL_MASK_WIDTH	3
#define MAXPOOL_MASK_HEIGHT	3
#define MAXPOOL_STRIDE	2
#define NO_WEIGHTS	(INPUT_NO_FEATURES * INPUT_MASK_WIDTH * INPUT_MASK_HEIGHT)
#define NO_WEIGHT_BITS	8
#define OUTPUT_TENSOR_WIDTH	55
#define OUTPUT_TENSOR_HEIGHT	55
#define INPUT_BIAS	128
#define WEIGHT_SCALING	(16777216 / 2)
#define BIAS_SCALING	(WEIGHT_SCALING * powl(2 ,BIAS_SHIFT))

// BIAS_VECTOR_TYPE
typedef sc_int <WEIGHT_WIDTH>  bias_vector_type [LAYER_SIZE] ;
// INPUT_TENSOR_TYPE
typedef sc_int <INPUT_FEATURE_WIDTH >  input_tensor_type [INPUT_FEATURE_PLANE_HEIGHT] [INPUT_FEATURE_PLANE_WIDTH] [INPUT_NO_FEATURES] ;
// MASK_REGION_TENSOR_TYPE
typedef sc_int <INPUT_FEATURE_WIDTH > mask_region_tensor_type [INPUT_MASK_HEIGHT] [INPUT_MASK_WIDTH] [INPUT_NO_FEATURES] ;
// OUTPUT_TENSOR_TYPE
typedef sc_int <OUTPUT_WIDTH > output_tensor_type [OUTPUT_TENSOR_HEIGHT] [OUTPUT_TENSOR_WIDTH] [LAYER_SIZE] ;
// POST_FILTER_TENSOR_TYPE
typedef sc_int <OUTPUT_WIDTH > post_filter_tensor_type [POST_FILTER_HEIGHT] [POST_FILTER_WIDTH] [LAYER_SIZE] ;
// WEIGHT_MATRIX_TYPE
typedef sc_int <WEIGHT_WIDTH>  weight_matrix_type [LAYER_SIZE] [NO_WEIGHTS] ;
extern    ifstream input_file ;
extern    ifstream weight_file ;
extern    ifstream bias_file ;
extern    ofstream output_file ;
extern    ofstream output_bin ;

SC_MODULE (sim_anet_layer0)
{
    sc_in<bool >  clk;
    sc_in<bool >  start;
    sc_out<bool >  done;

    bias_vector_type bias;
    input_tensor_type input_tensor;
    mask_region_tensor_type mask_region_tensor;
    output_tensor_type output_tensor;
    post_filter_tensor_type post_filter_tensor;
    weight_matrix_type weights;

    void process_line106 ();

    sim_anet_layer0 (sc_module_name _n):sc_module (_n) {
        //SC_CTHREAD (process_line106, clk.pos());
        SC_METHOD (process_line106);
        sensitive << start  ;
    }

    ~sim_anet_layer0 () { }
    SC_HAS_PROCESS (sim_anet_layer0);
};


#endif	/*  */
