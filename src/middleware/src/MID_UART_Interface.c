/*
 *  Filename: MID_UART_Interface.c
 *
 *  Created on: 11-16-2024
 *      Author: Ndhieu131020@gmail.com
*/
#include "MID_UART_Interface.h"
#include "DRV_S32K144_PORT.h"
#include "DRV_S32K144_MCU.h"
#include "DRV_S32K144_LPUART.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define UART_USER_BAUDRATE   115200

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void LPUART_PinConfig(void);
static void LPUART_ParamConfig(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void LPUART_PinConfig(void)
{
    virtual_pin_id_t LPUART_Tx_Pin = PTC7;
    virtual_pin_id_t LPUART_Rx_Pin = PTC6;

    const PortConfig_t  PortConfigLPUART =
    {
        .Mux         =  PORT_MUX_ALT2,                    /* Configure the pin mux mode as LPUART       */
        .Interrupt   =  PORT_INT_DISABLED,                /* Disable interrupt for the pin              */
        .Pull        =  PORT_INTERNAL_PULL_NOT_ENABLED    /* Disable internal pull resistor for the pin */
    };
    DRV_PORT_Init((uint8_t)GET_PORT(LPUART_Tx_Pin), (uint8_t)GET_PIN_NUM(LPUART_Tx_Pin), &PortConfigLPUART);
    DRV_PORT_Init((uint8_t)GET_PORT(LPUART_Rx_Pin), (uint8_t)GET_PIN_NUM(LPUART_Rx_Pin), &PortConfigLPUART);
}

static void LPUART_ParamConfig(void)
{
    lpuart_config_t LPUART_InitStructure;
    uint32_t Uart_Clk_Freq = 0u;

    LPUART_InitStructure.baudRate = 115200;
    LPUART_InitStructure.numberDataBits = LPUART_8_BITS_PER_CHAR;
    LPUART_InitStructure.parityMode = LPUART_PARITY_DISABLED;
    LPUART_InitStructure.bitOrder = LPUART_LSB_FIRST;
    LPUART_InitStructure.stopBit = LPUART_ONE_STOP_BIT;
    LPUART_InitStructure.transmitDataInverted = false;
    LPUART_InitStructure.receiveDataInverted = false;
    LPUART_InitStructure.enableTransmitInterrupt = true;
    LPUART_InitStructure.enableReceiveInterrupt = true;

    DRV_Clock_GetFrequency(LPUART1_CLK, &Uart_Clk_Freq);

    if (Uart_Clk_Freq != 0u)
    {
       DRV_LPUART_Init(USR_INSTANCE, &LPUART_InitStructure, Uart_Clk_Freq);
    }
    else
    {
        /* Error */
    }
}

void MID_UART_Init(void)
{
    LPUART_PinConfig();
    LPUART_ParamConfig();
}

void MID_UART_RegisterNotificationCallback(void (*cb_ptr)(void))
{
    DRV_LPUART_RegisterIntCallback(USR_INSTANCE, cb_ptr);
}

uint8_t MID_UART_ReceiveData(void)
{
    return DRV_LPUART_ReceiveChar(USR_INSTANCE);
}

void MID_UART_SendData(uint8_t Data)
{
    DRV_LPUART_SendChar(USR_INSTANCE, Data);
}

uint8_t MID_UART_GetCommingMessageEvent(void)
{
    return DRV_GetReceiveITStatus(USR_INSTANCE);
}

uint8_t MID_UART_GetSendingMessageEvent(void)
{
    return DRV_GetTransmitITStatus(USR_INSTANCE);
}