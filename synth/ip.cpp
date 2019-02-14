#include <systemc.h>

#include "ip.h"

#undef ADEBUG

#define DIV2(d,n)  (d&0x80000000)!=0?((d>>n)+1):(d>>n);

//// BIAS_VECTOR_TYPE
//typedef sc_int <WEIGHT_WIDTH>  bias_vector_type [LAYER_SIZE] ;
//// INPUT_TENSOR_TYPE
//typedef sc_int <INPUT_FEATURE_WIDTH >  input_tensor_type [INPUT_FEATURE_PLANE_HEIGHT] [INPUT_FEATURE_PLANE_WIDTH] [INPUT_NO_FEATURES] ;
//// MASK_REGION_TENSOR_TYPE
//typedef sc_int <INPUT_FEATURE_WIDTH > mask_region_tensor_type [INPUT_MASK_HEIGHT] [INPUT_MASK_WIDTH] [INPUT_NO_FEATURES] ;
//// OUTPUT_TENSOR_TYPE
//typedef sc_uint <OUTPUT_WIDTH > output_tensor_type [OUTPUT_TENSOR_HEIGHT] [OUTPUT_TENSOR_WIDTH] [LAYER_SIZE] ;
//// POST_FILTER_TENSOR_TYPE
//typedef sc_int <OUTPUT_WIDTH > post_filter_tensor_type [POST_FILTER_HEIGHT] [POST_FILTER_WIDTH] [LAYER_SIZE] ;
//// WEIGHT_MATRIX_TYPE
//typedef sc_int <WEIGHT_WIDTH>  weight_matrix_type [LAYER_SIZE] [NO_WEIGHTS] ;
//
extern FILE *fp_mask;
#ifndef __SYNTHESIS__
bias_vector_type bias;
weight_matrix_type weights;
input_tensor_type input_tensor;
output_tensor_type output_tensor;
mask_region_tensor_type mask_region_tensor;
post_filter_tensor_type post_filter_tensor;
#endif

