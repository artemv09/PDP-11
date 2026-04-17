#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cstdint>

#include "memory.h"
#include "compiler.h" 
#include "function.h"

int function_execution(PDP_11* pdp_11, PDP11_FUNC type) // 1 если опереация выполнена и 0 если нет
{
    uint16_t opcode = read_comand_RAM(pdp_11); // получили нормальное значение 

    switch (type)
    {
        case MOV:
        {
            uint16_t mod_1 = reading_mod(1, opcode);
            uint16_t mod_2 = reading_mod(2, opcode);
            uint16_t reg_1 = reading_reg_name(1, opcode);
            uint16_t reg_2 = reading_reg_name(2, opcode);

            printf("%d %d %d %d", mod_2, reg_2, mod_1, reg_1);

            switch (mod_2)
            {
                case 0:
                {
                    pdp_11 -> reg_arr[reg_2] = pdp_11 -> reg_arr[reg_1];
                    break;
                }
                case 1:
                {     
                    break;
                }
                case 2:
                {
                    if(reg_2 == 7)
                    { 
                        pdp_11 -> reg_arr[PC] += 2;
                        uint16_t in_reg = read_comand_RAM(pdp_11);
                        pdp_11 -> reg_arr[reg_1] = in_reg;
                    }
                    else
                    {
                        pdp_11 -> reg_arr[PC] += 2;
                        pdp_11 -> reg_arr[reg_1] = read_comand_RAM(pdp_11);
                    }
                    break;
                }
                case 3:
                {  
                    break;
                }
                case 4:
                {
                    uint16_t pc_const = pdp_11 -> reg_arr[PC];
                    pdp_11 -> reg_arr[PC] = pdp_11 -> reg_arr[reg_2];
                    pdp_11 -> reg_arr[reg_2] -= 2;
                    pdp_11 -> reg_arr[reg_1] = read_comand_RAM(pdp_11);
                    pdp_11 -> reg_arr[PC] = pc_const;
                    break;
                }
                case 5:
                {
                        
                    break;
                }
                case 6:
                {
                    if(reg_2 == 7)
                    {
                        pdp_11 -> reg_arr[reg_1];
                    }
                    break;
                }
                case 7:
                {
                        
                    break;
                }
                default:
                {
                    printf("Ошибка в чтении моды, такой моды не существует\n");
                    break;
                }
            }
                
            }
        case MOVB:
        {
           
           break;
        }
        case ADD:
        {
            break;
        }
        case CLR:
        {              
            break;
        }
        case SOB:
        {
            break;
        }
        case BEQ:
        {
            break;
        }
        case BNE:
        {

            break;
        }
        case BR:
        {
            break;
        }
        case BPL:
        {
            break;
        }
        case JSR:
        {
    
            break;
        }
        case RTS:
        {
            break;
        }
        case TSTB:
        {
            break;
        }
        case TST:
        {
            break;
        }
        case HALT:
        {
            break;
        }
        default:
            return 0;
    }

    pdp_11 -> reg_arr[PC] += 2;

    return 0;
}


uint16_t reading_mod(int number_arg, uint16_t opcode)
{
    if(number_arg == 1)
    {
        uint16_t mask_1 = 0x0038;
        return (opcode & mask_1) >> 3;
    }
    else
    {
        uint16_t mask_2 = 0x0E00;
        return (opcode & mask_2) >> 9;
    }
}

uint16_t reading_reg_name(int number_arg, uint16_t opcode)
{
    if(number_arg == 1)
    {
        uint16_t mask_1 = 0x0007;
        return (opcode & mask_1);
    }
    else
    {
        uint16_t mask_2 = 0x01C0;
        return (opcode & mask_2) >> 6;
    }
}