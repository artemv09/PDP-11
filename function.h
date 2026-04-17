#ifndef FUNC
#define FUNC

#include "memory.h"

int function_execution(PDP_11* pdp_11, PDP11_FUNC type);

uint16_t reading_mod(int number_arg, uint16_t opcode);

uint16_t reading_reg_name(int number_arg, uint16_t opcode);

#endif