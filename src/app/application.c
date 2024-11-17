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
#include "MID_ReceiveQueue_Interface.h"
#include "MID_TransmitQueue_Interface.h"

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
uint8_t data = 0u;
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
        MID_UART_SendData('A');
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
    if(MID_UART_GetCommingMessageEvent())
    {
        data = MID_UART_ReceiveData();
    }

    if(MID_UART_GetSendingMessageEvent())
    {
        MID_UART_SendData('B');
    }
}
