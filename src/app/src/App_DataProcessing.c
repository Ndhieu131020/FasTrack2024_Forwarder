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

#define DECIMAL_BASE  (10u)
#define MAX_VALUE_STR  (5u)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static uint8_t UIntToString(uint32_t value, uint8_t *buffer);

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

/**
 * @brief  Function to convert ID and Data to a UART string that can be printed out
 * @param[in]  id       ID of the frame
 * @param[in]  data     Data of the frame
 * @param[out] outputBuffer  Pointer to store the output string
 */
void APP_Compose_UARTFrame(uint32_t id, uint32_t data, uint8_t *outputBuffer)
{
    uint8_t *ptr = outputBuffer;

    ptr += UIntToString(id, ptr);

    *ptr++ = '-';

    ptr += UIntToString(data, ptr);

    *ptr++ = '\n';

    *ptr = '\0';
}

/**
 * @brief  Converts an unsigned integer to a null-terminated string in decimal (base 10) format.
 *
 * @param[in]  value   The unsigned integer to be converted.
 * @param[out] buffer  The buffer where the resulting string will be stored.
 *
 * @return  The length of the resulting string.
 */
static uint8_t UIntToString(uint32_t value, uint8_t *buffer)
{
    uint8_t temp[MAX_VALUE_STR];  /* Temporary buffer to store digits in reverse order.    */
    uint8_t digitIndex = 0u;      /* Index for storing digits in the temporary buffer.     */
    uint8_t reverseIndex = 0u;    /* Index for reversing the digits into the final buffer. */

    /* Handle the special case where the value is 0. */
    if (value == 0u)
    {
        buffer[digitIndex++] = '0'; /* Add '0' to the buffer. */
        buffer[digitIndex] = '\0';  /* Null-terminate the string. */
        reverseIndex = digitIndex;
    }
    else
    {
        /* Extract digits from the number and store them in reverse order in the temp buffer. */
        while (value > 0u)
        {
            temp[digitIndex++] = (value % DECIMAL_BASE) + '0'; /* Get the last digit and convert to a character. */
            value /= DECIMAL_BASE;                             /* Remove the last digit from the number. */
        }

        /* Reverse the digits from the temp buffer into the final buffer. */
        for (reverseIndex = 0u; reverseIndex < digitIndex; reverseIndex++)
        {
            buffer[reverseIndex] = temp[digitIndex - 1u - reverseIndex];
        }
        buffer[reverseIndex] = '\0'; /* Null-terminate the string. */
    }

    return reverseIndex; /* Return the length of the resulting string. */
}
