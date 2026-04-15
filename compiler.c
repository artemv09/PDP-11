#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#include "memory.h"
#include "compiler.h"

const char* pdp11_str[] = {"MOV", "MOVB", "ADD", "CLR", "SOB", "BEQ", "BNE", "BR", "BPL", "JSR", "RTS", "TSTB", "TST", "HALT"};

PDP11_FUNC get_func(char* buffer) 
{
    int count = sizeof(pdp11_str) / sizeof(pdp11_str[0]);
    
    for (int i = 0; i < count; i++) 
    {
        if (strcmp(buffer, pdp11_str[i]) == 0) 
        {
            return (PDP11_FUNC) i;
        }
    }
    return UNKNOWN;
}

char* delet_comment(char* buffer) 
{
    char *ptr = strchr(buffer, ';'); 
    if (ptr != NULL) 
    {
        *ptr = '\n';    
        *(ptr + 1) = '\0'; 
    }
    return buffer;
}

void transfer_file_code(FILE* file, PDP_11* pdp_11) 
{
    FILE* processed_file = fopen("processed_file.txt", "w");

    if (file == NULL || processed_file == NULL) return;

    size_t buf_size = 128;

    char* buffer = (char*)calloc(buf_size, sizeof(char));

    int count = 1000;

    while (fgets(buffer, buf_size, file) != NULL) 
    {
        delet_comment(buffer);

        if (buffer[0] == '\n' || buffer[0] == '\0') 
        {
            continue;
        }

        char* ptr_colon = strchr(buffer, ':'); 

        if (ptr_colon != NULL) 
        {
            *ptr_colon = '\0'; 
            
            pdp_11 -> metca_arr[pdp_11 -> count_metca].name_metca = strdup(buffer);
            printf("[[[%s]]]", buffer);
            pdp_11 -> metca_arr[pdp_11 -> count_metca].adrecc = count;
            pdp_11 -> count_metca++;         
        } 
        else 
        {
            fprintf(processed_file, "%s", buffer);
            count += 2; 
        }

        memset(buffer, 0, buf_size);
    }

    free(buffer);
    fclose(file);
    fclose(processed_file);

    return;
}

//==============================================================
Arg parse_arg(char* str) 
{
    Arg arg = {0};
    char buf[64];
    strcpy(buf, str);
    
    // Убираем запятую в конце
    char* comma = strchr(buf, ',');

    if(comma) 
    {
        *comma = '\0';
    }
    // Убираем пробелы
    char* s = buf;
    while (isspace(*s)) s++;
    
    // #n — непосредственное
    if (s[0] == '#') 
    {
        arg.code = (2 << 3) | 7;  // mode=2, reg=7
        arg.imm = (s[1] == '\'') ? s[2] : atoi(s + 1);
        arg.has_imm = 1;
        return arg;
    }
    
    // @#addr — абсолютная
    if (s[0] == '@' && s[1] == '#') 
    {
        arg.code = (3 << 3) | 7;
        arg.imm = strtol(s + 2, NULL, 8);
        arg.has_imm = 1;
        return arg;
    }
    
    // Определяем моду по первому символу
    int mode = 0;

    if (s[0] == '@') 
    { 
        mode = (strchr(s, '+')) ? 3 : (s[1] == '-' ? 5 : 1); 
    }
    else if (s[0] == '(') 
    { 
        mode = (strchr(s, '+')) ? 2 : 1; 
    }
    else if (s[0] == '-') 
    { 
        mode = (strstr(s, "@-")) ? 5 : 4; 
    }
    
    // Ищем регистр или число
    char* reg_str = s;

    while (*reg_str && !isalpha(*reg_str) && !isdigit(*reg_str)) reg_str++;
    
    int reg = 7;  // по умолчанию PC
    
    // Если это число — адрес
    if (isdigit(*reg_str) || *reg_str == '-') 
    {
        if (mode == 0) mode = 6;
        else if (mode == 1) mode = 7;

        arg.imm = strtol(reg_str, NULL, 8);
        arg.has_imm = 1;
    }
    // Регистры
    else 
    {
             if (strcasecmp(reg_str, "R0") == 0) reg = 0;
        else if (strcasecmp(reg_str, "R1") == 0) reg = 1;
        else if (strcasecmp(reg_str, "R2") == 0) reg = 2;
        else if (strcasecmp(reg_str, "R3") == 0) reg = 3;
        else if (strcasecmp(reg_str, "R4") == 0) reg = 4;
        else if (strcasecmp(reg_str, "R5") == 0) reg = 5;
        else if (strcasecmp(reg_str, "SP") == 0) reg = 6;
        else if (strcasecmp(reg_str, "PC") == 0) reg = 7;
    }
    
    arg.code = (mode << 3) | reg;

    char bin_str[17];
        for (int i = 15; i >= 0; i--) 
        {
            bin_str[15 - i] = (arg.code & (1 << i)) ? '1' : '0';
            if (i == 8) bin_str[15 - i] = ' ';
        }
        bin_str[16] = '\0';
        printf("[[%s]]", bin_str);

    return arg;
}
//==============================================================

