/*
 *  Filename: MID_Notification_Manager.c
 *
 *  Created on: 11-05-2024
 *      Author: Ndhieu131020@gmail.com
*/

#include "DRV_S32K144_NVIC.h"
#include "MID_Notification_Manager.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void MID_EnableNotification(void)
{
    NVIC_EnableIRQ(LPUART1_RxTx_IRQn);
    NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
    NVIC_EnableIRQ(CAN0_ORed_IRQn);
    NVIC_EnableIRQ(CAN0_ORed_0_15_MB_IRQn);
}
