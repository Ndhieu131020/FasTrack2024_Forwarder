/*
 *  Filename: application.c
 *
 *  Created on: 11-04-2024
 *      Author: Ndhieu131020@gmail.com
*/

#include <stdint.h>
#include "MID_Clock_Interface.h"
#include "MID_Notification_Manager.h"
#include "MID_Timer_Interface.h"
#include "MID_CAN_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void App_ReceiveMessageNotification(void);
void App_BusOffNotification(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    MID_Clock_Init();
    MID_CAN_Init();
    MID_Timer_Init();

    MID_CAN_RegisterRxNotificationCallback(&App_ReceiveMessageNotification);
    MID_CAN_RegisterBusOffNotificationCallback(&App_BusOffNotification);

    MID_EnableNotification();

    MID_Timer_StartTimer();

    while(1)
    {

    }

    return 0;
}

void App_ReceiveMessageNotification(void)
{
    
}

void App_BusOffNotification(void)
{

}
