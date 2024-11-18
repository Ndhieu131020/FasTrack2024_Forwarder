#ifndef MID_UART_INTERFACE_H_
#define MID_UART_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
void MID_UART_Init(void);

void MID_UART_RegisterNotificationCallback(void (*TxCallback)(void), void (*RxCallback)(void));

uint8_t MID_UART_ReceiveData(void);

void MID_UART_SendData(uint8_t data);

void MID_UART_SetTxInterrupt(bool enable);

#endif /* MID_UART_INTERFACE_H_ */