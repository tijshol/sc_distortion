/*******************************************************************************
*
*
********************************************************************************/

#include "app.h"
#include "mmap.h"
#include "isrsupport.h"
#include "dbg_console.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define halt()            DBGC_HALT = 1

uint32_t *BIASRAM = (uint32_t *)SYS_BIAS_RAM_BASE;
uint32_t *FILTERRAM = (uint32_t *)SYS_FILTER_RAM_BASE;
uint32_t *INPUTRAM = (uint32_t *)SYS_INPUT_RAM_BASE;
uint32_t *OUTPUTRAM = (uint32_t *)SYS_OUTPUT_RAM_BASE;

int Iflag  = 1;

char str[5] = "dick";

int main ()
{

    enableIRQ();

    print_str("Software begins\n");
    uint32_t i = 0, j = 0;

    for (i = 0; i < 16; i++){
        SYS_MEM32((SYS_AXI_BASE + (4*i) )) = i+1;
    }

    for (i = 0; i < 4; i++){
        print_str("bias i = "); print_str(str[i]); print_str("\n");
        SYS_MEM32((SYS_AXI_BASE + BIAS_OFFSET + (4*i) )) = ((uint32_t) str[i]);
    }

    // for (i = 0; i < weight_size; i++){
        // // print_str("weight i = "); print_int(i); print_str(" "); print_int(filterram[i]); print_str("\n");
        // sys_mem32((sys_axi_base + weight_offset + (4*i) )) = filterram[i];
    // }

    // for (i = 0; i < tensor_size; i++){
        // // print_str("tensor i = "); print_int(i); print_str(" "); print_int(inputram[i]); print_str("\n");
        // sys_mem32((sys_axi_base + tensor_offset + (4*i) )) = inputram[i];
    // }

// #ifdef nono
    // for (i = 0; i < output_tensor_size; i++){
        // // print_str("output_tensor i = "); print_int(i); print_str(" "); print_int(inputram[i]); print_str("\n");
        // sys_mem32((sys_axi_base + output_tensor_offset + (4*i) )) = outputram[i];
    // }

// #endif

    print_str("***********\n 1..16 + 21 !! (0x16) \n***********\n ");

    for (i = 0; i < 16; i++){
        j = SYS_MEM32((SYS_AXI_BASE + (4*i) ));
        print_str("j = "); print_int(j); print_str("\n");
    }

    for (i = 0; i < 4; i++){
        j = SYS_MEM32((SYS_AXI_BASE + BIAS_OFFSET + (4*i) ));
        // BIASRAM[i] = j;
        print_str("char  = "); print_str((char)j); print_str("\n");
    }

    // for (i = 0; i < WEIGHT_SIZE; i++){
        // j = SYS_MEM32((SYS_AXI_BASE + WEIGHT_OFFSET + (4*i) ));
        // FILTERRAM[i] = j;
        // //print_str("weight  = "); print_int(j); print_str("\n");
    // }

    // for (i = 0; i < TENSOR_SIZE; i++){
        // j = SYS_MEM32((SYS_AXI_BASE + TENSOR_OFFSET + (4*i) ));
        // INPUTRAM[i] = j;
        // //print_str("tensor  = "); print_int(j); print_str("\n");
    // }
    // SYS_MEM32((SYS_AXI_BASE ) ) = 0x80; // run_cnn
    // print_str("Start CNN \n");

    // //while ( (j = SYS_MEM32((SYS_AXI_BASE + 4))) != 0x80);

    // while (Iflag);

    // SYS_MEM32((SYS_AXI_BASE ) ) = 0x00; // stop _cnn
    // SYS_MEM32((SYS_AXI_BASE + 4) ) = 0x00; // stop _cnn
    // print_str("CNN Done\n");

    // for (i = 0; i < OUTPUT_TENSOR_SIZE; i++){
        // j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_TENSOR_OFFSET + (4*i) ));
        // OUTPUTRAM[i] = j;
        // //print_str("output_tensor  = "); print_int(j); print_str("\n");
    // }

    print_str("***********\nEVERYTHING IS DONE\n***********\n ");


    halt ();

    return(1);
}

//void handle_interrupt(void) __attribute__ ((interrupt("IRQ"))) ;

void handle_interrupt(void)
{
    disableIRQ();
    SYS_MEM32((SYS_AXI_BASE ) +0) = 0x00; // stop _cnn
    SYS_MEM32((SYS_AXI_BASE ) +4) = 0x00; // stop _cnn
    print_str("***********\nIRQ received\n***********\n ");
    Iflag  = 0;
}

