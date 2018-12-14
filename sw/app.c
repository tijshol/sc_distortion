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

uint32_t *INPUTRAM = (uint32_t *)SYS_INPUT_RAM_BASE;
uint32_t *OUTPUTRAM = (uint32_t *)SYS_OUTPUT_RAM_BASE;

int Iflag  = 1;

int main ()
{

    enableIRQ();

    print_str("Software begins\n");
    uint32_t i = 0, j = 0;

    for (i = 0; i < 16; i++){
        print_str("input = "); print_int(INPUTRAM[i]); print_str("\n");
        SYS_MEM32((SYS_AXI_BASE + INPUT_OFFSET)) = INPUTRAM[i];

        SYS_MEM32((SYS_AXI_BASE ) ) = 0x80;
        print_str("Start DISTORT \n");

        while (Iflag);

        SYS_MEM32((SYS_AXI_BASE ) ) = 0x00;
        SYS_MEM32((SYS_AXI_BASE + 4) ) = 0x00;
        print_str("DISTORT Done\n");

        print_hex_uint((SYS_AXI_BASE + OUTPUT_OFFSET - SYS_AXI_BASE - OUTPUT_OFFSET)>>2);
        print_str("\n");
        j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_OFFSET));
        print_str("output  = "); print_int(j); print_str("\n");
        OUTPUTRAM[i] = j;
    }

    print_str("***********\nEVERYTHING IS DONE\n***********\n");

    halt ();

    return(1);
}

void handle_interrupt(void)
{
    disableIRQ();
    SYS_MEM32((SYS_AXI_BASE ) +0) = 0x00; // stop _cnn
    SYS_MEM32((SYS_AXI_BASE ) +4) = 0x00; // stop _cnn
    print_str("***********\nIRQ received\n***********\n");
    Iflag  = 0;
}
