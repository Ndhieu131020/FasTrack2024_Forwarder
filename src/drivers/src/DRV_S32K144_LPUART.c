/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "DRV_S32K144_LPUART.h"
#include "DRV_S32K144_MCU.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Calculate the absolute difference between two values */
#define ABS(a,b)                     ((a < b) ? (b - a) : (a - b))

/* Minimum value for the oversampling ratio (OSR) */
#define OSR_MIN                      (4U)

/* Maximum value for the oversampling ratio (OSR) */
#define OSR_MAX                      (32U)

/* Minimum value for the baud rate divisor (SBR) */
#define SBR_MIN                      (1U)

/* Oversampling ratio of 7 for UART communication */
#define OVERSAMPLING_RATIO_OF_7      (6U)

/* Structure to hold the two callback functions for Rx and Tx interrupts */
typedef struct
{
    IRQ_FuncCallback rxCallback;  /* Callback for Rx interrupt */
    IRQ_FuncCallback txCallback;  /* Callback for Tx interrupt */
} LPUART_IRQ_Callbacks;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void DRV_LPUART_SetBitPerChar(LPUART_Type *base, const lpuart_data_bits_t numberDataBits);
static void DRV_LPUART_SetParityMode(LPUART_Type *base, const lpuart_parity_mode_t parityMode);
static void DRV_LPUART_setTransmitDataInversion(LPUART_Type *base, const bool isTransmitDataInverted);
static void DRV_LPUART_setReceiveDataInversion(LPUART_Type *base, const bool isreceiveDataInverted);
static void DRV_LPUART_setBitOrder(LPUART_Type *base, const lpuart_bit_order_t bitOrder);
static void DRV_LPUART_setStopBit(LPUART_Type *base, const lpuart_stop_bit_t stopBit);
static void DRV_LPUART_SetBaudRate(LPUART_Type *base, const uint32_t baudRate, const uint32_t lpuartClkFreq);
static void DRV_LPUART_SetRxTxInterrupt(LPUART_Type *base, const bool isTransmitInterruptEnabled, \
                                    const bool isReceiveInterruptEnabled);
static void DRV_LPUART_IRQHandler(uint8_t instance);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array LPUART address base */
static LPUART_Type * const g_LPUARTBase[LPUART_INSTANCE_COUNT] = IP_LPUART_BASE_PTRS;

/* Array LPUART clock names */
static const clock_names_t g_LPUARTClkNames[LPUART_INSTANCE_COUNT] = LPUART_CLOCK_NAMES;

static LPUART_IRQ_Callbacks LPUART_IRQ_CallbacksArray[LPUART_INSTANCE_COUNT] = { {NULL, NULL} };

/*******************************************************************************
 * Codes
 ******************************************************************************/

/**
  * @brief: Initialize the LPUART module
  * @param [in] LPUART_Ins:    The LPUART instance
  * @param [in] pConfig:       Pointer to the LPUART configuration structure
  * @param [in] lpuartClkFreq: The clock frequency of the LPUART module
  * @return: None
  */
void DRV_LPUART_Init(const uint8_t instance, const lpuart_config_t * pConfig)
{
    uint32_t      lpuartClkFreq = 0U;
    LPUART_Type * base          = g_LPUARTBase[instance];
    clock_names_t lpuartClkName = g_LPUARTClkNames[instance];

    DRV_Clock_GetFrequency(lpuartClkName, &lpuartClkFreq);

    if (lpuartClkFreq != 0U)
    {
        /* Disable TX, RX to configure the LPUART module */
        base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

        /* Set the number of bits per character for LPUART communication */
        DRV_LPUART_SetBitPerChar(base, pConfig->numberDataBits);

        /* Set the parity mode for LPUART communication */
        DRV_LPUART_SetParityMode(base, pConfig->parityMode);

        /* Configure whether the transmit data is inverted or not */
        DRV_LPUART_setTransmitDataInversion(base, pConfig->transmitDataInverted);

        /* Configure whether the receive data is inverted or not */
        DRV_LPUART_setReceiveDataInversion(base, pConfig->receiveDataInverted);

        /* Set the bit order for LPUART communication */
        DRV_LPUART_setBitOrder(base, pConfig->bitOrder);

        /* Set the number of stop bits for LPUART communication */
        DRV_LPUART_setStopBit(base, pConfig->stopBit);

        /* Configure the LPUART baud rate */
        DRV_LPUART_SetBaudRate(base, pConfig->baudRate, lpuartClkFreq);

        /* Configure the LPUART transmit and receive interrupts */
        DRV_LPUART_SetRxTxInterrupt(base, pConfig->enableTransmitInterrupt,
                                            pConfig->enableReceiveInterrupt);

        /* Enable TX, RX */
        base->CTRL |= (LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);
    }
    else
    {
        /* Current LPUART instance is clock gated off */
    }
}

