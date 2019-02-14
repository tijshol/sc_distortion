#ifndef IP_H
#define IP_H

#include <systemc.h>
#include "../sw/app.h"

#define    AXI_ADDR_WIDTH                     32	// width of the AXI address bus
// BIAS_VECTOR_TYPE
typedef sc_int <WEIGHT_WIDTH>  bias_vector_type [LAYER_SIZE] ;
// INPUT_TENSOR_TYPE
typedef sc_int <INPUT_FEATURE_WIDTH >  input_tensor_type [INPUT_FEATURE_PLANE_HEIGHT] [INPUT_FEATURE_PLANE_WIDTH] [INPUT_NO_FEATURES] ;
// MASK_REGION_TENSOR_TYPE
typedef sc_int <INPUT_FEATURE_WIDTH > mask_region_tensor_type [INPUT_MASK_HEIGHT] [INPUT_MASK_WIDTH] [INPUT_NO_FEATURES] ;
// OUTPUT_TENSOR_TYPE
typedef sc_uint <OUTPUT_WIDTH > output_tensor_type [OUTPUT_TENSOR_HEIGHT] [OUTPUT_TENSOR_WIDTH] [LAYER_SIZE] ;
// POST_FILTER_TENSOR_TYPE
typedef sc_int <OUTPUT_WIDTH > post_filter_tensor_type [POST_FILTER_HEIGHT] [POST_FILTER_WIDTH] [LAYER_SIZE] ;
// WEIGHT_MATRIX_TYPE
typedef sc_int <WEIGHT_WIDTH>  weight_matrix_type [LAYER_SIZE] [NO_WEIGHTS] ;

#ifndef __SYNTHESIS__
extern unsigned registers[16];
extern bias_vector_type bias;
extern weight_matrix_type weights;
extern input_tensor_type input_tensor;
extern output_tensor_type output_tensor;
extern mask_region_tensor_type mask_region_tensor;
extern post_filter_tensor_type post_filter_tensor;
#endif


SC_MODULE (myip)
{
    // Clock and Reset
    sc_in < bool > axi_aclk;
    sc_in < bool > axi_aresetn;

    // User Ports
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_ip_waddr;
    sc_in < sc_uint < 32 > >s_ip_wdata;
    sc_in < sc_uint < 4 > >s_ip_wstrb;
    sc_in < sc_uint < AXI_ADDR_WIDTH > >s_ip_raddr;
    sc_in < bool > s_ip_wvalid;
    sc_in < bool > s_ip_rvalid;

    sc_out < sc_uint < 32 > >s_ip_rdata;
    sc_out < bool > s_ip_wready;
    sc_out < bool > s_ip_rready;

    sc_out < bool > interrupt_request;
	unsigned registers[16];
#ifdef __SYNTHESIS__
	bias_vector_type bias;
	weight_matrix_type weights;
	input_tensor_type input_tensor;
	output_tensor_type output_tensor;
	mask_region_tensor_type mask_region_tensor;
	post_filter_tensor_type post_filter_tensor;
#endif

    sc_signal < bool > cnn_start;
    sc_signal < bool > cnn_done;

    void proc_ip();
    int gen_select_mask();
    int run_cnn();

    SC_CTOR(myip)
    {
        SC_CTHREAD (proc_ip, axi_aclk.pos ());
        reset_signal_is (axi_aresetn, false);
    }

    ~myip ()
    {
    }

};

#endif
