#ifndef COMP
#define COMP

#include "memory.h"

PDP11_FUNC get_func(char* buffer) ;

char* delet_comment(char* buffer);

void transfer_file_code(FILE* file, PDP_11* pdp_11) ;

uint16_t one_arguemnt_reading(char* buffer, PDP_11* pdp_11);

void completion_code_seg(FILE* file, PDP_11* pdp_11);

uint8_t* transfer_byte(FILE* file, PDP_11* pdp_11);

#endif