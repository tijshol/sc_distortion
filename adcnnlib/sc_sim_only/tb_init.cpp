#include "tb_init.h"

void tb_init::prc_reset() {
    reset = true;
    start = false;
    wait(30,SC_NS);

    reset = false;
    wait(10,SC_NS);

    start = true;
    //wait(10,SC_NS);
    //start = false;

    while (!done.read()) wait();
    start = false;
};
