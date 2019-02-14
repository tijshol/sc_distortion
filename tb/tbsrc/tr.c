#include <stdio.h>

#include "../../sw/mmap.h"

extern FILE *tbsrc;

void
write_transaction (a, d)
int a, d;
{
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		-- Generate a write transaction \n");
    fprintf (tbsrc, "		address    <= X\"%.8x\";\n", a);
    fprintf (tbsrc, "		write_data <= X\"%.8x\";\n", d);
    fprintf (tbsrc, "		rnw        <= '0';\n");
    fprintf (tbsrc, "		go         <= '1';\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf (tbsrc, "		wait until done = '1';\n");
    fprintf (tbsrc, "		go         <= '0';\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf (tbsrc, "		address    <= X\"00000000\";\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf (tbsrc, "		wait for simulation_interval;\n");
    fprintf (tbsrc, "\n");

    if (a == SYS_AXI_BASE && d == 0x80)
    {
        fprintf (tbsrc, "		write(my_line, string'(\"CNN starts\")); -- formatting\n");
        fprintf (tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
        fprintf (tbsrc, "\n");
        fprintf (tbsrc, "		wait until interrupt_request = '1';\n");
        fprintf (tbsrc, "		write(my_line, string'(\"CNN finished\")); -- formatting\n");
        fprintf (tbsrc, "		writeline(output, my_line);     -- write to \"output\"\n");
        fprintf (tbsrc, "\n");
    }
}

void
read_transaction (a)
int a;
{
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		-- Generate a read transaction \n");
    fprintf (tbsrc, "		address    <= X\"%.8x\";\n", a);
    fprintf (tbsrc, "		write_data <= X\"00000000\";\n");
    fprintf (tbsrc, "		rnw        <= '1';\n");
    fprintf (tbsrc, "		go         <= '1';\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
               fprintf (tbsrc, "hwrite(my_line, axi_rdata_r);\n");
               fprintf (tbsrc, "writeline(output, my_line);     -- write to \"output\"\n");
    fprintf (tbsrc, "		wait until done = '1';\n");
    fprintf (tbsrc, "		go         <= '0';\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf (tbsrc, "		address    <= X\"00000000\";\n");
    fprintf (tbsrc, "\n");
    fprintf (tbsrc, "		wait for AXI_ACLK_period;\n");
    fprintf (tbsrc, "		wait for simulation_interval;\n");

               fprintf (tbsrc, "write(my_line, string\'(\"address \"));\n");
               fprintf (tbsrc, "hwrite(my_line, axi_araddr_r);\n");
               fprintf (tbsrc, "write(my_line, string\'(\" data \"));\n");


}
