#include "DRV_S32K144_NVIC.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

#define NVIC_IRQS_PER_REGISTER    (32u)  /* Number of IRQs per NVIC register */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Enable interrupt request
  * @param  IRQn: IRQ type
  * @retval None
  */
void NVIC_EnableIRQ(IRQn_Type IRQn)
{
    int8_t IRQ_Register = IRQn / NVIC_IRQS_PER_REGISTER;
    int8_t IRQ_Bit      = IRQn % NVIC_IRQS_PER_REGISTER;

    /* Enable NVIC correspond IRQn_Type */
    NVIC->ISER[IRQ_Register] |= (uint32_t)(1<<IRQ_Bit);
}

/**
  * @brief  Disable interrupt request
  * @param  IRQn: IRQ type
  * @retval None
  */
void NVIC_DisbleIRQ(IRQn_Type IRQn)
{
    int8_t IRQ_Register = IRQn / NVIC_IRQS_PER_REGISTER;
    int8_t IRQ_Bit      = IRQn % NVIC_IRQS_PER_REGISTER;

    /* Disable NVIC correspond IRQn_Type */
    NVIC->ISER[IRQ_Register] &= ~((uint32_t)(1<<IRQ_Bit));
}

/**
  * @brief  Set the pending status of interrupt
  * @param  IRQn: IRQ type
  * @retval None
  */
void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
    int8_t IRQ_Register = IRQn / NVIC_IRQS_PER_REGISTER;
    int8_t IRQ_Bit      = IRQn % NVIC_IRQS_PER_REGISTER;

    /* Sets the pending status of interrupt to 1  */
    NVIC->ISPR[IRQ_Register] |= (uint32_t)(1<<IRQ_Bit);
}

/**
  * @brief  Clear the pending status of interrupt
  * @param  IRQn: IRQ type
  * @retval None
  */
void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
    int8_t IRQ_Register = IRQn / NVIC_IRQS_PER_REGISTER;
    int8_t IRQ_Bit      = IRQn % NVIC_IRQS_PER_REGISTER;

    /* Clears the pending status of interrupt to 0 */
    NVIC->ICPR[IRQ_Register] |= (uint32_t)(1<<IRQ_Bit);
}

/**
  * @brief  Get the pending status of interrupt
  * @param  IRQn: IRQ type
  * @retval Set(1) or Clear(0)
  */
uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
    uint32_t retVal     = 0u;
    int8_t IRQ_Register = IRQn / NVIC_IRQS_PER_REGISTER;
    int8_t IRQ_Bit      = IRQn % NVIC_IRQS_PER_REGISTER;

    retVal = (NVIC->ISPR[IRQ_Register] >> IRQ_Bit) & 1u;

    return retVal;
}

/**
  * @brief  Set the priority of interrupt
  * @param  IRQn: IRQ type
  * @param  priority: IRQ priority number
  * @retval None
  */
void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    NVIC->IP[IRQn] = (uint8_t)priority;
}

/**
  * @brief  Get the priority of interrupt
  * @param  IRQn: IRQ type
  * @retval Priority number
  */
uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{
    uint8_t retVal = 0u;

    retVal = NVIC->IP[IRQn];

    return (uint32_t)retVal;
}