/**
  * @brief: Deinitialize the LPUART module
  * @param [in] LPUART_Ins: The LPUART instance
  * @return: None
  */
void DRV_LPUART_DeInit(const uint8_t instance)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];

    /* Disable TX, RX */
    base->CTRL &= ~(LPUART_CTRL_TE_MASK | LPUART_CTRL_RE_MASK);

    /* Check if either TIE or RIE is set in the control register */
    if (base->CTRL & (LPUART_CTRL_TIE_MASK | LPUART_CTRL_RIE_MASK))
    {
        /* If either flag is set, clear both TIE and RIE flags in the control register */
        base->CTRL &= ~(LPUART_CTRL_TIE_MASK | LPUART_CTRL_RIE_MASK);
    }
}

/**
  * @brief: Set the number of bits per character for LPUART communication
  * @param [in] base:           Pointer to the LPUART peripheral base address
  * @param [in] numberDataBits: Number of data bits per character (7, 8, 9, or 10 bits)
  * @return: None
  */
static void DRV_LPUART_SetBitPerChar(LPUART_Type * base, const lpuart_data_bits_t numberDataBits)
{
    switch(numberDataBits)
    {
        case LPUART_7_BITS_PER_CHAR:
        {
            /* Clear M (8-bits), Set M7 (7-bits) */
            base->CTRL = ((base->CTRL & (~LPUART_CTRL_M_MASK)) | LPUART_CTRL_M7_MASK);
            /* Clear M10 (7-bits to 9-bits) */
            base->BAUD &= ~LPUART_BAUD_M10_MASK;

            break;
        }
        case LPUART_8_BITS_PER_CHAR:
        {
            /* Clear M (8-bits), Clear M7 (8-bits to 10-bits) */
            base->CTRL &= ~(LPUART_CTRL_M_MASK | LPUART_CTRL_M7_MASK);
            /* Clear M10 (7-bits to 9-bits) */
            base->BAUD &= ~LPUART_BAUD_M10_MASK;

            break;
        }
        case LPUART_9_BITS_PER_CHAR:
        {
            /* Set M (9-bits), Clear M7 (8-bits to 10-bits) */
            base->CTRL = ((base->CTRL & (~LPUART_CTRL_M7_MASK)) | LPUART_CTRL_M_MASK);
            /* Clear M10 (7-bits to 9-bits) */
            base->BAUD &= ~LPUART_BAUD_M10_MASK;

            break;
        }
        case LPUART_10_BITS_PER_CHAR:
        {
            /* Clear M (8-bits), Clear M7 (8-bits to 10-bits) */
            base->CTRL &= ~(LPUART_CTRL_M_MASK | LPUART_CTRL_M7_MASK);
            /* Set M10 (10-bits) */
            base->BAUD |= LPUART_BAUD_M10_MASK;

            break;
        }
    }
}

/**
  * @brief: Set the parity mode for LPUART communication
  * @param [in] base:         Pointer to the LPUART peripheral base address
  * @param [in] parityMode:   Parity mode to be set (Disabled, Even, or Odd)
  * @return: None
  */
static void DRV_LPUART_SetParityMode(LPUART_Type * base, const lpuart_parity_mode_t parityMode)
{
    switch(parityMode)
    {
        case LPUART_PARITY_DISABLED:
        {
            /* No hardware parity generation or checking */
            base->CTRL &= ~LPUART_CTRL_PE_MASK;

            break;
        }
        case LPUART_PARITY_EVEN:
        {
            /* Parity enabled, even parity */
            base->CTRL = ((base->CTRL & (~LPUART_CTRL_PT_MASK)) | LPUART_CTRL_PE_MASK);

            break;
        }
        case LPUART_PARITY_ODD:
        {
            /* Parity enabled, odd parity */
            base->CTRL |= LPUART_CTRL_PE_MASK | LPUART_CTRL_PT_MASK;

            break;
        }
    }
}

/**
  * @brief: Configure whether the transmit data is inverted or not
  * @param [in] base:                    Pointer to the LPUART peripheral base address
  * @param [in] isTransmitDataInverted:  Boolean value indicating whether transmit data is inverted or not
  * @return: None
  */
