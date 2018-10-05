#include <systemc.h>
#include "axi_if.h"

#undef ADEBUG

// Based on revision 4 vhdl code

#define READ_IDLE 0
#define READ_REGISTER 1
#define READ_WAIT_MEMORY_RDATA 2
#define READ_RESPONSE 3
#define READ_DONE 4
#define READ_ERROR 5

#define MEM_WAIT_COUNT 2

void axi_if::read_state (void)
{
    s_rstate.write(READ_IDLE);
    wait();
    while (true)
    {
        wait();
        s_rstate.write(s_rnstate.read());
    }
}

void axi_if::read_nstate (void)
{
    sc_uint <S_AXI_ARADDR> v_axi_araddr_reg_r;
    sc_uint <S_AXI_ARADDR> v_araddr_reg;

    sc_uint <16> v_mem_wait_count_r;

    s_rnstate.write(READ_IDLE);
    s_axi_arready_r.write (false);
    s_axi_rvalid_r.write (false);
    s_axi_rresp_r.write ((sc_uint < S_AXI_RRESP > (0)));
    s_axi_rdata_r.write ((sc_uint < S_AXI_RDATA > (0)));

    s_axi_rdata .write((sc_uint < S_AXI_WDATA >  (0)));
    s_ip_raddr .write((sc_uint < S_AXI_AWADDR >  (0)));
    s_ip_rvalid .write( (false));

    wait();

    while (true)
    {
        switch (s_rstate.read())
        {
        case READ_IDLE:
            v_mem_wait_count_r = 0;
            s_axi_rvalid_r.write(false);
            s_axi_rdata_r.write(0x0);

            if (s_axi_arvalid.read () == (true))
            {
                s_axi_araddr_reg_r.write(s_axi_araddr.read());
                s_axi_arready_r.write(true);
                s_rnstate.write(READ_REGISTER);
            }
            else
            {
                s_rnstate.write(READ_IDLE);
            }
            break;
        case READ_REGISTER:
            s_axi_arready_r.write(false);

            s_ip_raddr.write(s_axi_araddr_reg_r.read());
            s_ip_rvalid.write(true); // START SEND

            s_rnstate.write(READ_WAIT_MEMORY_RDATA);
            break;

        case READ_WAIT_MEMORY_RDATA:
            s_ip_rvalid.write(false); // 1 CLOCK
            s_axi_rdata.write(s_ip_rdata.read());

            if (v_mem_wait_count_r == MEM_WAIT_COUNT-1 ) {
                s_rnstate.write(READ_RESPONSE);
                s_axi_rvalid_r.write(true);
            } else {
                v_mem_wait_count_r = v_mem_wait_count_r + 1;
                s_rnstate.write(READ_WAIT_MEMORY_RDATA);
            }
            break;

        case READ_RESPONSE:
            s_axi_rresp_r.write (AXI_OKAY);
            s_rnstate.write(READ_DONE);
            break;


        case READ_DONE:
            while (s_axi_rready.read() != true) wait();
            s_axi_rvalid_r.write (false);
            s_rnstate.write(READ_IDLE);
            break;
        }
        wait();
    }
}

#define WRITE_IDLE				0
#define WRITE_ADDR_FIRST		1
#define WRITE_DATA_FIRST		2
#define WRITE_UPDATE_REGISTER	3
#define WRITE_ERROR				4
#define WRITE_DONE				5

void axi_if::write_state (void)
{
    s_wstate.write(WRITE_IDLE);
    wait();
    while (true)
    {
        wait();
        s_wstate.write(s_wnstate.read());
    }
}

