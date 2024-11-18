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
                return false; // Chuỗi không hợp lệ (nhiều hơn một dấu '-')
            }
            is_second_number = 1; // Chuyển sang đọc số thứ hai
        }
        else
        {
            return false; // Ký tự không hợp lệ
        }
        idx++;
    }

    Output->ID   = temp1;
    Output->Data = temp2;

    return true;
}