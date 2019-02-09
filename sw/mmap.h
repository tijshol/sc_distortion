
#include <inttypes.h>

//#define RAM_BASEADDR  0xB1000000
//#define SLV_BASEADDR   0xC0000000

#define  SYS_MEM_BASE           0x00000000
#define  SYS_MEM_SIZE           0x04000000

// #define  SYS_IRQC_BASE          0x17000000
#define  SYS_IRQC_BASE          (SYS_MEM_BASE + SYS_MEM_SIZE)
#define  SYS_IRQC_SIZE          0x0000FFFF

// #define  SYS_CONS_BASE          0x40000000
#define  SYS_CONS_BASE          (SYS_IRQC_BASE + SYS_IRQC_SIZE)
#define  SYS_CONS_SIZE          0x0000FFFF

// #define  SYS_INPUT_RAM_BASE     0xB2000000
#define  SYS_INPUT_RAM_BASE     (SYS_CONS_BASE + SYS_CONS_SIZE)
#define  SYS_INPUT_RAM_SIZE     0x05000000

// #define  SYS_OUTPUT_RAM_BASE    0xC6000000
#define  SYS_OUTPUT_RAM_BASE    (SYS_INPUT_RAM_BASE + (SYS_INPUT_RAM_SIZE * 4))
#define  SYS_OUTPUT_RAM_SIZE    0x05000000

// #define  SYS_AXI_BASE           0xE0000000
#define  SYS_AXI_BASE           (SYS_OUTPUT_RAM_BASE + (SYS_OUTPUT_RAM_SIZE * 4))
#define  SYS_AXI_SIZE           (0xFFFFFFFF - SYS_AXI_BASE)

#define SYS_MEM32(mem_addr)  (*((volatile uint32_t *)mem_addr)) 

