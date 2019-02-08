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
    uint32_t i = 0, ii = 0, j = 0;

    while(i < SYS_INPUT_RAM_SIZE/1000) {
        
        print_str("INPUTRAM["); print_int(i+ii); print_str("] = "); print_hex_uint(INPUTRAM[i+ii]); print_str("\n");
        for(ii = 0; ii < INPUT_SIZE; ii++) { // transfer 1 chunk of INPUT_SIZE
            SYS_MEM32((SYS_AXI_BASE + INPUT_OFFSET + (4*ii))) = INPUTRAM[i+ii];
        }

        SYS_MEM32((SYS_AXI_BASE ) ) = 0x80;
        print_str("Start DISTORT \n");

        while (Iflag);

        // SYS_MEM32((SYS_AXI_BASE ) ) = 0x00;
        // SYS_MEM32((SYS_AXI_BASE + 4) ) = 0x00;
        print_str("DISTORT Done\n");

        print_str("OUTPUTRAM["); print_int(i+ii); print_str("] = "); print_hex_uint(OUTPUTRAM[i+ii]); print_str("\n");
        for(ii = 0; ii < INPUT_SIZE; ii++) {
            j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_OFFSET + (4*ii)));
            OUTPUTRAM[i+ii] = j;
        }

        Iflag  = 1;

        i+=INPUT_SIZE;
    }

    disableIRQ();

    print_str("***********\nEVERYTHING IS DONE\n***********\n");

    halt ();

    return(1);
}

void handle_interrupt(void)
{
    disableIRQ();
    SYS_MEM32((SYS_AXI_BASE ) ) = 0x00;
    SYS_MEM32((SYS_AXI_BASE + 4) ) = 0x00;
    print_str("***********\nIRQ received\n***********\n");
    Iflag  = 0;
    enableIRQ();
}
