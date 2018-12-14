#include <systemc.h>

#include "../synth/ip.h"

#undef ADEBUG

unsigned registers[16];

typedef sc_int <WEIGHT_WIDTH>  bias_vector_type [LAYER_SIZE] ;
typedef sc_uint <OUTPUT_WIDTH > output_tensor_type [LAYER_SIZE] ;

bias_vector_type bias;
// weight_matrix_type weights;
// input_tensor_type input_tensor;
output_tensor_type output_tensor;
// mask_region_tensor_type mask_region_tensor;
// post_filter_tensor_type post_filter_tensor;

int k5  = 0;

int weight_pos  = 0;
int r_weight_pos  = 0;

sc_fixed_fast <32,1> fixed_point_vector [LAYER_SIZE];

int rdone = 0;

void myip::run() {
    unsigned axi_raddr;
    unsigned axi_waddr;
    unsigned axi_data;
    unsigned axi_strb;

    axi_waddr = s_ip_waddr.read();
    axi_data = s_ip_wdata.read();
    axi_strb = s_ip_wstrb.read();
    axi_raddr =s_ip_raddr.read();

    unsigned int waddr =  axi_waddr >> 2;
    unsigned int raddr =  axi_raddr >> 2;

	signed exponent;
	unsigned mantisa;

    sc_uint <32> dataout;
	sc_fixed <64,32> temp_float;

    if (s_ip_wvalid.read() ) {

        switch (axi_waddr) {

        case 0 ... BIAS_OFFSET-1:
            registers[waddr] = axi_data + 21;
            if ( ((registers[0]) & 0x80)  == 0x80 ) {
                run_distort();
                registers[1] = 0x80;
            }
            break;

        case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
            waddr = (axi_waddr - BIAS_OFFSET) >> 2;
			exponent = (axi_data & ~(1 << 31)) >> 23; // Get exponent from float
			mantisa = axi_data << 8; // Get mantisa from float and set 1 bit in front of it
			mantisa |= (1 << 31); // Set first bit to 1
			temp_float = mantisa >> (127 - exponent); // shift the mantisa by the exponent -127 (because that's how that shit works)
			fixed_point_vector[waddr] = sc_fix(temp_float>>31,32,1); // fixed point vector creation
            break;
        default:
            break;
        }
        s_ip_wready.write(true);
    }
    else
        s_ip_wready.write(false);

    if (s_ip_rvalid.read() && rdone == 0) {
        switch (axi_raddr) {

        case 0 ... BIAS_OFFSET-1:
            dataout = registers[raddr] ;
            break;

        case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
            raddr = (axi_raddr - BIAS_OFFSET) >> 2;
			if (registers[1] == 0x80)
				dataout = (sc_int <32>) bias[raddr];
			else
			{
				dataout = 0;
				temp_float = fixed_point_vector[raddr]*100;
				dataout = fixed_point_vector[raddr]*100;
			}
            break;

        case OUTPUT_TENSOR_OFFSET ... (OUTPUT_TENSOR_OFFSET+OUTPUT_TENSOR_SIZE*4) -1 :
            raddr = (axi_raddr - OUTPUT_TENSOR_OFFSET) >> 2;

            dataout = output_tensor[k5];

            k5 = k5 + 1;
            if (k5 == LAYER_SIZE)
                k5 = 0;
            break;

        default:
            break;
        }

        s_ip_rdata.write(dataout);
        s_ip_rready.write(true);
        rdone++;
    }
    else {
        s_ip_rready.write(false);
        rdone = 0;
    }

    interrupt.write(((registers[1]) & 0x80)  == 0x80 ) ;
}

int myip::run_distort() {

	int wd = 0;

	for (wd = 0; wd < LAYER_SIZE; wd++) {
        if (fixed_point_vector[wd][32 - 1] == 1)
		  output_tensor[wd] = 0x80000000;
        else
          output_tensor[wd] = 0x7FFFFFFF;
	}
	return(1);
}

// int myip::gen_select_mask() {
//     int mask = 0;
//     int select = s_ip_wstrb.read();
//     for (int i = 0; i < 4; ++i) {
//         mask = mask << 8;
//         if (select & 8) {
//             mask |= 0xFF;
//         }
//         select = select << 1;
//     }
//     return(mask);
// }
