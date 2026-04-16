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
            //будет собирать цифру и потом классть правильно в память
            case MOV:
            {
                int mod_1 = reading_mod(1, opcode);
                int mod_2 = reading_mod(2, opcode);

                int reg_1 = reading_reg_name(1, opcode);
                int reg_2 = reading_reg_name(2, opcode);

                if(mod_2 == )

                break;
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
                break;
        }
}

int reading_mod(int number_arg, uint16_t opcode)
{
    if(number_arg == 1)
    {
        int mask_1 = 0x0038;
        return (opcode & mask_1) << 3;
    }
    else
    {
        int mask_2 = 0x0E00;
        return (opcode & mask_2) << 9;
    }
}

int reading_reg_name(int number_arg, uint16_t opcode)
{
    if(number_arg == 1)
    {
        int mask_1 = 0x0007;
        return (opcode & mask_1);
    }
    else
    {
        int mask_2 = 0x01C0;
        return (opcode & mask_2) << 6;
    }
}