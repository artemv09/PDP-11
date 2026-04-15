#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include "memory.h"

PDP_11* init()
{
    PDP_11* pdp_11 = (PDP_11*)calloc(1, sizeof(PDP_11));
    
    memset(pdp_11, 0, sizeof(PDP_11)); //обнулили всю структуру
    
    pdp_11 -> reg_arr[PC] = 1000;
    pdp_11 -> reg_arr[SP] = 6300;

    return pdp_11;
}

uint16_t read_comand_RAM(PDP_11* pdp_11)
{
    int adress = pdp_11 -> reg_arr[PC];
    uint8_t low = pdp_11 -> ram[adress];
    uint8_t hight = pdp_11 -> ram[adress + 1];

    return (hight << 8) | low; //должны получить порядок следования байтов как в файле
}

void dump_memory_words(PDP_11* pdp, uint16_t start_addr, uint16_t end_addr) 
{
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║           ДАМП ПАМЯТИ (16-битные слова)              ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n");
    printf("┌────────────┬────────────┬────────────┬──────────────────┐\n");
    printf("│ Адрес (oct)│ Слово (oct)│  Hex       │  Bin             │\n");
    printf("├────────────┼────────────┼────────────┼──────────────────┤\n");
    
    for (uint16_t addr = start_addr; addr < end_addr; addr += 2) 
    {
        uint16_t word = pdp->ram[addr] | (pdp->ram[addr + 1] << 8);
        
        // Вывод бинарного представления (только первые 8 бит для краткости)
        char bin_str[17];
        for (int i = 15; i >= 0; i--) 
        {
            bin_str[15 - i] = (word & (1 << i)) ? '1' : '0';
        }
        bin_str[16] = '\0';
        
        printf("│ %d       │ %d       │ 0x%04X     │ %s │\n", 
               addr, word, word, bin_str);
    }

    printf("└────────────┴────────────┴────────────┴──────────────────┘\n");
}