void myip::proc_ip() {
    int n1  = 0;
    int j11  = 0;
    int k1  = 0;
    int i1  = 0;

    int j2  = 0;
    int k2  = 0;
    int i2  = 0;

    int j3  = 0;
    int k3  = 0;
    int i3  = 0;

    int j4  = 0;
    int k4  = 0;
    int i4  = 0;

    int j5  = 0;
    int k5  = 0;
    int i5  = 0;

    int n6  = 0;
    int j6  = 0;
    int k6  = 0;
    int i6  = 0;

    int weight_pos  = 0;
    int r_weight_pos  = 0;

    unsigned axi_raddr = 0;
    unsigned axi_waddr = 0;
    unsigned axi_data = 0;

    unsigned int waddr;
    unsigned int prev_waddr;
    unsigned int raddr;
    unsigned int prev_raddr;

    int int_file = 0;

    sc_uint <32> dataout;
    sc_uint <INPUT_FEATURE_WIDTH > tensor_u;
    sc_uint <OUTPUT_WIDTH >  out_tensor_u;



    interrupt_request.write(0);

    s_ip_rdata.write(0);
    s_ip_rready.write(false);
    s_ip_wready.write(false);

    prev_raddr = 0xffffffff;
    prev_waddr = 0xffffffff;

    waddr = 0x0;
    raddr = 0x0;

    registers[0]=0;
    bias[0]=0;
    weights[0][0]=0;
    input_tensor[0][0][0]=0;
    output_tensor[0][0][0]=0;

    wait();

    registers[1]=0;

    wait();

    while (true) {

        if (s_ip_wvalid.read() == true) {

            axi_waddr = s_ip_waddr.read();
            if (s_ip_wstrb.read()==0xF)
                axi_data  = s_ip_wdata.read();

            switch (axi_waddr) {

            case 0 ... BIAS_OFFSET-1:
                waddr =  axi_waddr >> 2;
                registers[waddr] = axi_data + 21;

                break;

            case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
                waddr = (axi_waddr - BIAS_OFFSET) >> 2;
                //int_file = ( (axi_data >> 26));
                int_file = DIV2(axi_data , 26);
//printf("int_file 1: %d %x\n", int_file,int_file);
                int_file = (axi_data / BIAS_SCALING);
//printf("int_file 2: %d %x\n", int_file,int_file);

                bias[waddr] = ( sc_int < WEIGHT_WIDTH> ) int_file;

                break;


                //case WEIGHT_OFFSET ... (WEIGHT_OFFSET+WEIGHT_SIZE*4)-1 :
            case WEIGHT_OFFSET ... TENSOR_OFFSET-1 :
                waddr = (axi_waddr - WEIGHT_OFFSET) >> 2;
                if (waddr != prev_waddr) {
                    //int_file = ((axi_data >> 23));
                    int_file = DIV2(axi_data , 23);
//printf("int_file 3: %d %x\n", int_file,int_file);
                    int_file = (axi_data / WEIGHT_SCALING);
//printf("int_file 4: %d %x\n", int_file,int_file);

                    if (i1 == 0)
                    {
                        weights[n1][(weight_pos + 2)] = (sc_int<WEIGHT_WIDTH>)int_file;
                        //printf("wweights:axi_waddr=%#x\taxi_data=%d\twaddr=%d,\tint_file=%d,\tweight_pos=%d,\tweights[%d][%d]=%d\n", (int)axi_waddr, (int)axi_data, waddr, int_file, weight_pos, (int)n1, (int)(weight_pos+2), (int)weights[n1][weight_pos+2]);
                    }
                    else if (i1 == 1)
                    {
                        weights[n1][weight_pos] = (sc_int<WEIGHT_WIDTH>)int_file;
                        //printf("wweights:axi_waddr=%#x\taxi_data=%d\twaddr=%d,\tint_file=%d,\tweight_pos=%d,\tweights[%d][%d]=%d\n", (int)axi_waddr, (int)axi_data, waddr, int_file, weight_pos, (int)n1, (int)weight_pos, (int)weights[n1][weight_pos]);
                    }
                    else
                    {
                        weights[n1][(weight_pos - 2)] = (sc_int<WEIGHT_WIDTH>)int_file;
                        //printf("wweights:axi_waddr=%#x\taxi_data=%d\twaddr=%d,\tint_file=%d,\tweight_pos=%d,\tweights[%d][%d]=%d\n", (int)axi_waddr, (int)axi_data, waddr, int_file, weight_pos, (int)n1, (int)(weight_pos-2), (int)weights[n1][weight_pos-2]);
                    }

                    weight_pos = (weight_pos + 1);

                    i1 = i1 + 1;
                    if (i1 == INPUT_NO_FEATURES)
                    {
                        i1 = 0;
                        k1 = (k1 + 1);
                    }
                    if (k1 ==  INPUT_MASK_WIDTH)
                    {
                        k1 = 0;
                        j11 = (j11 + 1);
                    }
                    if (j11 == INPUT_MASK_HEIGHT)
                    {
                        j11 = 0;
                        n1 = (n1 + 1);
                        weight_pos = 0;
                    }
                    prev_waddr = waddr;
                }
                break;

            case TENSOR_OFFSET ... (TENSOR_OFFSET+TENSOR_SIZE*4)-1 :
                waddr = (axi_waddr - TENSOR_OFFSET) >> 2;
                if (waddr != prev_waddr) {
                    int_file = ((int)axi_data - INPUT_BIAS);
                    input_tensor[i2][j2][k2] = (sc_int <INPUT_FEATURE_WIDTH > ) int_file;
                    //printf("witensor:axi_waddr=%#x\taxi_data=%d\twaddr=%d\tint_file=%d\tinput_tensor[%d][%d][%d]=%d\n", (int)axi_waddr, (int)axi_data, (int)waddr, (int)int_file, (int)i2, (int)j2, (int)k2, (int)input_tensor[i2][j2][k2]);
                    k2 = k2 + 1;
                    if (k2 == INPUT_NO_FEATURES)
                    {
                        k2 = 0;
                        j2 = (j2 + 1);
                    }
                    if (j2 ==  INPUT_FEATURE_PLANE_WIDTH)
                    {
                        j2 = 0;
                        i2 = (i2 + 1);
                    }
                    if (i2 == INPUT_FEATURE_PLANE_HEIGHT)
                    {
                        i2 = 0;
                    }
                    prev_waddr = waddr;
                }
                break;

            case OUTPUT_TENSOR_OFFSET ... (OUTPUT_TENSOR_OFFSET+OUTPUT_TENSOR_SIZE*4) -1 :
                waddr = (axi_waddr - OUTPUT_TENSOR_OFFSET) >> 2;
                if (waddr != prev_waddr) {
                    output_tensor[i3][j3][k3] = axi_data;
                    k3 = k3 + 1;
                    if (k3 == LAYER_SIZE)
                    {
                        k3 = 0;
                        j3 = (j3 + 1);
                    }
                    if (j3 == OUTPUT_TENSOR_WIDTH)
                    {
                        j3 = 0;
                        i3 = (i3 + 1);
                    }
                    if (i3 == OUTPUT_TENSOR_HEIGHT)
                    {
                        i3 = 0;
                    }
                    prev_waddr = waddr;
                }
                break;

            default:
                break;
            }

            s_ip_wready.write(true);
        }
        else if (s_ip_rvalid.read() == true) {
            axi_raddr = s_ip_raddr.read();

            switch (axi_raddr) {

            case 0 ... BIAS_OFFSET-1:
                raddr =  axi_raddr >> 2;
                dataout = registers[raddr];

                break;

            case  BIAS_OFFSET ... WEIGHT_OFFSET-1 :
                raddr = (axi_raddr - BIAS_OFFSET) >> 2;
                dataout = (sc_int <32>) bias[raddr]; // * BIAS_SCALING;
                break;

            case WEIGHT_OFFSET ... TENSOR_OFFSET-1 :
                raddr = (axi_raddr - WEIGHT_OFFSET) >> 2;
                if ( raddr != prev_raddr) {
                    if (i6 == 0)
                    {
                        dataout = (sc_int <32 >)weights[n6][((r_weight_pos) + 2)]; // * WEIGHT_SCALING;

                        //printf("rweights:axi_raddr=%#x, raddr=%d, dataout=%d, r_weight_pos=%d, weights[%d][%d]=%d\n", (int)axi_raddr, (int)raddr, (int)dataout, (int)r_weight_pos, (int)n6, (int)(r_weight_pos+2),  (int)weights[n6][r_weight_pos+2]);
                    }
                    else if (i6 == 1)
                    {
                        dataout = (sc_int <32 >)weights[n6][(r_weight_pos)]; // * WEIGHT_SCALING;
                        //printf("rweights:axi_raddr=%#x, raddr=%d, dataout=%d, r_weight_pos=%d, weights[%d][%d]=%d\n", (int)axi_raddr, (int)raddr, (int)dataout, (int)r_weight_pos, (int)n6, (int)(r_weight_pos),  (int)weights[n6][r_weight_pos]);
                    }
                    else
                    {
                        dataout = (sc_int <32 >)weights[n6][((r_weight_pos) - 2)]; // * WEIGHT_SCALING;
                        //printf("rweights:axi_raddr=%#x, raddr=%d, dataout=%d, r_weight_pos=%d, weights[%d][%d]=%d\n", (int)axi_raddr, (int)raddr, (int)dataout, (int)r_weight_pos, (int)n6, (int)(r_weight_pos-2),  (int)weights[n6][r_weight_pos-2]);
                    }

                    r_weight_pos = (r_weight_pos + 1);
                    i6 = i6 + 1;

                    if (i6 == INPUT_NO_FEATURES)
                    {
                        i6 = 0;
                        k6 = (k6 + 1);
                    }
                    if (k6 ==  INPUT_MASK_WIDTH)
                    {
                        k6 = 0;
                        j6 = (j6 + 1);
                    }
                    if (j6 == INPUT_MASK_HEIGHT)
                    {
                        j6 = 0;
                        n6 = (n6 + 1);
                        r_weight_pos = 0;
                    }
                    prev_raddr = raddr;
                }
                break;

            case TENSOR_OFFSET ... (TENSOR_OFFSET+TENSOR_SIZE*4) -1 :
                raddr = (axi_raddr - TENSOR_OFFSET) >> 2;

                if (raddr != prev_raddr) {
                    dataout = (sc_uint <INPUT_FEATURE_WIDTH >) input_tensor[i4][j4][k4];
                    //printf("ritensor:axi_raddr=%#x,raddr=%d\tdataout=%d\tinput_tensor[%d][%d][%d]=%d\n",(int)axi_raddr, (int)raddr, (int)dataout, (int)i4, (int)j4, (int)k4, (int)input_tensor[i4][j4][k4]);

                    k4 = k4 + 1;
                    if (k4 == INPUT_NO_FEATURES)
                    {
                        k4 = 0;
                        j4 = (j4 + 1);
                    }
                    if (j4 ==  INPUT_FEATURE_PLANE_WIDTH)
                    {
                        j4 = 0;
                        i4 = (i4 + 1);
                    }
                    if (i4 == INPUT_FEATURE_PLANE_HEIGHT)
                    {
                        i4 = 0;
                    }
                    prev_raddr = raddr;
                }
                break;

            case OUTPUT_TENSOR_OFFSET ... (OUTPUT_TENSOR_OFFSET+OUTPUT_TENSOR_SIZE*4) -1 :
                raddr = (axi_raddr - OUTPUT_TENSOR_OFFSET) >> 2;
                if (raddr != prev_raddr) {

                    dataout = output_tensor[i5][j5][k5];
                    //wait(3);

                    k5 = k5 + 1;
                    if (k5 == LAYER_SIZE)
                    {
                        k5 = 0;
                        j5 = (j5 + 1);
                    }
                    if (j5 == OUTPUT_TENSOR_WIDTH)
                    {
                        j5 = 0;
                        i5 = (i5 + 1);
                    }
                    if (i5 == OUTPUT_TENSOR_HEIGHT)
                    {
                        i5 = 0;
                    }
                    prev_raddr = raddr;
                }
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
                run_cnn();
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


//		printf(" n1=%d j11=%d k1=%d i1=%d j2=%d k2=%d i2=%d j3=%d k3=%d i3=%d || j4=%d k4=%d i4=%d j5=%d k5=%d i5=%d n6=%d j6=%d k6=%d i6=%d\n",
//		n1, j11, k1, i1, j2, k2, i2, j3, k3, i3, j4, k4, i4, j5, k5, i5, n6, j6, k6, i6);

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

int myip::run_cnn() {


    int pfh = 0;
    int pfw = 0;
    int imh = 0;
    int imw = 0;
    int inf = 0;
    int ls = 0;

    int start_row =0;
    int start_col =0;
    int wp = 0;

    sc_int < INPUT_FEATURE_WIDTH > signed_feature;
    sc_int < (INPUT_FEATURE_WIDTH + WEIGHT_WIDTH) > neuron_product;
    sc_int < (NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH + 1 > neuron_acc;
    sc_uint < OUTPUT_WIDTH > neuron_out;

    //
    // Scan across input image
    //

#ifndef __SYNTHESIS__
    printf("Begin CNN \n");
    printf("pfh =\n");
#endif
    for (pfh = 0; pfh < POST_FILTER_HEIGHT; pfh++) {

#ifndef __SYNTHESIS__
        if (pfh%20==0) {
            printf(" %d\n", pfh);
        }
        else
            printf(" %d", pfh);
#endif

run_cnn_label0:
        for (pfw = 0; pfw < POST_FILTER_WIDTH; pfw++) {
            start_row = ((INPUT_STRIDE * (pfh)) - 1 );
            start_col = ((INPUT_STRIDE * (pfw)) - 1 );
run_cnn_label1:
            for (imh = 0; imh < INPUT_MASK_HEIGHT; imh++) {
                for (imw = 0; imw < INPUT_MASK_WIDTH; imw++) {
                    for (inf = 0; inf < INPUT_NO_FEATURES; inf++) {
                        if (((((start_row + imh) >= 0) && ((start_row + imh) < INPUT_FEATURE_PLANE_HEIGHT))
                                && ((start_col + imw) >= 0)) && ((start_col + imw) < INPUT_FEATURE_PLANE_WIDTH)) {
                            mask_region_tensor[imh][imw][inf] = input_tensor[(start_row + imh)][(start_col + imw)][inf];
                        }
                        else {
                            mask_region_tensor[imh][imw][inf] = 0;
                        }
#ifndef __SYNTHESIS__
//		if(pfh==0 &&pfw==0)
//		fprintf(fp_mask, "%0x\n", (unsigned)(mask_region_tensor[imh][imw][inf] & 0xff));
                        //fprintf(fp_mask, "mask_region_tensor[%d][%d][%d] = %0x\n", imh, imw, inf, (unsigned)(mask_region_tensor[imh][imw][inf] & 0xff));
#endif

                    }
                }
            }

            for (ls = 0; ls < LAYER_SIZE; ls++) {
                neuron_acc = (bias[ls]) <<BIAS_SHIFT;
                wp = 0;
                for (imh = 0; imh < INPUT_MASK_HEIGHT; imh++) {
                    for (imw = 0; imw < INPUT_MASK_WIDTH; imw++) {
                        for (inf = 0; inf < INPUT_NO_FEATURES; inf++) {
                            signed_feature = mask_region_tensor[imh][imw][inf];
                            neuron_product = (signed_feature * weights[ls][wp]);
                            wp = (wp + 1);
                            neuron_acc = neuron_acc + neuron_product;
                        }
                    }
                }

                if (neuron_acc [((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH)] == 1) {
                    neuron_out = 0;
                }
                else {
                    if ((OUTPUT_SHIFT + OUTPUT_WIDTH) < (((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH) - 1)) {
                        if (neuron_acc ((((NO_WEIGHT_BITS + INPUT_FEATURE_WIDTH) + WEIGHT_WIDTH) - 1), (OUTPUT_SHIFT + OUTPUT_WIDTH)) != 0) {
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
                output_tensor[pfh][pfw][ls] = neuron_out;
            }
        }

    }
#ifndef __SYNTHESIS__
    printf("\nEnd CNN \n");
#endif
    return (1);
}
