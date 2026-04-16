#ifndef EMUL
#define EMUL

#include "memory.h"
typedef struct 
{
    PDP11_FUNC type;

    char name[10];

    uint16_t mask;      // маска для сравнения
    uint16_t pattern;   // паттерн для сравнения
    uint8_t is_byte;    // 1 = байтовая операция, 0 = словная
} CommandInfo;

void execution_ram(PDP_11* pdp_11);

PDP11_FUNC defining_type_command(PDP_11* pdp_11);


#endif