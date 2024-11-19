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

#define MSG_LENGTH_MAX    10u

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void App_CANReceiveNotification(void);
static void App_CANBusOffNotification(void);
static void App_UART_TxNotification(void);
static void App_UART_RxNotification(void);

static void App_Handle_DataFromDistanceSensor(void);
static void App_Handle_DataFromRotationSensor(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array stores UART frame from PC Tool */
uint8_t Receive_Data_Str[MSG_LENGTH_MAX] = {0};
uint8_t Receive_Data_Idx                 = 0u;

/* Array stores UART frame to transmit PC Tool */
uint8_t Transmit_Data_Str[MSG_LENGTH_MAX] = {0};
uint8_t Transmit_Data_Idx                 = 0u;

ReceiveFrame_t Processing_Msg = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;
    MID_Clock_Init();
    MID_CAN_Init();
//    MID_Timer_Init();
    MID_UART_Init();
    MID_Transmit_Queue_Init();
    MID_Receive_Queue_Init();

    MID_CAN_RegisterRxNotificationCallback(App_CANReceiveNotification);
    MID_CAN_RegisterBusOffNotificationCallback(App_CANBusOffNotification);
    MID_UART_RegisterNotificationCallback(&App_UART_TxNotification, &App_UART_RxNotification);

    MID_EnableNotification();
//    MID_Timer_StartTimer();

    while(1)
    {

        status = MID_Receive_DeQueue(&Processing_Msg);

        if(status == QUEUE_DONE_SUCCESS)
        {
            switch (Processing_Msg.ID)
            {
            /* If received data message from Distance sensor node */
            case RX_DISTANCE_DATA_ID:
                /* Send confirm message to Distance sensor node*/
                App_Handle_DataFromDistanceSensor();
                break;
            /* If received data message from Rotation sensor node */
            case RX_ROTATION_DATA_ID:
                /* Send confirm message to Rotation sensor node */
                App_Handle_DataFromRotationSensor();
                break;
            default:
                break;
            }
        }
        status = QUEUE_DONE_FAILED;
    }

    return 0;
}

static void App_CANReceiveNotification(void)
{
    /* Structure to save received CAN data */
    Data_Typedef CAN_Data_Receive = {0u};

    /* Struture contain Data was converted from CAN Message to be used as param input of MID_Receive_EnQueue()*/
    ReceiveFrame_t l_Data_Receive = {0U};

    uint8_t index = 0u;

    /* Array of mailbox IDs and their associated data */
    static const uint8_t messageBoxes[] =
    {
        RX_DISTANCE_DATA_MB,
        RX_ROTATION_DATA_MB,
        RX_CONFIRM_FROM_DISTANCE_NODE_MB,
        RX_CONFIRM_FROM_ROTATION_NODE_MB
    };

    for (index = 0u; index < (sizeof(messageBoxes) / sizeof(messageBoxes[0])); index++)
    {
        if (MID_CheckCommingMessageEvent(messageBoxes[index]) == CAN_MSG_RECEIVED)
        {
            MID_CAN_ReceiveMessage(messageBoxes[index], &CAN_Data_Receive);

            l_Data_Receive.ID = CAN_Data_Receive.ID;
            l_Data_Receive.Data = CAN_Data_Receive.Data;

            MID_ClearMessageCommingEvent(messageBoxes[index]);
            (void)MID_Receive_EnQueue(&l_Data_Receive);
        }
        else
        {
            /* Do nothing */
        }
    }
}

static void App_CANBusOffNotification(void)
{

}

static void App_UART_TxNotification(void)
{
    uint8_t               data   = 0u;
    QueueCheckOperation_t status = MID_Transmit_Dequeue(&data);

    if (status == QUEUE_DONE_SUCCESS)
    {
        if (data != '\0')
        {
            MID_UART_SendData(data);
        }
        else
        {
            MID_UART_SetTxInterrupt(false);
        }
    }
    else
    {
        MID_UART_SetTxInterrupt(false);
    }
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

static void App_Handle_DataFromDistanceSensor(void)
{
    /* Send confirm message to distance sensor node */
    MID_CAN_SendCANMessage(TX_CONFIRM_DISTANCE_DATA_MB, TX_MSG_CONFIRM_DATA);
    /**/
    APP_Compose_UARTFrame(DISTANCE_DATA_ID, Processing_Msg.Data, Transmit_Data_Str);

    while (Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

static void App_Handle_DataFromRotationSensor(void)
{
    /* Send confirm message to rotation sensor */
    MID_CAN_SendCANMessage(TX_CONFIRM_ROTATION_DATA_MB, TX_MSG_CONFIRM_DATA);
    /* Convert message for uart transfer */
    APP_Compose_UARTFrame(ROTATION_DATA_ID, Processing_Msg.Data, Transmit_Data_Str);
    /* Push message to transmit queue and enable Tx interrupt to send */
    while(Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}
