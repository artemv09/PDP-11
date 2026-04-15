#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#include "memory.h"
#include "compiler.h"

int main()
{
    PDP_11* pdp_11 = init();

    FILE* file = fopen("code.txt", "r");
    transfer_file_code(file, pdp_11);

    FILE* processed_file = fopen("processed_file.txt", "r");

    if (!processed_file) 
    {
        printf("Ошибка открытия для чтения\n");
        return 1;
    }

    transfer_byte(processed_file, pdp_11);
    
    pdp_11 -> reg_arr[PC] = 1000;
    dump_memory_words(pdp_11, 1000, 1010);


    return 0;
}