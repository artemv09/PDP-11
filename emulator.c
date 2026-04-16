#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#include "memory.h"
#include "compiler.h"
#include "emulator.h"
#include "function.h"

CommandInfo commands[] = {
    // HALT
    {HALT, "HALT", 0xFFFF, 0x0000, 0},
    
    // Безусловный переход
    {BR, "BR", 0xFF00, 0x0400, 0},
    
    // Условные переходы
    {BEQ, "BEQ", 0xFF00, 0x1400, 0},
    {BNE, "BNE", 0xFF00, 0x1000, 0},
    {BPL, "BPL", 0xFF00, 0x8000, 0},
    
    // SOB
    {SOB, "SOB", 0xFE00, 0x3E00, 0},
    
    // JSR и RTS
    {JSR, "JSR", 0xFC00, 0x0400, 0},
    {RTS, "RTS", 0xFFF0, 0x0200, 0},
    
    // Двухоперандные
    {MOV, "MOV", 0xF000, 0x1000, 0},
    {MOVB, "MOVB", 0xF000, 0x9000, 1},
    {ADD, "ADD", 0xF000, 0x6000, 0},
    
    // Однооперандные
    {CLR, "CLR", 0xFF00, 0x5000, 0},
    {TST, "TST", 0xFF00, 0x5700, 0},
    {TSTB, "TSTB", 0xFF00, 0x5700, 1},  // бит 15 = 1 отличает от TST
    
    // Последний элемент
    {UNKNOWN, "UNKNOWN", 0x0000, 0x0000, 0}
};


void execution_ram(PDP_11* pdp_11)
{
    pdp_11 -> reg_arr[PC] = 1000;

    PDP11_FUNC type_func = defining_type_command(pdp_11);
    
    while(type_func != HALT)
    {
        function_execution(pdp_11, type_func);
    }

    return;
}

// написать программу для распощзнавания функции при выгрузке из памяти

PDP11_FUNC defining_type_command(PDP_11* pdp_11) 
{
    uint16_t opcode = read_comand_RAM(pdp_11);

    int count = 0;

    while((opcode & commands[count].mask) != commands[count].pattern)
    {
        count++;
    }

    printf("%s\n", commands[count].name);

    return commands[count].type;
}