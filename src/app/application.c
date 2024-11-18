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
#include "App_DataProcessing.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MSG_LENGTH_MAX    9u
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

/* Array stores UART frame from PC Tool */
uint8_t Receive_Data_Str[MSG_LENGTH_MAX] = {0};
uint8_t Receive_Data_Idx                 = 0u;

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    MID_Clock_Init();
    MID_CAN_Init();
//    MID_Timer_Init();
    MID_UART_Init();
    MID_Receive_Queue_Init();

    MID_CAN_RegisterRxNotificationCallback(App_CANReceiveNotification);
    MID_CAN_RegisterBusOffNotificationCallback(App_CANBusOffNotification);
    MID_UART_RegisterNotificationCallback(&App_UART_TxNotification, &App_UART_RxNotification);

    MID_EnableNotification();
//    MID_Timer_StartTimer();

    while(1)
    {

    }

    return 0;
}

static void App_CANReceiveNotification(void)
{
    /* Structure to save received CAN data */
    Data_Typedef CAN_Data_Receive = {0u};

    /* Struture contain Data was converted from CAN Message to be used as param input of MID_Receive_EnQueue()*/
    ReceiveFrame_t l_Data_Receive   = {0U};

   if(MID_CheckCommingMessageEvent(RX_DISTANCE_DATA_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_DISTANCE_DATA_MB, &CAN_Data_Receive);

        l_Data_Receive.ID = CAN_Data_Receive.ID;
        l_Data_Receive.Data = CAN_Data_Receive.Data;

        MID_ClearMessageCommingEvent(RX_DISTANCE_DATA_MB);
        /* Push distance data into queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);
   }

   if(MID_CheckCommingMessageEvent(RX_ROTATION_DATA_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_ROTATION_DATA_MB, &CAN_Data_Receive);

        l_Data_Receive.ID = CAN_Data_Receive.ID;
        l_Data_Receive.Data = CAN_Data_Receive.Data;

        MID_ClearMessageCommingEvent(RX_ROTATION_DATA_MB);
        /* Push rotation data into queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);
   }

      if(MID_CheckCommingMessageEvent(RX_CONFIRM_FROM_DISTANCE_NODE_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_CONFIRM_FROM_DISTANCE_NODE_MB, &CAN_Data_Receive);

        l_Data_Receive.ID = CAN_Data_Receive.ID;
        l_Data_Receive.Data = CAN_Data_Receive.Data;

        MID_ClearMessageCommingEvent(RX_CONFIRM_FROM_DISTANCE_NODE_MB);
        /* Push distance confirm data into queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);
   }

   if(MID_CheckCommingMessageEvent(RX_CONFIRM_FROM_ROTATION_NODE_MB) == CAN_MSG_RECEIVED)
   {
        MID_CAN_ReceiveMessage(RX_CONFIRM_FROM_ROTATION_NODE_MB, &CAN_Data_Receive);

        l_Data_Receive.ID = CAN_Data_Receive.ID;
        l_Data_Receive.Data = CAN_Data_Receive.Data;

        MID_ClearMessageCommingEvent(RX_CONFIRM_FROM_ROTATION_NODE_MB);
        /* Push rotation confirm data into queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);
   }
}

static void App_CANBusOffNotification(void)
{

}

static void App_UART_TxNotification(void)
{

}

static void App_UART_RxNotification(void)
{
    ReceiveFrame_t l_Data_Receive  = {0U};
    uint8_t l_UART_Received_Data    = 0u;

    /* Get 1 byte message from UART*/
    l_UART_Received_Data = MID_UART_ReceiveData();

    /* Check if end of frame (\n character) */
    if(l_UART_Received_Data != '\n')
    {
        Receive_Data_Str[Receive_Data_Idx] = l_UART_Received_Data;
        Receive_Data_Idx++;
    }
    else
    {
        /* Convert string to number */
        App_Parser_UARTFrame(Receive_Data_Str, Receive_Data_Idx-1, &l_Data_Receive);

        /* Push to receive Queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);

        /* Clear data index */
        Receive_Data_Idx = 0u;
    }
}
