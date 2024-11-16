/*
 *  Filename: MID_UART_Interface.h
 *
 *  Created on: 11-16-2024
 *      Author: Ndhieu131020@gmail.com
*/

#ifndef MID_UART_INTERFACE_H_
#define MID_UART_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define USR_INSTANCE    1u
/*******************************************************************************
 * API
 ******************************************************************************/
void MID_UART_Init(void);

void MID_UART_RegisterNotificationCallback(void (*cb_ptr)(void));

uint8_t MID_UART_ReceiveData(void);

void MID_UART_SendData(uint8_t Data);

uint8_t MID_UART_GetCommingMessageEvent(void);

uint8_t MID_UART_GetSendingMessageEvent(void);

#endif /* MID_UART_INTERFACE_H_ */