#ifndef COMP
#define COMP

#include "memory.h"

typedef struct 
{
    uint8_t code;    // 6-битный код (mode<<3 | reg)
    uint16_t imm;    // непосредственное значение (если есть)
    int has_imm;     // 1 если нужно доп. слово
} Arg;

PDP11_FUNC get_func(char* buffer) ;

char* delet_comment(char* buffer);

void transfer_file_code(FILE* file, PDP_11* pdp_11) ;

Arg parse_arg(const char* str, PDP_11* pdp_11);

void writing_memory(FILE* file, PDP_11* pdp_11, uint16_t low);

int metca_found(char* buffer, PDP_11* pdp_11);

void read_metca_first(FILE* file, PDP_11* pdp_11);

void completion_code_seg(FILE* file, PDP_11* pdp_11);

uint8_t* transfer_byte(FILE* file, PDP_11* pdp_11);

int completion_data_string(FILE* file, PDP_11* pdp_11);

int completion_data_seg(FILE* file, PDP_11* pdp_11);

int byte_type_definition(char* name);

#endif