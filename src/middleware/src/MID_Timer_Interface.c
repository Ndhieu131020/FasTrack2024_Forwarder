/*
 *  Filename: MID_Timer_Interface.c
 *
 *  Created on: 11-05-2024
 *      Author: Ndhieu131020@gmail.com
*/

#include "DRV_S32K144_LPIT.h"
#include "DRV_S32K144_MCU.h"
#include "MID_Timer_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Timer_Notification(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
uint32_t LPIT_Freq = 0u;

/*******************************************************************************
 * Code
 ******************************************************************************/
void MID_Timer_Init(void)
{
    LPIT_InitTypedef LPIT_InitStructure;

    DRV_LPIT_EnableModule(LPIT_INSTANCE);

    DRV_LPIT_StopTimerChannel(LPIT_INSTANCE, LPIT_CH0);

    LPIT_InitStructure.LPIT_ChainChannel = DISABLE;
    LPIT_InitStructure.LPIT_OperationMode = Periodic_Cnt_32b;
    LPIT_InitStructure.LPIT_Interupt = ENABLE;

    DRV_LPIT_Init(LPIT_INSTANCE, LPIT_CH0, &LPIT_InitStructure);

    DRV_Clock_GetFrequency(LPIT0_CLK, &LPIT_Freq);

    DRV_LPIT_SetReloadValue(LPIT_INSTANCE, LPIT_CH0, 400);

}

void MID_Timer_RegisterNotificationCallback(void (*cb_ptr)(void))
{
    DRV_LPIT0_RegisterIntCallback(LPIT_CH0, cb_ptr);
}

void MID_Timer_StartTimer(void)
{
    DRV_LPIT_StartTimerChannel(LPIT_INSTANCE, LPIT_CH0);
}

void MID_Timer_StopTimer(void)
{
    DRV_LPIT_StopTimerChannel(LPIT_INSTANCE, LPIT_CH0);
}
