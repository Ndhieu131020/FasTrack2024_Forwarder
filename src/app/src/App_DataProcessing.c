/*
 *  Filename: App_DataProcessing.c
 *
 *  Created on: 11-18-2024
 *      Author: Ndhieu131020@gmail.com
*/
#include "App_DataProcessing.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Function to parser the string and extract 2 unsigned integers
  * @param[in]  str   Pointer to the input string to be parsed.
  * @param[in]  length Length of the input string (excluding any null terminator).
  * @param[out] Output  Pointer to store Message frame struct (contain ID and Data).
  * @return true if the parsing is successful and both numbers are extracted, false if the input format is invalid.
  */
bool App_Parser_UARTFrame(const uint8_t* str, int length, ReceiveFrame_t *Output)
{
    uint32_t temp1           = 0u;
    uint32_t temp2           = 0u;
    uint8_t idx              = 0u;
    uint8_t is_second_number = 0u;

    /* Pass through each character in the string */
    while (idx < length)
    {
        if (str[idx] >= '0' && str[idx] <= '9')
        {
            if (is_second_number)
            {
                temp2 = temp2 * 10 + (str[idx] - '0');
            } else
            {
                temp1 = temp1 * 10 + (str[idx] - '0');
            }
        }
        else if (str[idx] == '-')
        {
            if (is_second_number)
            {
                return false; /* Invalid string (more than one '-') */
            }
            is_second_number = 1; /* Switch to parsing the second number */
        }
        else
        {
            return false; /* Invalid character */
        }
        idx++;
    }

    Output->ID   = temp1;
    Output->Data = temp2;

    return true;
}