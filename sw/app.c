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
    uint32_t i = 0, j = 0,corrected_ram = 0,l = 0;

    for (i = 0; i < 16; i++){
        corrected_ram = (INPUTRAM[i]<<16)|(INPUTRAM[i]>>16);
        SYS_MEM32((SYS_AXI_BASE + INPUT_OFFSET) + i) = corrected_ram;
    }

    for (i = 0; i < 16; i++){
        j = SYS_MEM32((SYS_AXI_BASE + INPUT_OFFSET) + i);
    }

    SYS_MEM32((SYS_AXI_BASE )) = 0x80;
    print_str("Start DISTORT \n");
    
    while (Iflag);
    print_str("DISTORT Done\n");

    for (i = 0; i < 16; i++){
        j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_OFFSET) + i);
        corrected_ram = (j<<16)|(j>>16);
        OUTPUTRAM[i] = corrected_ram;
    }

    for (l=0; l<16; l++){
        print_str("output  = "); print_hex_uint(OUTPUTRAM[l]); print_str("\n");
    }

    disableIRQ();

    print_str("***********\nEVERYTHING IS DONE\n***********\n");

    halt ();

    return(1);
}

void handle_interrupt(void)
{
    disableIRQ();
    SYS_MEM32((SYS_AXI_BASE )+0 ) = 0x00;
    SYS_MEM32((SYS_AXI_BASE )+1 ) = 0x00;
    print_str("***********\nIRQ received\n***********\n");
    Iflag  = 0;
}