static void DRV_LPUART_setTransmitDataInversion(LPUART_Type * base, const bool isTransmitDataInverted)
{
    if(isTransmitDataInverted)
    {
        base->CTRL |= LPUART_CTRL_TXINV_MASK;
    }
    else
    {
        base->CTRL &= ~LPUART_CTRL_TXINV_MASK;
    }
}

/**
  * @brief: Configure whether the receive data is inverted or not
  * @param [in] base:                  Pointer to the LPUART peripheral base address
  * @param [in] isReceiveDataInverted: Boolean value indicating whether receive data is inverted or not
  * @return: None
  */
static void DRV_LPUART_setReceiveDataInversion(LPUART_Type * base, const bool isreceiveDataInverted)
{
    if(isreceiveDataInverted)
    {
        base->STAT |= LPUART_STAT_RXINV_MASK;
    }
    else
    {
        base->STAT &= ~LPUART_STAT_RXINV_MASK;
    }
}

/**
  * @brief: Set the bit order for LPUART communication
  * @param [in] base:      Pointer to the LPUART peripheral base address
  * @param [in] bitOrder:  Bit order to be set (LSB first or MSB first)
  * @return: None
  */
static void DRV_LPUART_setBitOrder(LPUART_Type * base, const lpuart_bit_order_t bitOrder)
{
    base->STAT = (base->STAT & (~LPUART_STAT_MSBF_MASK)) | LPUART_STAT_MSBF(bitOrder);
}

/**
  * @brief: Set the number of stop bits for LPUART communication
  * @param [in] base:     Pointer to the LPUART peripheral base address
  * @param [in] stopBit:  Number of stop bits to be set
  * @return: None
  */
static void DRV_LPUART_setStopBit(LPUART_Type * base, const lpuart_stop_bit_t stopBit)
{
    base->BAUD = (base->BAUD & (~LPUART_BAUD_SBNS_MASK)) | LPUART_BAUD_SBNS(stopBit);
}

/**
  * @brief: Configure the LPUART baud rate
  * @param [in] base:          Pointer to the LPUART peripheral base address
  * @param [in] baudRate:      Desired baud rate
  * @param [in] lpuartClkFreq: Frequency of the LPUART clock source
  * @return: None
  */
