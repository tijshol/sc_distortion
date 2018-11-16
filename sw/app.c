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
uint32_t *INPUTRAM = (uint32_t *)SYS_INPUT_RAM_BASE;
uint32_t *OUTPUTRAM = (uint32_t *)SYS_OUTPUT_RAM_BASE;

int Iflag  = 1;

char str[5] = "dick";

int main ()
{

    enableIRQ();

    print_str("Software begins\n");
    uint32_t i = 0, j = 0;

    // for (i = 0; i < 16; i++){
        // SYS_MEM32((SYS_AXI_BASE + (4*i) )) = i+1;
    // }

    // for (i = 0; i < 4; i++){
    //     print_str("bias i = "); print_char(str[i]); print_str("\n");
    //     SYS_MEM32((SYS_AXI_BASE + BIAS_OFFSET + (4*i) )) = ((int) str[i]);
    // }

    for (i = 0; i < 16; i++){
        print_str("input = "); print_int(INPUTRAM[i]); print_str("\n");
        SYS_MEM32((SYS_AXI_BASE + BIAS_OFFSET + (4*i) )) = INPUTRAM[i];
    }
	
    // for (i = 0; i < 16; i++){
        // j = SYS_MEM32((SYS_AXI_BASE + (4*i) ));
        // print_str("j = "); print_int(j); print_str("\n");
    // }

	SYS_MEM32((SYS_AXI_BASE ) ) = 0x80; // run_cnn
    print_str("Start DISTORT \n");

    while (Iflag);

    SYS_MEM32((SYS_AXI_BASE ) ) = 0x00; // stop _cnn
    SYS_MEM32((SYS_AXI_BASE + 4) ) = 0x00; // stop _cnn
    print_str("DISTORT Done\n");
	
	for (i = 0; i < 4; i++){
        j = SYS_MEM32((SYS_AXI_BASE + BIAS_OFFSET + (4*i) ));
        // BIASRAM[i] = j;
        print_str("char = "); print_char((char)j); print_str("\n");
    }

     for (i = 0; i < 16; i++){
        j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_TENSOR_OFFSET + (4*i) ));
        OUTPUTRAM[i] = j;
        print_str("output_tensor  = "); print_int(j); print_str("\n");
    }


    print_str("***********\nEVERYTHING IS DONE\n***********\n ");


    halt ();

    return(1);
}

void handle_interrupt(void)
{
    disableIRQ();
    SYS_MEM32((SYS_AXI_BASE ) +0) = 0x00; // stop _cnn
    SYS_MEM32((SYS_AXI_BASE ) +4) = 0x00; // stop _cnn
    print_str("***********\nIRQ received\n***********\n ");
    Iflag  = 0;
}

