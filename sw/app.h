#define SAMPLE_SIZE   32 // number of bits per sample
#define EXP_ADD_BITS  16 // Additional bits needed for the calculation of the exponential function

#define SAMPLE_RATE   48000 // kHz
#define SAMPLE_MULT   1
#define SAMPLE_DIV    48000 // 48000/16 = 3000
#define INPUT_SIZE    (( SAMPLE_RATE / SAMPLE_DIV ) *  SAMPLE_MULT)
#define OUTPUT_SIZE   (INPUT_SIZE)

#define NUM_REG       16
// #define AXI_BASE_SIZE (NUM_REG * 32)

#define INPUT_OFFSET  0x10000
#define OUTPUT_OFFSET (INPUT_OFFSET + (INPUT_SIZE * 4))