static void DRV_LPUART_SetBaudRate(LPUART_Type * base, const uint32_t baudRate, const uint32_t lpuartClkFreq)
{
    uint8_t  OSR_final                 = OSR_MIN;          /* Final value for OSR                                            */
    uint8_t  OSR_tmp                   = OSR_MIN;          /* Temporary value for OSR                                        */
    uint16_t SBR_final                 = SBR_MIN;          /* Final value for SBR                                            */
    uint16_t SBR_tmp                   = SBR_MIN;          /* Temporary value for SBR                                        */
    uint16_t SBR_rounded_up            = SBR_MIN;          /* Rounded-up SBR value for optimized baud rate calculation       */
    uint32_t BaudRate_tmp1             = 0U;               /* Temporary value for calculated baud rate with initial SBR      */
    uint32_t BaudRate_tmp2             = 0U;               /* Temporary value for calculated baud rate with rounded-up SBR   */
    uint32_t BaudRate_difference_final = 0U;               /* Final difference between desired and calculated baud rates     */
    uint32_t BaudRate_difference_tmp   = 0U;               /* Temporary difference between desired and calculated baud rates */
    uint32_t BAUD_Register_tmp         = base->BAUD;       /* Temporary storage for the BAUD register value                  */

    /* Calculate initial SBR */
    SBR_final = (uint16_t)(lpuartClkFreq / (baudRate * OSR_final));

    /* Handle edge case where SBR is 0 */
    if(SBR_final == 0U)
    {
        SBR_final = SBR_MIN;    /* Set SBR to min */
    }
    else
    {
        SBR_rounded_up = SBR_final + 1U;      /* Calculate the rounded-up SBR */
    }

    /* Calculate baudrate in case OSR is at initial value */
    BaudRate_tmp1 = (lpuartClkFreq / (SBR_final * OSR_final));

    /* Calculate baudrate with rounded-up SBR and OSR is at initial value */
    BaudRate_tmp2 = (lpuartClkFreq / (SBR_rounded_up * OSR_final));

    /* Calculate the difference between the baudrate in case the OSR is at the initial value and the desired baudrate */
    BaudRate_difference_final = ABS(BaudRate_tmp1, baudRate);

    /* Check if rounding up SBR provides a closer baudrate to the desired baudrate */
    if (ABS(BaudRate_tmp2, baudRate) < BaudRate_difference_final)
    {
        /* Update SBR_final with rounded-up SBR */
        SBR_final = SBR_rounded_up;
        /* Update the difference with the new baudrate */
        BaudRate_difference_final = ABS(BaudRate_tmp2, baudRate);
    }
    else
    {
        /* Do nothing */
    }

    /* Loop through possible OSR values to find optimal settings */
    for (OSR_tmp = OSR_MIN + 1U; OSR_tmp <= OSR_MAX; OSR_tmp++)
    {
        /* Calculate temporary SBR */
        SBR_tmp = (uint16_t)(lpuartClkFreq / (baudRate * OSR_tmp));

        /* Handle edge case where SBR is 0 */
        if(SBR_tmp == 0U)
        {
            SBR_tmp = SBR_MIN;    /* Set SBR to min */
        }
        else
        {
            SBR_rounded_up = SBR_tmp + 1U;    /* Calculate the rounded-up SBR */
        }

        /* Calculate baudrate with SBR */
        BaudRate_tmp1 = (lpuartClkFreq / (SBR_tmp * OSR_tmp));

        /* Calculate baudrate with rounded-up SBR */
        BaudRate_tmp2 = (lpuartClkFreq / (SBR_rounded_up * OSR_tmp));

        /* Calculate the difference between the baudrate and the desired baudrate */
        BaudRate_difference_tmp = ABS(BaudRate_tmp1, baudRate);

        /* Check if rounding up SBR provides a closer baudrate to the desired baudrate */
        if (ABS(BaudRate_tmp2, baudRate) < BaudRate_difference_tmp)
        {
            /* Update SBR_tmp with rounded-up SBR */
            SBR_tmp = SBR_rounded_up;
            /* Update the difference_tmp with the new baudrate */
            BaudRate_difference_tmp = ABS(BaudRate_tmp2, baudRate);
        }
        else
        {
            /* Do nothing */
        }

        /* Update the final setting if the difference between the newly calculated
           baudrate and the desired baudrate is less than the old difference */
        if (BaudRate_difference_tmp < BaudRate_difference_final)
        {
            BaudRate_difference_final = BaudRate_difference_tmp;
            OSR_final                 = OSR_tmp;
            SBR_final                 = SBR_tmp;
        }
        else
        {
            /* Do nothing */
        }
    }

    /* Check if OSR_final is between oversampling ratio of 4 and oversampling ratio of 7 */
    if (OSR_final <= OVERSAMPLING_RATIO_OF_7)
    {
        /* Set BOTHEDGE bit */
        BAUD_Register_tmp |= LPUART_BAUD_BOTHEDGE_MASK;
    }
    else
    {
        /* Do nothing */
    }

    base->BAUD = (BAUD_Register_tmp & (~(LPUART_BAUD_OSR_MASK            | LPUART_BAUD_SBR_MASK)))
                                    |   (LPUART_BAUD_OSR(OSR_final - 1U) | LPUART_BAUD_SBR(SBR_final));

}

/**
  * @brief: Configure the LPUART transmit and receive interrupts
  * @param [in] base:                         Pointer to the LPUART peripheral base address
  * @param [in] instance:                   Instance of the LPUART peripheral
  * @param [in] isTransmitInterruptEnabled:   Flag to enable or disable transmit interrupts
  * @param [in] isReceiveInterruptEnabled:    Flag to enable or disable receive interrupts
  * @return: None
  */
static void DRV_LPUART_SetRxTxInterrupt(LPUART_Type *base, const bool isTransmitInterruptEnabled,
                                    const bool isReceiveInterruptEnabled)
{
    base->CTRL = (base->CTRL & (~(LPUART_CTRL_TIE_MASK                       | LPUART_CTRL_RIE_MASK)))
                             |   LPUART_CTRL_TIE(isTransmitInterruptEnabled) | LPUART_CTRL_RIE(isReceiveInterruptEnabled);
}

/**
  * @brief  Disable transmitter & receiver
  * @param [in] instance: The LPUART instance to use for communication
  * @retval None
  */
void DRV_LPUART_DisableCommunication(const uint8_t instance)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];

    base->CTRL &= ~LPUART_CTRL_TE_MASK; /* Disable Transmitter */
    base->CTRL &= ~LPUART_CTRL_RE_MASK; /* Disable Receiver */
}

