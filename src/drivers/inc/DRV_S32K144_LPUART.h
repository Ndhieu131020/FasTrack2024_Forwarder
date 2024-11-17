#ifndef DRV_S32K144_LPUART_H_
#define DRV_S32K144_LPUART_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdbool.h>
#include <stddef.h>
#include "S32K144.h"
#include "S32K144_features.h"
#include "common_typedef.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Enum: LPUART instance */
typedef enum
{
    LPUART0 = 0U,    /* Low Power Universal Asynchronous Receiver/Transmitter 0  */
    LPUART1 = 1U,    /* Low Power Universal Asynchronous Receiver/Transmitter 1  */
    LPUART2 = 2U     /* Low Power Universal Asynchronous Receiver/Transmitter 2  */
} lpuart_instance;

/* Enum: number of bits in a character */
typedef enum
{
    LPUART_7_BITS_PER_CHAR,    /* 7-bit data characters  */
    LPUART_8_BITS_PER_CHAR,    /* 8-bit data characters  */
    LPUART_9_BITS_PER_CHAR,    /* 9-bit data characters  */
    LPUART_10_BITS_PER_CHAR    /* 10-bit data characters */
} lpuart_data_bits_t;

/* Enum: parity mode */
typedef enum
{
    LPUART_PARITY_DISABLED,    /* parity disabled           */
    LPUART_PARITY_EVEN,        /* parity enabled, type even */
    LPUART_PARITY_ODD          /* parity enabled, type odd  */
} lpuart_parity_mode_t;

/* Enum: bit order */
typedef enum
{
    LPUART_LSB_FIRST = 0U,     /* Least Significant Bit First */
    LPUART_MSB_FIRST = 1U      /* Most Significant Bit First  */
} lpuart_bit_order_t;

/* Enum: number of stop bits */
typedef enum
{
    LPUART_ONE_STOP_BIT = 0U,  /* one stop bit  */
    LPUART_TWO_STOP_BIT = 1U   /* two stop bits */
} lpuart_stop_bit_t;

/* Struct: LPUART configuration */
typedef struct
{
    uint32_t               baudRate;                       /* LPUART baud rate                                              */
    lpuart_data_bits_t     numberDataBits;                 /* Number of bits in a character (7, 8-default, 9 or 10)         */
    lpuart_parity_mode_t   parityMode;                     /* Parity mode, disabled (default), even, odd                    */
    lpuart_bit_order_t     bitOrder;                       /* LSB first (default) or MSB first                              */
    lpuart_stop_bit_t      stopBit;                        /* Number of stop bits, 1 stop bit (default) or 2 stop bits      */
    bool                   transmitDataInverted;           /* Transmitted data is either non-inverted (default) or inverted */
    bool                   receiveDataInverted;            /* Received data is either non-inverted (default) or inverted    */
    bool                   enableTransmitInterrupt;        /* Enable or disable transmit interrupt                          */
    bool                   enableReceiveInterrupt;         /* Enable or disable receive interrupt                           */
} lpuart_config_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/**
  * @brief: Initialize the LPUART module
  * @param [in] LPUART_Ins:    The LPUART instance
  * @param [in] pConfig:       Pointer to the LPUART configuration structure
  * @param [in] lpuartClkFreq: The clock frequency of the LPUART module
  * @return: None
  */
void DRV_LPUART_Init(const uint8_t instance, const lpuart_config_t * pConfig);

/**
  * @brief: Deinitialize the LPUART module
  * @param [in] LPUART_Ins: The LPUART instance
  * @return: None
  */
void DRV_LPUART_DeInit(const uint8_t instance);

/**
  * @brief  Disable transmitter & receiver
  * @param [in] instance: The LPUART instance to use for transmission
  * @retval None
  */
void DRV_LPUART_DisableTransmit(const uint8_t instance);

/**
  * @brief  Enable transmitter & receiver
  * @param [in] instance: The LPUART instance to use for transmission
  * @retval None
  */
void DRV_LPUART_EnableTransmit(const uint8_t instance);

/**
  * @brief: Transmit a single character via LPUART
  * @param [in] LPUART_Ins: The LPUART instance to use for transmission
  * @param [in] send:       The character to transmit
  * @return: None
  */
void DRV_LPUART_SendChar(const uint8_t instance, uint8_t Data);

/**
  * @brief: Transmit a single character via LPUART
  * @param[in] LPUART_Ins: The LPUART instance to use for transmission
  * @param[in] Data: Pointer to data string
  * @param[in] d_length: the length of Data string
  * @return: None
  */
void DRV_LPUART_SendString(const uint8_t instance, uint8_t *Data, uint8_t d_length);

/**
  * @brief  Receive a chacacter from LPUART
  * @param[in] LPUART_Ins: The LPUART instance to use for transmission
  * @retval Received data
  */
uint8_t DRV_LPUART_ReceiveChar(const uint8_t instance);

/**
  * @brief  Control transmit interrupt
  * @param[in] instance: The LPUART instance to use for transmission
  * @param[in] enable:   Enable or disable transmit interrupt
  * @retval None
  */
void DRV_LPUART_SetTransmitITStatus(const uint8_t instance, bool enable);

/**
  * @brief  Get empty state of transmit data register (Transmit Interrupt)
  * @param[in] instance: The LPUART instance to use for transmission
  * @retval IT status (0: DISABLE 1: ENABLE)
  */
uint8_t DRV_GetTransmitITStatus(const uint8_t instance);

/**
  * @brief  Get full state of receive data register (Transmit Interrupt)
  * @param[in] instance: The LPUART instance to use for transmission
  * @retval IT status (0: DISABLE 1: ENABLE)
  */
uint8_t DRV_GetReceiveITStatus(const uint8_t instance);

/**
  * @brief  Registers an interrupt callback function for the specified LPUART peripheral.
  * @param[in] instance: The LPUART instance to use for transmission
  * @param[in] fp:       The function pointer to the callback function that will handle the interrupt.
  * @retval None
  */
void DRV_LPUART_RegisterIntCallback(const uint8_t instance, IRQ_FuncCallback fp);

#endif /* DRV_S32K144_LPUART_H_ */

/*******************************************************************************
 * End Of File
 ******************************************************************************/
