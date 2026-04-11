#ifndef MEMORY
#define MEMORY

#define RAM_SIZE 64000 //64 кбайт
#define NUM_REGS 8

enum REGISTER
{
    R0 = 0,
    R1 = 1,
    R2 = 2,
    R3 = 3,
    R4 = 4,
    R5 = 5,
    SP = 6,
    PC = 7
};

typedef struct 
{
    uint8_t ram[RAM_SIZE];
    uint16_t reg_arr[NUM_REGS];
    uint8_t pwd;

} PDP_11;



#endif