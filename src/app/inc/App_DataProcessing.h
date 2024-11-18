/*
 *  Filename: App_DataProcessing.h
 *
 *  Created on: 11-18-2024
 *      Author: Ndhieu131020@gmail.com
*/

#ifndef APP_DATAPROCESSING_H_
#define APP_DATAPROCESSING_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include <stdint.h>
#include "Queue_Common.h"
#include <stdbool.h>

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
  * @brief  Function to parse the string and extract 2 unsigned integers
  * @param[in]  str   Pointer to the input string to be parsed.
  * @param[in]  length Length of the input string (excluding any null terminator).
  * @param[out] Output  Pointer to store Message frame struct (contain ID and Data).
  * @return true if the parsing is successful and both numbers are extracted, false if the input format is invalid.
  */
bool App_Parser_UARTFrame(const uint8_t* str, int length, ReceiveFrame_t *Output);

/**
  * @brief  Function to convert ID and Data to a UART string that can be printed out
  * @param[in]  id       ID of the frame
  * @param[in]  data     Data of the frame
  * @param[out] outputBuffer  Pointer to store the output string
  */
void APP_Compose_UARTFrame(uint32_t id, uint32_t data, uint8_t *outputBuffer);

#endif /* APP_DATAPROCESSING_H_ */
