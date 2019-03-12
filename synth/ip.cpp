#include <systemc.h>

#include "ip.h"

#undef ADEBUG

#ifndef __SYNTHESIS__

sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_input [INPUT_SIZE];
sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> fixed_point_output [OUTPUT_SIZE];
#endif

void myip::proc_ip() {

    unsigned axi_raddr = 0;
    unsigned axi_waddr = 0;
    unsigned axi_data = 0;

    unsigned int waddr;
    unsigned int prev_waddr;
    unsigned int raddr;
    unsigned int prev_raddr;

    sc_bv_base fraction(32);
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
                registers[waddr] = axi_data;
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
                // fraction = fixed_point_output[raddr].range(SAMPLE_SIZE-1,0); // Get fixed point in bit vector
                // dataout = fraction.to_uint(); // convert bit vector to uint
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
                if (((registers[0]) & 0x1) == 0x1)
                    run_clip();
                else if (((registers[0]) & 0x2) == 0x2)
                    run_overdrive();
                else if (((registers[0]) & 0x4) == 0x4)
                    run_fuzz();
                else
                    run_default();
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

// Default run function
/*  This function defaults when a run is set by arm but no effect is selected
    The function just returns the input
*/
int myip::run_default() {

    int i = 0;

#ifndef __SYNTHESIS__
    printf("Begin default \n");
#endif
    for (i = 0; i < INPUT_SIZE; i++) {
        fixed_point_output[i] = fixed_point_input[i];
	}
#ifndef __SYNTHESIS__
    printf("\nEnd default \n");
#endif
    return (1);
}

// Hard clip function (not very musical)
/*  This function clips the output audio to a threshold.
    Fixed points are used for ease of comparison
*/
int myip::run_clip() {

    int i = 0;

#ifndef __SYNTHESIS__
    printf("Begin clip \n");
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
    printf("\nEnd clip \n");
#endif
    return (1);
}

// Overdrive
/*  This function performs the overdrive effect as described by David Marshall's slides on digital audio effects
    The function has three regions of operation: 
    - The first is around zero {-1/3,1/3} and simply increase the slope of the curve
    - The seond rounds the curve using a second order polynominal {1/3,2/3}
    - The last region keeps the function at 1 which means the signal will clip {2/3,1}
*/
int myip::run_overdrive() {

    int i = 0;
    sc_fixed_fast <SAMPLE_SIZE,1> absolute;
    sc_fixed_fast <SAMPLE_SIZE,2> th = 1; // Threshold
    th = th/3;

#ifndef __SYNTHESIS__
    printf("Begin overdrive \n");
#endif
    for (i = 0; i < INPUT_SIZE; i++) {
        absolute = sc_abs(fixed_point_input[i]);
        if (absolute < th)
		    fixed_point_output[i] = 2 * fixed_point_input[i];
        else if (absolute >= th && absolute < (2*th))
            if (fixed_point_input[i] > 0)
                fixed_point_output[i] = (3 - (2 - 3 * fixed_point_input[i]) * (2 - 3 * fixed_point_input[i])) / 3;
            else
                fixed_point_output[i] = -(3 - (2 - 3 * absolute) * (2 - 3 * absolute)) / 3;
        else
            if (fixed_point_input[i] > 0)
                fixed_point_output[i] = 1;
            else
                fixed_point_output[i] = -1;
	}
#ifndef __SYNTHESIS__
    printf("\nEnd overdrive \n");
#endif
    return (1);
}

// Fuzz
/*  
*/
int myip::run_fuzz() {

    int i, j;
    sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> expo;
    sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> gain_input;
    int gain = 5;

#ifndef __SYNTHESIS__
    // printf("Begin fuzz \n");
#endif
    for (i = 0; i < INPUT_SIZE; i++) {
        gain_input = fixed_point_input[i];
        gain_input *= gain;
        if (fixed_point_input[i].is_neg()) {
            expo = expm1(gain_input,20) + 1 ;
            fixed_point_output[i] = 1 - expo;
        }
        else {
            expo = expm1(-gain_input,20) + 1;
            fixed_point_output[i] = expo - 1;
        }
	}
#ifndef __SYNTHESIS__
    // printf("\nEnd fuzz \n");
#endif
    return (1);
}

// Convert floating point variable (given as an unsigned int) to a fixed point variable
sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> myip::float2fixed(unsigned input) {

    signed int exponent;
	unsigned int mantisa;

	sc_fixed <SAMPLE_SIZE * 2,SAMPLE_SIZE,SC_TRN,SC_SAT> largeFixed;
    sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> outputFixed;

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

// Convert fixed point variable to floating point (given as an unsigned int)
unsigned myip::fixed2float(sc_fixed_fast <SAMPLE_SIZE,1,SC_TRN,SC_SAT> output) {

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
    while (((mantisa & 0x80000000) != 0x80000000) && (n <= SAMPLE_SIZE)) { // check if fisrt bit is one (the "invisible" one preceding the mantisa)
        mantisa <<= 1; // if not shift the mantissa to the left once
        n++; // this number keeps track of the number of shifts
    }
    mantisa = (mantisa >> 8) & 0x7FFFFF; // move the mantisa to the end and remove the first "invisible" one
    exponent = -n+127; // calculate exponent
    return((sign << 31) | (exponent << 23) | mantisa); // put everything together
}

// Exponential computation
/*  This function is used to calculate exp(x)-1 on hardware.
    The functions uses the taylor expantion the exp(x)-t which is equal to sum(x^k / k!,k=0,inf).
    Infinity not being practical we set the sum to a given depth using the variable "depth".
*/
sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> myip::expm1(sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> input, unsigned int depth) {
    // Initialize variables
    unsigned int i,j;
    sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> num;
    // unsigned int denom;
    sc_fixed_fast <SAMPLE_SIZE+EXP_ADD_BITS,1+EXP_ADD_BITS,SC_TRN,SC_SAT> output = 0;
    // Loop through the taylor series
    for (i = 1; i <= depth; i++) {  // depth sets the number of fractions to sum
        num = 1;                    // reset the numerator and denominator for each fraction
        // denom = 1;
        for (j=1; j <= i; j++) {
            // num *= input;      // Calculate the numerator which is the input to the power of the position of the fraction
            // denom *= j;      // Calculate the denominator which is the factorial of the position of the fraction
            num *= input/j;
        }
        output += num;        // Sum all the fractions
    }
    // j = 10;
    // num = input/j;
    // num.print();
    // printf("\n");
    return output;                  // Pass output
}