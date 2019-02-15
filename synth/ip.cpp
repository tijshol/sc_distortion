#include <systemc.h>

#include "ip.h"

#undef ADEBUG

#ifndef __SYNTHESIS__

sc_fixed_fast <SAMPLE_SIZE,1> fixed_point_input [INPUT_SIZE];
sc_fixed_fast <SAMPLE_SIZE,1> fixed_point_output [OUTPUT_SIZE];
#endif

void myip::proc_ip() {

    unsigned axi_raddr = 0;
    unsigned axi_waddr = 0;
    unsigned axi_data = 0;

    unsigned int waddr;
    unsigned int prev_waddr;
    unsigned int raddr;
    unsigned int prev_raddr;

    sc_uint <32> dataout;

    interrupt_request.write(0);

    s_ip_rdata.write(0);
    s_ip_rready.write(false);
    s_ip_wready.write(false);

    prev_raddr = 0xffffffff;
    prev_waddr = 0xffffffff;

    waddr = 0x0;
    raddr = 0x0;

    registers[0]=0;

    wait();

    registers[1]=0;

    wait();

    while (true) {

        if (s_ip_wvalid.read() == true) {

            axi_waddr = s_ip_waddr.read();
            if (s_ip_wstrb.read()==0xF)
                axi_data  = s_ip_wdata.read();

            switch (axi_waddr) {

            case 0 ... INPUT_OFFSET-1:
                waddr =  axi_waddr >> 2;
                registers[waddr] = axi_data + 21;
                break;

            case  INPUT_OFFSET ... OUTPUT_OFFSET-1 :
                waddr = (axi_waddr - INPUT_OFFSET) >> 2;
                fixed_point_input[waddr] = float2fixed(axi_data);
                break;
                
            default:
                break;
            }

            s_ip_wready.write(true);
        }
        else if (s_ip_rvalid.read() == true) {
            axi_raddr = s_ip_raddr.read();

            switch (axi_raddr) {

            case 0 ... INPUT_OFFSET-1:
                dataout = registers[raddr] ;
                break;

            case INPUT_OFFSET ... OUTPUT_OFFSET-1 :
                raddr = (axi_raddr - INPUT_OFFSET) >> 2;
                dataout = fixed2float(fixed_point_input[raddr]);
                break;

            case OUTPUT_OFFSET ... (OUTPUT_OFFSET + OUTPUT_SIZE*4)-1 :
                raddr = (axi_raddr - OUTPUT_OFFSET) >> 2;
                dataout = fixed2float(fixed_point_output[raddr]);
                break;
            default:
                break;
            }
            s_ip_rready.write(true);
            s_ip_rdata.write(dataout);
        }
        else {
            s_ip_rready.write(false);
            s_ip_wready.write(false);
            s_ip_rdata.write(0x0);
            if ( (((registers[0]) & 0x80)  == 0x80) && (((registers[1]) & 0x80)  == 0x00) ) {
                wait();
                run_distort();
                wait();
                registers[0] &= ~0x80;
                wait();
                registers[1] |= 0x80;
            } else if (((registers[1]) & 0x80)  == 0x80) {
                interrupt_request.write(true);
            }
            else
                interrupt_request.write(false);
        }

        wait();
    }
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

int myip::run_distort() {

    int i = 0;

#ifndef __SYNTHESIS__
    printf("Begin Distrot \n");
#endif
    for (i = 0; i < INPUT_SIZE; i++) {
        if (fixed_point_input[i]>0.5)
		    fixed_point_output[i] = 0.5;
        else if (fixed_point_input[i]<-0.5)
           fixed_point_output[i] = -0.5;
        else
            fixed_point_output[i] = fixed_point_input[i];
	}
#ifndef __SYNTHESIS__
    printf("\nEnd CNN \n");
#endif
    return (1);
}

sc_fixed_fast <SAMPLE_SIZE,1> myip::float2fixed(unsigned input) {

    signed int exponent;
	unsigned int mantisa;

	sc_fixed <SAMPLE_SIZE * 2,SAMPLE_SIZE> largeFixed;
    sc_fixed_fast <SAMPLE_SIZE,1> outputFixed;

    exponent = (input & ~(1 << (SAMPLE_SIZE-1))) >> 23; // Get exponent from float
    mantisa = input << 8; // Get mantisa from float and set 1 bit in front of it
    mantisa |= (1 << (SAMPLE_SIZE-1)); // Set first bit to 1
    largeFixed = mantisa >> (127 - exponent); // shift the mantisa by the exponent -127 (because that's how that shit works)
    if ((input>>31) == 1) {
        outputFixed = sc_fix(-largeFixed>>(SAMPLE_SIZE-1),SAMPLE_SIZE,1); // fixed point vector creation
    }
    else {
        outputFixed = sc_fix(largeFixed>>(SAMPLE_SIZE-1),SAMPLE_SIZE,1); // fixed point vector creation
    }
    return(outputFixed);
}

unsigned myip::fixed2float(sc_fixed_fast <SAMPLE_SIZE,1> output) {

    signed int exponent;
	unsigned int mantisa;
    bool sign = false;

    sc_bv_base fraction(32);
    int n = 0;

    fraction = output.range(SAMPLE_SIZE-1,0); // Get fixed point in bit vector
    mantisa = fraction.to_uint(); // convert bit vector to uint
    if (output.is_neg()){ // if the fixed point is negative
        mantisa = ~mantisa + 1; // correct for tow's complement
        sign = true; // and set sign bit
    }
    while ((mantisa & 0x80000000) != 0x80000000) { // check if fisrt bit is one (the "invisible" one preceding the mantisa)
        mantisa <<= 1; // if not shift the mantissa to the left once
        n++; // this number keeps track of the number of shifts
    }
    mantisa = (mantisa >> 8) & 0x7FFFFF; // move the mantisa to the end and remove the first "invisible" one
    exponent = -n+127; // calculate exponent
    return((sign << 31) | (exponent << 23) | mantisa); // put everything together
}