void axi_if::write_nstate (void)
{
    sc_uint <S_AXI_ARADDR> v_axi_awaddr_reg_r;
    sc_uint <S_AXI_ARADDR> v_awaddr_reg;
    sc_uint<4> v_axi_wstrb_reg_r;
    sc_uint<32> v_axi_wdata_reg_r;

    s_wnstate.write(WRITE_IDLE);

    s_axi_awready_r.write (false);
    s_axi_wready_r.write (false);
    s_axi_awaddr_reg_r.write ((sc_uint < S_AXI_AWADDR > (0)));
    s_axi_wdata_reg_r.write ((sc_uint < S_AXI_WDATA > (0)));
    s_axi_wstrb_reg_r.write ((sc_uint < S_AXI_WSTRB > (0)));
    s_axi_bvalid_r.write (false);
    s_axi_bresp_r.write ((sc_uint < S_AXI_BRESP > (0)));

    s_ip_waddr .write((sc_uint < S_AXI_AWADDR >  (0)));
    s_ip_wdata .write((sc_uint < S_AXI_WDATA >  (0)));
    s_ip_wstrb .write((sc_uint < S_AXI_WSTRB >  (0)));
    s_ip_wvalid .write( (false));

    wait();
    while (true)
    {
        switch (s_wstate.read())
        {
        case WRITE_IDLE:
        {
            if (s_axi_awvalid.read() == (true) && s_axi_wvalid.read() == (true))
            {
                s_axi_awaddr_reg_r.write (s_axi_awaddr.read());
                s_axi_awready_r.write (true);
                s_axi_wdata_reg_r.write (s_axi_wdata.read());
                s_axi_wstrb_reg_r.write(s_axi_wstrb.read());
                s_axi_wready_r.write (true);

                s_wnstate.write(WRITE_UPDATE_REGISTER);
            }
            else if (s_axi_awvalid.read() == (true))
            {
                s_axi_awaddr_reg_r.write (s_axi_awaddr.read());
                s_axi_awready_r.write (true);
                s_wnstate.write(WRITE_ADDR_FIRST);
            }
            else if (s_axi_wvalid.read() == (true))
            {
                s_axi_wdata_reg_r.write (s_axi_wdata.read());
                s_axi_wstrb_reg_r.write (s_axi_wstrb.read());
                s_axi_wready_r.write (true);
                s_wnstate.write(WRITE_DATA_FIRST);
            }
            else {
                s_axi_awready_r.write (false);
                s_axi_wready_r.write (false);
                s_axi_bvalid_r.write (false);
                s_ip_wvalid.write(false);
                s_wnstate.write(WRITE_IDLE);
            }
            break;
        }
        case WRITE_ADDR_FIRST:
        {
            if (s_axi_wvalid.read() == (true))
            {
                s_axi_wdata_reg_r.write (s_axi_wdata.read());
                s_axi_wstrb_reg_r.write (s_axi_wstrb.read());
                s_axi_wready_r.write (true);
                s_wnstate.write(WRITE_UPDATE_REGISTER);
            }
            else{
                s_wnstate.write(WRITE_ADDR_FIRST);
            }
            break;
        }
        case WRITE_DATA_FIRST:
        {
            if (s_axi_awvalid.read() == (true))
            {
                s_axi_awaddr_reg_r.write (s_axi_awaddr.read());
                s_axi_awready_r.write (true);
                s_wnstate.write(WRITE_UPDATE_REGISTER);
            }
            else{
                s_wnstate.write(WRITE_DATA_FIRST);
            }
            break;
        }
        case WRITE_UPDATE_REGISTER:
        {
            s_axi_bresp_r.write (AXI_OKAY);
            s_axi_bvalid_r.write (true);

            v_axi_wstrb_reg_r = s_axi_wstrb_reg_r.read();
            v_axi_wdata_reg_r = s_axi_wdata_reg_r.read();

            //receive(); START RECEIVE
            s_axi_wready_r.write (false);
            s_axi_awready_r.write (false);
            s_ip_waddr.write(s_axi_awaddr_reg_r.read());
            s_ip_wdata.write(s_axi_wdata_reg_r.read());
            s_ip_wstrb.write(s_axi_wstrb_reg_r.read());
            s_ip_wvalid.write(true);

            s_wnstate.write(WRITE_DONE);
        }
        break;
        case WRITE_DONE:
            while (s_axi_bready.read() != true) wait();
            s_axi_bvalid_r.write (false);
            s_ip_wvalid.write(false);
            s_wnstate.write(WRITE_IDLE);
            break;
        }
        wait();
    }
}

void axi_if::outputs (void)
{
    s_axi_awready.write(s_axi_awready_r.read());
    s_axi_wready.write(s_axi_wready_r.read());
    //s_axi_wvalid.write(s_axi_wvalid_r.read());
    s_axi_bvalid.write(s_axi_bvalid_r.read());
    s_axi_bresp.write(s_axi_bresp_r.read());
    s_axi_arready.write(s_axi_arready_r.read());
    s_axi_rvalid.write(s_axi_rvalid_r.read());
    s_axi_rresp.write(s_axi_rresp_r.read());
    //s_axi_rdata.write(s_axi_rdata_r.read());
    s_axi_rstate.write(s_rstate.read());
    s_axi_rnstate.write(s_rnstate.read());
    s_axi_wstate.write(s_wstate.read());
    s_axi_wnstate.write(s_wnstate.read());
}
