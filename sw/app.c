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

#define halt()          DBGC_HALT = 1

#define SIM_SIZE        48000

uint32_t *INPUTRAM = (uint32_t *)SYS_INPUT_RAM_BASE;
uint32_t *OUTPUTRAM = (uint32_t *)SYS_OUTPUT_RAM_BASE;

int Iflag  = 1;

int main ()
{

    enableIRQ();

    print_str("Software begins\n");
    uint32_t i = 0, j = 0, k = 0, corrected_ram = 0;

    for (k = 0; k < SIM_SIZE; k++) {
        for (i = 0; i < INPUT_SIZE; i++){
            corrected_ram = (INPUTRAM[k]<<16)|(INPUTRAM[k]>>16);
            SYS_MEM32((SYS_AXI_BASE + INPUT_OFFSET) + i) = corrected_ram;
        }

        SYS_MEM32((SYS_AXI_BASE )) = 0x84;
        while (Iflag);

        Iflag = 1;
        enableIRQ();

        for (i = 0; i < INPUT_SIZE; i++){
            j = SYS_MEM32((SYS_AXI_BASE + OUTPUT_OFFSET) + i);
            corrected_ram = (j<<16)|(j>>16);
            // print_str("returned: "); print_hex_uint(corrected_ram); print_str("\n");
            OUTPUTRAM[k] = corrected_ram;
            // print_str("written: "); print_hex_uint(OUTPUTRAM[k]); print_str("\n");
        }
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
    // print_str("***********\nIRQ received\n***********\n");
    Iflag  = 0;
}
