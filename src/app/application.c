/*
 *  Filename: application.c
 *
 *  Created on: 11-04-2024
 *      Author: Ndhieu131020@gmail.com
*/

#include <stdint.h>
#include <stdbool.h>
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
static void App_UART_TxNotification(void);
static void App_UART_RxNotification(void);

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
    MID_UART_RegisterNotificationCallback(&App_UART_TxNotification, &App_UART_RxNotification);

    MID_EnableNotification();
    MID_Timer_StartTimer();

    while(1)
    {
        // MID_UART_SendData('A');
        // MID_UART_SetTxInterrupt(false);
    }

    return 0;
}

static void App_CANReceiveNotification(void)
{
    ReceiveFrame_t        Data_Receive = {0U};
    QueueCheckOperation_t status       = QUEUE_DONE_FAILED;

   if(MID_CheckCommingMessageEvent(RX_DISTANCE_DATA_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_DISTANCE_DATA_MB, &Data_Receive);
        MID_ClearMessageCommingEvent(RX_DISTANCE_DATA_MB);
        /* Push distance data into queue */
        status = MID_Receive_EnQueue(&Data_Receive);
   }

   if(MID_CheckCommingMessageEvent(RX_ROTATION_DATA_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_ROTATION_DATA_MB, &Data_Receive);
        MID_ClearMessageCommingEvent(RX_ROTATION_DATA_MB);
        /* Push rotation data into queue */
        status = MID_Receive_EnQueue(&Data_Receive);
   }

      if(MID_CheckCommingMessageEvent(RX_CONFIRM_FROM_DISTANCE_NODE_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_CONFIRM_FROM_DISTANCE_NODE_MB, &Data_Receive);
        MID_ClearMessageCommingEvent(RX_CONFIRM_FROM_DISTANCE_NODE_MB);
        /* Push distance confirm data into queue */
        status = MID_Receive_EnQueue(&Data_Receive);
   }

   if(MID_CheckCommingMessageEvent(RX_CONFIRM_FROM_ROTATION_NODE_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_CONFIRM_FROM_ROTATION_NODE_MB, &Data_Receive);
        MID_ClearMessageCommingEvent(RX_CONFIRM_FROM_ROTATION_NODE_MB);
        /* Push rotation confirm data into queue */
        status = MID_Receive_EnQueue(&Data_Receive);
   }
}

static void App_CANBusOffNotification(void)
{

}

static void App_UART_TxNotification(void)
{
    MID_UART_SendData("DuongTDL2", 9u);
}

static void App_UART_RxNotification(void)
{
    data = MID_UART_ReceiveData();
}
