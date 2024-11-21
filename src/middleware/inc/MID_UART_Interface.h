#ifndef MID_UART_INTERFACE_H_
#define MID_UART_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/

/*******************************************************************************
 * Definition
 ******************************************************************************/
/** @defgroup Check Connection Message ID
  * @{
  */
#define PC_CONNECT_FORWARDER_ID         0xA0
#define PC_CONNECT_DISTANCE_SENSOR_ID   0xA1
#define PC_CONNECT_ROTATION_SENSOR_ID   0xA2

#define REQUEST_CONNECTION_DATA         0x10
#define CONFIRM_CONNECTION_DATA         0xFF

/** @defgroup Data Message ID
  * @{
  */
#define ROTATION_DATA_ID   0xC0
#define DISTANCE_DATA_ID   0xD0

#define CONFIRM_SENSOR_DATA      0xFFFF
#define SENSOR_DISCONNECT_DATA   0xFFFF

/*******************************************************************************
 * API
 ******************************************************************************/
void MID_UART_Init(void);

void MID_UART_RegisterNotificationCallback(void (*TxCallback)(void), void (*RxCallback)(void));

uint8_t MID_UART_ReceiveData(void);

void MID_UART_SendData(uint8_t data);

void MID_UART_SetTxInterrupt(bool enable);

#endif /* MID_UART_INTERFACE_H_ */