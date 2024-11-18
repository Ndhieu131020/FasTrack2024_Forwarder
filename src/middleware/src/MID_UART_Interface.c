#include "DRV_S32K144_PORT.h"
#include "DRV_S32K144_LPUART.h"
#include "MID_UART_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define USR_LPUART_INS         ((uint8_t)LPUART1)
#define STANDARD_BAUDRATE      (115200u)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void MID_UART_Init(void)
{
    virtual_pin_id_t LPUART_Tx_Pin = PTC7;
    virtual_pin_id_t LPUART_Rx_Pin = PTC6;

    const PortConfig_t  PortConfigLPUART =
    {
        .Mux         =  PORT_MUX_ALT2,                    /* Configure the pin mux mode as LPUART       */
        .Interrupt   =  PORT_INT_DISABLED,                /* Disable interrupt for the pin              */
        .Pull        =  PORT_INTERNAL_PULL_NOT_ENABLED    /* Disable internal pull resistor for the pin */
    };
    DRV_PORT_Init(GET_PORT((uint8_t)LPUART_Tx_Pin), GET_PIN_NUM((uint8_t)LPUART_Tx_Pin), &PortConfigLPUART);
    DRV_PORT_Init(GET_PORT((uint8_t)LPUART_Rx_Pin), GET_PIN_NUM((uint8_t)LPUART_Rx_Pin), &PortConfigLPUART);

    /* LPUART Configuration */
    const lpuart_config_t lpuartConfig =
    {
        .baudRate                 =  STANDARD_BAUDRATE,            /* Set standard baud rate is 115200         */
        .numberDataBits           =  LPUART_8_BITS_PER_CHAR,       /* Set number of data bits per character    */
        .parityMode               =  LPUART_PARITY_DISABLED,       /* Disable parity bit                       */
        .bitOrder                 =  LPUART_LSB_FIRST,             /* Set least significant bit first          */
        .stopBit                  =  LPUART_ONE_STOP_BIT,          /* Set one stop bit                         */
        .transmitDataInverted     =  false,                        /* Do not invert transmitted data           */
        .receiveDataInverted      =  false,                        /* Do not invert received data              */
        .enableTransmitInterrupt  =  false,                        /* Disable transmit interrupt               */
        .enableReceiveInterrupt   =  true                          /* Enable receive interrupt                 */
    };
    DRV_LPUART_Init(USR_LPUART_INS, &lpuartConfig);
}

void MID_UART_RegisterNotificationCallback(void (*TxCallback)(void), void (*RxCallback)(void))
{
    DRV_LPUART_RegisterIntCallback(USR_LPUART_INS, TxCallback, RxCallback);
}

uint8_t MID_UART_ReceiveData(void)
{
    return DRV_LPUART_ReceiveChar(USR_LPUART_INS);
}

void MID_UART_SendData(uint8_t data)
{
    DRV_LPUART_SendChar(USR_LPUART_INS, data);
}

void MID_UART_SetTxInterrupt(bool enable)
{
    DRV_LPUART_SetTransmitITStatus(USR_LPUART_INS, enable);
}
