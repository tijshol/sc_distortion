#include <systemc.h>
#include "AXISlave.h"
#include "../sw/mmap.h"

#undef ADEBUG

// Based on revision 4 vhdl code

void AXISlave::read_fsm (void)
{
    enum rstate_t { IDLE, READ_ADDRESS, WAIT_RDATA, READ_RESPONSE} ;

    // registered state variables
    rstate_t v_rstate_r;
    sc_uint <32> v_rdata_r;
    sc_uint <32> v_raddr_r;

    // set reset values
    v_rstate_r = IDLE;
    v_rdata_r = 0x0;
    s_axi_arready_r.write(false);
    s_axi_rvalid_r.write(false);

    wait();

    while (true)
    {

        switch (v_rstate_r)
        {
        case IDLE:	// rstate = 0
            s_axi_arready_r.write(false);
            s_axi_rvalid_r.write(false);
            v_rstate_r = READ_ADDRESS;
            break;

        case READ_ADDRESS:	// rstate = 0

            v_raddr_r = s_axi_araddr.read();
            if (s_axi_arvalid.read () == (true)
                    && (v_raddr_r >= SYS_AXI_BASE && v_raddr_r < SYS_AXI_TOP )){
                s_axi_araddr_reg_r.write(v_raddr_r - SYS_AXI_BASE);
                //s_ip_valid_r.write(true);
		s_ip_rvalid.write(true);

                v_rstate_r = WAIT_RDATA;
            }


            break;


        case WAIT_RDATA:	// rstate = 2
            s_axi_arready_r.write(true);

            while (s_ip_rready.read() == true)   wait();

            s_axi_rvalid_r.write(true);
            //s_ip_valid_r.write (false);
		s_ip_rvalid.write(false);
            //s_axi_rdata_r.write(s_ip_rdata_r.read());

            v_rstate_r = READ_RESPONSE;

            break;

        case READ_RESPONSE:	// rstate = 3
            while (s_ip_rready.read() == false)   wait();
            s_axi_rresp_r.write (AXI_OKAY);
            v_rstate_r = IDLE;
            break;

        }
        wait();
    }
}

void AXISlave::write_fsm (void)
{

    enum wstate_t {IDLE, WRITE_ADDRESS, UPDATE_REGISTER};

    wstate_t v_wstate_r;
    sc_uint <32> v_waddr_r;

    // set reset values
    v_wstate_r = IDLE;

    s_axi_awready_r.write(false);
    s_axi_wready_r.write(false);
    s_axi_bvalid_r.write(false);

    s_ip_wvalid_r.write(false);

    wait();

    while (true) {


        switch (v_wstate_r)
        {
            // Wait for the start of a write transaction, which may be
            // initiated by either of the following conditions:
            //   * assertion of both AWVALID and WVALID
            //   * assertion of AWVALID
            //   * assertion of WVALID

        case IDLE:	// wstate = 0
        {
            s_axi_wready_r.write (false);
            s_axi_awready_r.write (false);
            s_axi_bvalid_r.write(false);

            v_wstate_r = WRITE_ADDRESS;

            break;
        }

        case WRITE_ADDRESS:	// wstate = 0
        {
            v_waddr_r =s_axi_awaddr.read();

            if (s_axi_awvalid.read() == (true) && s_axi_wvalid.read() == (true)
                    &&  (v_waddr_r >= SYS_AXI_BASE && v_waddr_r < SYS_AXI_TOP)){
                s_axi_awaddr_reg_r.write (v_waddr_r - SYS_AXI_BASE);
                s_axi_wdata_reg_r.write (s_axi_wdata.read());
                s_axi_wstrb_reg_r.write(s_axi_wstrb.read());
                s_ip_wvalid_r.write (true);

                s_axi_awready_r.write (true);

                v_wstate_r = UPDATE_REGISTER;
            }
            else
                v_wstate_r = IDLE;
            break;
        }
        case UPDATE_REGISTER:	// wstate = 3
        {

            while (s_ip_wready.read()== false) wait();

            s_axi_wready_r.write(true);

            while (s_axi_bready.read()== false) wait();

            s_axi_bvalid_r.write(true);
            s_axi_bresp_r.write(AXI_OKAY);
            s_ip_wvalid_r.write (false);
            v_wstate_r = IDLE;
            break;
        }
        }
        wait();
    }
}


void AXISlave::outputs (void)
{

    s_axi_awready.write(s_axi_awready_r.read());
    s_axi_wready.write(s_axi_wready_r.read());
    s_axi_bvalid.write(s_axi_bvalid_r.read());
    s_axi_bresp.write(s_axi_bresp_r.read());

    s_axi_arready.write(s_axi_arready_r.read());
    s_axi_rvalid.write(s_axi_rvalid_r.read());
    s_axi_rresp.write(s_axi_rresp_r.read());
    //s_axi_rdata.write(s_axi_rdata_r.read());
    s_axi_rdata.write(s_ip_rdata_r.read());

    s_ip_raddr_r.write(s_axi_araddr_reg_r.read());
    //s_ip_rvalid.write(s_ip_valid_r.read());

    s_ip_waddr_r.write(s_axi_awaddr_reg_r.read());
    s_ip_wdata_r.write(s_axi_wdata_reg_r.read());
    s_ip_wstrb_r.write(s_axi_wstrb_reg_r.read());
    s_ip_wvalid.write(s_ip_wvalid_r.read());

}
