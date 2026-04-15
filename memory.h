#ifndef MEMORY
#define MEMORY

#define RAM_SIZE 64000 //64 кбайт
#define NUM_REGS 8

typedef enum 
{
    MOV,
    MOVB,
    ADD,
    CLR,
    SOB,
    BEQ,
    BNE,
    BR,
    BPL,
    JSR,
    RTS,
    TSTB,
    TST,
    HALT,
    UNKNOWN
} PDP11_FUNC;

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
    char* name_metca;
    int adrecc;
} Metca;

typedef struct 
{
    uint8_t ram[RAM_SIZE];
    uint16_t reg_arr[NUM_REGS];
    uint8_t pwd;

    size_t count_metca = 0; // число меток

    int completion_ram = 0; // указатель на память во время заполнения

    Metca metca_arr[64];

    int flag_data_100 = 0;
    int flag_data_200 = 0;

} PDP_11;


PDP_11* init();

uint16_t read_comand_RAM(PDP_11* pdp_11);

void dump_memory_words(PDP_11* pdp, uint16_t start_addr, uint16_t end_addr);

#endif