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
#include "MID_UART_Interface.h"
#include "DRV_S32K144_LPUART.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void App_CANReceiveNotification(void);
static void App_CANBusOffNotification(void);
static void App_UARTNotification(void);

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
    MID_UART_Init();

    MID_CAN_RegisterRxNotificationCallback(App_CANReceiveNotification);
    MID_CAN_RegisterBusOffNotificationCallback(App_CANBusOffNotification);
    MID_UART_RegisterNotificationCallback(&App_UARTNotification);

    MID_EnableNotification();
    MID_Timer_StartTimer();

    while(1)
    {
        DRV_LPUART_SendChar(1, 'A');
    }

    return 0;
}

static void App_CANReceiveNotification(void)
{

}

static void App_CANBusOffNotification(void)
{

}

static void App_UARTNotification(void)
{
    if()
    DRV_LPUART_SendChar(1, 'B');
}
