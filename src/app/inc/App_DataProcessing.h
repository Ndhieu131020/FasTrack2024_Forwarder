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

bool App_Parser_UARTFrame(const uint8_t* str, int length, ReceiveFrame_t *Output);

#endif /* APP_DATAPROCESSING_H_ */