#include <systemc.h>

#include "../synth/ip.h"

#undef ADEBUG

unsigned registers[16];

// BIAS_VECTOR_TYPE
typedef sc_int <32>  bias_vector_type [LAYER_SIZE] ;

bias_vector_type bias;

//sc_fxtype_params fxt(32,1);
//sc_fxtype_context fcxt(fxt);

//sc_fix fixed_point_vector [LAYER_SIZE];

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
	
    int mask;

    sc_uint <32> dataout;

    if (s_ip_wvalid.read() ) {

        switch (axi_waddr) {

		case 0 ... BIAS_OFFSET-1:
            mask = gen_select_mask();
            //registers[waddr] = (registers[waddr] & (~mask)) | (axi_data & mask);
            registers[waddr] = axi_data + 21;
            if ( ((registers[0]) & 0x80)  == 0x80 ) {
                registers[1] = 0x80;
            }
            break;
		
        case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
            waddr = (axi_waddr - BIAS_OFFSET) >> 2;
			exponent = (axi_data & ~(1 << 31)) >> 23; // Get exponent from float
			mantisa = axi_data << 8; // Get mantisa from float and set 1 bit in front of it
			mantisa |= (1 << 31); // Set first bit to 1
			fixed_point_vector[waddr] = mantisa >> (127 - exponent); // shift the mantisa by the exponent -127 (because that's how that shit works)
            bias[waddr] = ( sc_int <32> ) (mantisa >> (127 - exponent));

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
            mask = gen_select_mask();
            //dataout = registers[raddr] & mask;
            dataout = registers[raddr] ;
            break;

        case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
            raddr = (axi_raddr - BIAS_OFFSET) >> 2;
			if (registers[1] == 0x80)
				dataout = (sc_int <32>) bias[raddr];
			else
				dataout = (sc_int <32>) 0;
				dataout |= fixed_point_vector[raddr];
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

int myip::gen_select_mask() {
    int mask = 0;
    int select = s_ip_wstrb.read();
    for (int i = 0; i < 4; ++i) {
        mask = mask << 8;
        if (select & 8) {
            mask |= 0xFF;
        }
        select = select << 1;
    }
    return(mask);
}