void writing_memory(FILE* file, PDP_11* pdp_11, uint16_t low)
{
    char buffer[64];
    fscanf(file, "%s", buffer);
    Arg arg1 = parse_arg(buffer);

    fscanf(file, "%s", buffer);
    Arg arg2 = parse_arg(buffer);

    uint16_t rez = low | (arg1.code << 6) | arg2.code;

    uint16_t copy_rez = rez;

    pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

    pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = copy_rez >> 8;


    if((arg1.has_imm) == 0)
    {
        pdp_11 -> completion_ram  += 2; 
    }
    else
    {
        pdp_11 -> ram[(pdp_11 -> completion_ram) + 2] = arg1.imm;
        pdp_11 -> ram[(pdp_11 -> completion_ram) + 3] = arg1.imm >> 8;
        pdp_11 -> completion_ram  += 4; 
    }

    return;
} 

void completion_code_seg(FILE* file, PDP_11* pdp_11)
{
    pdp_11 -> completion_ram = 1000;

    char buffer[64];

    while(fscanf(file, "%s", buffer) != EOF)
    {
        switch (get_func(buffer))
        {
            //будет собирать цифру и потом классть правильно в память
            case MOV:
            {
                printf("все получилось считать\n");
                uint16_t low = 1;
                low = low << 12;

                writing_memory(file, pdp_11, low);

                break;
            }
            case MOVB:
            {
                printf("все получилось считать\n");
                uint16_t low = 9;
                low = low << 12;

                writing_memory(file, pdp_11, low);

                break;
            }
            case ADD:
            {
                printf("все получилось считать\n");
                uint16_t low = 6;
                low = low << 12;

                writing_memory(file, pdp_11, low);

                break;
            }
            case CLR:
            {              
                uint16_t low = 40;
                low = low << 6;  

                fscanf(file, "%s", buffer);
                Arg arg1 = parse_arg(buffer);

                uint16_t rez = low | arg1.code;

                uint16_t copy_rez = rez;

                pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

                pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = copy_rez >> 8;

                pdp_11 -> completion_ram += 2;

                break;
            }
            case SOB:
            {
                uint16_t low = 63;
                low = low << 9;

                fscanf(file, "%s", buffer);
                Arg arg1 = parse_arg(buffer);

                fscanf(file, "%s", buffer);
                char* str = strchr(buffer, '\n');
                if(str) 
                {
                    *str = '\0';
                }

                int adrecc = metca_found(buffer, pdp_11);

                int x = ((pdp_11 -> completion_ram) + 2 - adrecc) / 2;

                arg1.code &= ~0x38;

                uint16_t rez = low | (arg1.code << 6) | (uint8_t) x;

                uint16_t copy_rez = rez;

                pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

                pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = copy_rez >> 8;


                break;
            }
            case BEQ:
            {
                uint16_t low = 12;
                low = low << 8;

                fscanf(file, "%s", buffer);  

                int label_address = metca_found(buffer, pdp_11);

                int pc_after = (pdp_11 -> completion_ram) + 2;

                int offset = (label_address - pc_after) / 2;

                uint8_t offset_byte = (uint8_t)(offset & 0xFF);

                uint16_t rez = low | offset_byte;

                pdp_11 -> ram[pdp_11 -> completion_ram]     = rez & 0xFF;        // младший байт
                pdp_11 -> ram[pdp_11 -> completion_ram + 1] = (rez >> 8) & 0xFF; // старший байт

                pdp_11 -> completion_ram += 2;
                break;
            }
            case BNE:
            {
                uint16_t low = 12;
                low = low << 8;

                fscanf(file, "%s", buffer);  

                int label_address = metca_found(buffer, pdp_11);

                int pc_after = (pdp_11 -> completion_ram) + 2;

                int offset = (label_address - pc_after) / 2;

                uint8_t offset_byte = (uint8_t)(offset & 0xFF);

                uint16_t rez = low | offset_byte;

                pdp_11 -> ram[pdp_11 -> completion_ram]     = rez & 0xFF;        // младший байт
                pdp_11 -> ram[pdp_11 -> completion_ram + 1] = (rez >> 8) & 0xFF; // старший байт

                pdp_11 -> completion_ram += 2;
                break;
            }
            case BR:
            {
                uint16_t low = 6;
                low = low << 12;
                break;
            }
            case BPL:
            {
                uint16_t low = 6;
                low = low << 12;
                break;
            }
            case JSR:
            {
                uint16_t low = 6;
                low = low << 12;
                break;
            }
            case RTS:
            {
                uint16_t low = 6;
                low = low << 12;
                break;
            }
            case TSTB:
            {
                uint16_t low = 533;
                low = low << 6;

                fscanf(file, "%s", buffer);

                Arg arg1 = parse_arg(buffer);

                uint16_t rez = low | arg1.code;

                uint16_t copy_rez = rez;

                pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

                pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = copy_rez >> 8;

                pdp_11 -> completion_ram += 2;

                break;
            }
            case TST:
            {
                uint16_t low = 21;
                low = low << 6;

                fscanf(file, "%s", buffer);

                Arg arg1 = parse_arg(buffer);

                uint16_t rez = low | arg1.code;

                uint16_t copy_rez = rez;

                pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

                pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = copy_rez >> 8;

                pdp_11 -> completion_ram += 2;

                break;
            }
            case HALT:
            {
                pdp_11 -> ram[pdp_11 -> completion_ram] = 0;
                pdp_11 -> ram[(pdp_11 -> completion_ram) + 1] = 0;

                pdp_11 -> completion_ram += 2;
                break;
            }
            default:
                break;
        }
    }
}

int metca_found(char* buffer, PDP_11* pdp_11)
{
    int count = 0;

    while(count < pdp_11 -> count_metca)
    {
        if(strcmp(pdp_11 -> metca_arr[count].name_metca, buffer) == 0)
        {
            return pdp_11 -> metca_arr[count].adrecc;
        }
        else
        {
            count++;
        }
    }

    printf("такой метки не существует\n");

    return 0;
}

uint8_t* transfer_byte(FILE* file, PDP_11* pdp_11)
{
    char buffer[64];

    fscanf(file, "%s", buffer);
    fscanf(file, "%s", buffer);

    while(fscanf(file, "%s", buffer) != EOF)
    { 
        if(strcmp(buffer, "1000") == 0)
        {
            completion_code_seg(file, pdp_11);
        }
        else if(strcmp(buffer, "200") == 0)
        {
            //completion_data_string(file, pdp_11);
        }
        else if(strcmp(buffer, "100") == 0)
        {
            //completion_data_seg(file, pdp_11);
        }
        else
        {
            printf("Ошибка чтения\n");
            return NULL;
        }
    }

    return NULL;
}