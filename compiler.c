#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>
#include "memory.h"

const char* pdp11_str[] = {"MOV", "MOVB", "ADD", "CLR", "SOB", "BEQ", "BNE", "BR", "BPL", "JSR", "RTS", "HALT"};

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

        char *ptr_colon = strchr(buffer, ':'); 

        if (ptr_colon != NULL) 
        {
            *ptr_colon = '\0'; 
            
            pdp_11 -> metca_arr[pdp_11 -> count_metca].name_metca = strdup(buffer);
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

uint16_t one_arguemnt_reading(char* buffer, PDP_11* pdp_11)
{
    char buffer_1[8];
    char buffer_2[8];

    int count_2 = 0;
    int count_1 = 0;
    int count = 0;
    int copy = 0;

    while(!isalpha(*(buffer + count)) && !isdigit(*(buffer + count)))
    {
        buffer_1[count_1] = *(buffer + count);
        count++;
        count_1++;
    }

    copy = count;

    while(isalpha(*(buffer + count)) || isdigit(*(buffer + count)))
    {
        buffer_2[count_2] = *(buffer + count);
        count++;
        count_2++;
    }

    while(*(buffer + count) != '\0')
    {
        buffer_1[count_1] = *(buffer + count);
        count++;
        count_1++;
    }

    if(buffer_1[count_1 - 1] == ',')
    {
        buffer_1[count_1 - 1] = '\0';
    }

    buffer_1[count_1] = '\0';
    buffer_2[count_2] = '\0';

    uint8_t hight = 0;
    uint8_t low = 0;

    if(strcmp(buffer_1, "") == 0) //хз может надо поменять
    {
        hight = 0;
    }
    else if(strcmp(buffer_1, "()") == 0|| strcmp(buffer_1, "@") == 0)
    {
        hight = 1;
    }
    else if(strcmp(buffer_1, "()+") == 0)
    {
        hight = 2;
    }
    else if(strcmp(buffer_1, "@()+") == 0)
    {
        hight = 3;
    }
    else if(strcmp(buffer_1, "-()") == 0)
    {
        hight = 4;
    }
    else if(strcmp(buffer_1, "@-()") == 0)
    {
        hight = 5;
    }
    else
    {
        printf("не удавлось считать моду\n");
        return 0;    
    }


    if(isdigit(buffer_2[0])) //хз может надо поменять
    {
        low = 7;
        pdp_11 -> ram[pdp_11 -> completion_ram + 2] = atoi(buffer_2);
    }
    else if(strcmp(buffer_2, "R0") == 0)
    {
        low = 0;
    }
    else if(strcmp(buffer_2, "R1") == 0)
    {
        low = 1;
    }
    else if(strcmp(buffer_2, "R2") == 0)
    {
        low = 2;
    }
    else if(strcmp(buffer_2, "R3") == 0)
    {
        low = 3;
    }
    else if(strcmp(buffer_2, "R4") == 0)
    {
        low = 4;
    }
    else if(strcmp(buffer_2, "R5") == 0)
    {
        low = 5;
    }
    else
    {
        printf("не удавлось считать номер регистра\n");
        return 0;    
    }

    uint16_t arg = 0;
    arg = (hight << 3) | low;
    
    return arg;
}

void completion_code_seg(FILE* file, PDP_11* pdp_11)
{
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

                pdp_11 -> completion_ram = 1000;

                fscanf(file, "%s", buffer);
                uint16_t arg1 = one_arguemnt_reading(buffer, pdp_11);

                fscanf(file, "%s", buffer);
                uint16_t arg2 = one_arguemnt_reading(buffer, pdp_11);

                uint16_t rez = low | (arg1 << 6) | arg2;

                uint16_t copy_rez = rez;

                pdp_11 -> ram[pdp_11 -> completion_ram] = rez;

                pdp_11 -> ram[(pdp_11 -> completion_ram)  + 1] = copy_rez >> 8;

                uint16_t MASK_BITS_11_10_9 = 0x0E00;

                if((MASK_BITS_11_10_9 & rez) != 0)
                {
                    pdp_11 -> completion_ram  += 2; 
                }
                else
                {
                   pdp_11 -> completion_ram  += 4; 
                }

                break;
            }
            case MOVB:
                
                break;
            case ADD:
                
                break;
            case CLR:
                
                break;
            case SOB:
                
                break;
            case BEQ:
                
                break;
            case BNE:
                
                break;
            case BR:
                
                break;
            case BPL:
                
                break;
            case JSR:
                
                break;
            case RTS:
                
                break;
            case HALT:
                
                break;
            default:
                break;
        }
    }
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