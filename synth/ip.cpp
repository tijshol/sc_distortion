#include <systemc.h>

#include "../synth/ip.h"

#undef ADEBUG

unsigned registers[NUM_REG];

sc_fixed_fast <SAMPLE_SIZE,1> fixed_point_input [INPUT_SIZE];
sc_fixed_fast <SAMPLE_SIZE,1> fixed_point_output [OUTPUT_SIZE];

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

    sc_uint <SAMPLE_SIZE> dataout;
	sc_fixed <SAMPLE_SIZE * 2,SAMPLE_SIZE> temp_float;

    if (s_ip_wvalid.read() ) {

        switch (axi_waddr) {

        case 0 ... INPUT_OFFSET-1:
            registers[waddr] = axi_data + 21;
            if ( ((registers[0]) & 0x80)  == 0x80 ) {
                run_distort();
                registers[1] = 0x80;
            }
            break;

        case  INPUT_OFFSET ... OUTPUT_OFFSET-1 :
            waddr = (axi_waddr - INPUT_OFFSET) >> 2;
			exponent = (axi_data & ~(1 << (SAMPLE_SIZE-1))) >> 23; // Get exponent from float
			mantisa = axi_data << 8; // Get mantisa from float and set 1 bit in front of it
			mantisa |= (1 << (SAMPLE_SIZE-1)); // Set first bit to 1
			temp_float = mantisa >> (127 - exponent); // shift the mantisa by the exponent -127 (because that's how that shit works)
			fixed_point_input[waddr] = sc_fix(temp_float>>(SAMPLE_SIZE-1),SAMPLE_SIZE,1); // fixed point vector creation
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

        case 0 ... INPUT_OFFSET-1:
            dataout = registers[raddr] ;
            break;

        case INPUT_OFFSET ... OUTPUT_OFFSET-1 :
            raddr = (axi_raddr - INPUT_OFFSET) >> 2;
            dataout = fixed_point_input[raddr];
            break;

        case OUTPUT_OFFSET ... (OUTPUT_OFFSET + OUTPUT_SIZE*4)-1 :
            raddr = (axi_raddr - OUTPUT_OFFSET) >> 2;
            dataout = fixed_point_output[raddr];
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

	int i = 0;

	for (i = 0; i < INPUT_SIZE; i++) {
        if (fixed_point_input[i][SAMPLE_SIZE - 1] == 1)
		  fixed_point_output[i] = 1;
        else
          fixed_point_output[i] = -1;
	}
	return(1);
}