/**
  * @brief  Enable transmitter & receiver
  * @param [in] instance: The LPUART instance to use for communication
  * @retval None
  */
void DRV_LPUART_EnableCommunication(const uint8_t instance)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];

    base->CTRL |= LPUART_CTRL_TE_MASK; /* Disable Transmitter */
    base->CTRL |= LPUART_CTRL_RE_MASK; /* Disable Receiver */
}

/**
  * @brief: Transmit a single character via LPUART
  * @param [in] instance: The LPUART instance to use for communication
  * @param [in] Data:     The character to transmit
  * @return: None
  */
void DRV_LPUART_SendChar(const uint8_t instance, uint8_t Data)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];

    /* Send data */
    base->DATA = Data;
}

/**
  * @brief  Receive a chacacter from LPUART
  * @param[in] instance: The LPUART instance to use for communication
  * @retval Received data
  */
uint8_t DRV_LPUART_ReceiveChar(const uint8_t instance)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];
    uint8_t retVal = 0u;

    if(((base->STAT & LPUART_STAT_RDRF_MASK) >> LPUART_STAT_RDRF_SHIFT) == 1U)
    {
        retVal = base->DATA; /* Read received data */
    }

    return retVal;
}

/**
  * @brief  Control transmit interrupt
  * @param[in] instance: The LPUART instance to use for communication
  * @param[in] enable:   Enable or disable transmit interrupt
  * @retval None
  */
void DRV_LPUART_SetTransmitITStatus(const uint8_t instance, bool enable)
{
    /* Get LPUART base address */
    LPUART_Type *base = g_LPUARTBase[instance];

    if (enable)
    {
        if (!(base->CTRL & LPUART_CTRL_TIE_MASK))
        {
            base->CTRL |= LPUART_CTRL_TIE_MASK;
        }
        else
        {
            /* Do nothing. Interrupt is already enabled */
        }
    }
    else
    {
        if (base->CTRL & LPUART_CTRL_TIE_MASK)
        {
            base->CTRL &= ~LPUART_CTRL_TIE_MASK;
        }
        else
        {
            /* Do nothing. Interrupt is already disabled */
        }
    }
}

/**
  * @brief  Register interrupt callback
  * @param[in] instance: The LPUART instance to use for communication
  * @param[in] Txcallback: Pointer to transmit interrupt callback function
  * @param[in] Rxcallback: Pointer to receive interrupt callback function
  * @retval None
  */
void DRV_LPUART_RegisterIntCallback(uint8_t instance, IRQ_FuncCallback Txcallback, IRQ_FuncCallback Rxcallback)
{
    if (Txcallback != NULL)
    {
        LPUART_IRQ_CallbacksArray[instance].txCallback = Txcallback;
    }

    if (Rxcallback != NULL)
    {
        LPUART_IRQ_CallbacksArray[instance].rxCallback = Rxcallback;
    }
}

/**
  * @brief  LPUART interrupt handler function
  * @param  None
  * @retval None
  */
static void DRV_LPUART_IRQHandler(uint8_t instance)
{
    /* Get LPUART base address */
    LPUART_Type * base = g_LPUARTBase[instance];

    /* Check if the Receive Data Register Full (RDRF) flag is set */
    if ((base->STAT & LPUART_STAT_RDRF_MASK) >> LPUART_STAT_RDRF_SHIFT)
    {
        LPUART_IRQ_CallbacksArray[instance].rxCallback();
    }

    /* Check if the Transmit Data Register Empty (TDRE) flag is set */
    if ((base->STAT & LPUART_STAT_TDRE_MASK) >> LPUART_STAT_TDRE_SHIFT)
    {
        LPUART_IRQ_CallbacksArray[instance].txCallback();
    }
}

/**
  * @brief  LPUART0 interrupt handler function
  * @param  None
  * @retval None
  */
void LPUART0_RxTx_IRQHandler(void)
{
    DRV_LPUART_IRQHandler((uint8_t)LPUART0);
}

/**
  * @brief  LPUART1 interrupt handler function
  * @param  None
  * @retval None
  */
void LPUART1_RxTx_IRQHandler(void)
{
    DRV_LPUART_IRQHandler((uint8_t)LPUART1);
}

/**
  * @brief  LPUART2 interrupt handler function
  * @param  None
  * @retval None
  */
void LPUART2_RxTx_IRQHandler(void)
{
    DRV_LPUART_IRQHandler((uint8_t)LPUART2);
}

/*******************************************************************************
 * End Of File
 ******************************************************************************/
