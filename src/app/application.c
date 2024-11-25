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
#define MSG_LENGTH_MAX    12u

#define IDLE              0u
#define STOP              1u
#define RUNNING           2u

#define D_LOCK            0u
#define R_LOCK            1u
#define UNLOCK            2u

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void App_CANReceiveNotification(void);
static void App_UART_TxNotification(void);
static void App_UART_RxNotification(void);

static void App_Handle_DataFromDistanceSensor(void);
static void App_Handle_DataFromRotationSensor(void);
static void App_Handle_ConfirmConnectionFromDistanceSensor(void);
static void App_Handle_ConfirmConnectionFromRotationSensor(void);
static void App_Handle_RequestConnectFromPcToFw(void);
static void App_Handle_RequestConnectFromPcToDistanceNode(void);
static void App_Handle_RequestConnectFromPcToRotationNode(void);
static void App_Handle_ReceivePingFromDistanceNode(void);
static void App_Handle_ReceivePingFromRotationNode(void);
static void App_Handle_ConfirmDataFromPCTool(void);
static void App_Handle_TimeoutEvent(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Array stores UART frame from PC Tool */
static uint8_t Receive_Data_Str[MSG_LENGTH_MAX] = {0};
static uint8_t Receive_Data_Idx                 = 0u;

/* Array stores UART frame to transmit PC Tool */
static uint8_t Transmit_Data_Str[MSG_LENGTH_MAX] = {0};
static uint8_t Transmit_Data_Idx                 = 0u;

/* Lastest sensor value */
static uint16_t Current_D_Value = 0u;
static uint16_t Current_R_Value = 0u;

/* Sensor node operation status */
static uint8_t Node_State = IDLE;
/* Sensor node operation status */
static uint8_t PcTool_Timer_Lock_State = UNLOCK;

static ReceiveFrame_t Processing_Msg = {0};

/*   Variables to track if timeout notification has been sent to user layer for Distance and Rotation sensor node */
static bool g_Dnode_isTimeoutNotified = false; /* Distance sensor node timeout notification flag */
static bool g_Rnode_isTimeoutNotified = false; /* Rotation sensor node timeout notification flag */

/*******************************************************************************
 * Code
 ******************************************************************************/

int main(void)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    /* System initialization */
    MID_Clock_Init();
    MID_CAN_Init();
    MID_Timer_Init();
    MID_UART_Init();
    MID_Transmit_Queue_Init();
    MID_Receive_Queue_Init();

    /* Register Notification */
    MID_CAN_RegisterRxNotificationCallback(App_CANReceiveNotification);
    MID_UART_RegisterNotificationCallback(App_UART_TxNotification, App_UART_RxNotification);

    /* Allow notification */
    MID_EnableNotification();

    /* Start timer */
    MID_Timer_StartTimer();

    /* Enable timeout gate for data message */
    MID_TimeoutService_CounterCmd(D_NODE_COMMINGDATA_GATE, ENABLE);
    MID_TimeoutService_CounterCmd(R_NODE_COMMINGDATA_GATE, ENABLE);
    MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_CNT, ENABLE);

    while(1)
    {

        status = MID_Receive_DeQueue(&Processing_Msg);

        if(status == QUEUE_DONE_SUCCESS)
        {
            switch (Processing_Msg.ID)
            {
            /* If received data message from Distance sensor node */
            case RX_DISTANCE_DATA_ID:
                /* Reset timeout counter */
                MID_TimeoutService_ResetCounter(D_NODE_COMMINGDATA_CNT);

                /* Send confirm message to Distance sensor node */
                App_Handle_DataFromDistanceSensor();

                if(PcTool_Timer_Lock_State == UNLOCK)
                {
                    /* Start counter to calculate timeout for respond data message from Pc Tool */
                    MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_GATE, ENABLE);
                    PcTool_Timer_Lock_State = D_LOCK;
                }

                break;

            /* If received data message from Rotation sensor node */
            case RX_ROTATION_DATA_ID:

                /* Send confirm message to Rotation sensor node */
                App_Handle_DataFromRotationSensor();

                /* Reset timeout counter */
                MID_TimeoutService_ResetCounter(R_NODE_COMMINGDATA_CNT);

                if(PcTool_Timer_Lock_State == UNLOCK)
                {
                    /* Start counter to calculate timeout for respond data message from Pc Tool */
                    MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_GATE, ENABLE);
                    PcTool_Timer_Lock_State = R_LOCK;
                }

                break;

            /* If receive confirm request connection message to intialize PC Tool */
            case RX_CONFIRM_FROM_DISTANCE_NODE_ID:

                /* Send confirm connection to PC tool */
                App_Handle_ConfirmConnectionFromDistanceSensor();
                break;

            /* If receive confirm request connection message to intialize PC Tool */
            case RX_CONFIRM_FROM_ROTATION_NODE_ID:

                /* Send confirm connection to PC tool */
                App_Handle_ConfirmConnectionFromRotationSensor();
                break;

            /* If receive confirm ping message from distance node */
            case RX_CONFIRM_PING_DISTANCE_NODE_ID:

                App_Handle_ReceivePingFromDistanceNode();

                /* Disable timeout counter */
                MID_TimeoutService_CounterCmd(D_NODE_RESPONDCONNECTION_GATE, DISABLE);

                g_Dnode_isTimeoutNotified = false;
                break;

            /* If receive confirm ping message from rotation node */
            case RX_CONFIRM_PING_ROTATION_NODE_ID:
                App_Handle_ReceivePingFromRotationNode();

                /* Disable timeout counter */
                MID_TimeoutService_CounterCmd(R_NODE_RESPONDCONNECTION_GATE, DISABLE);

                g_Rnode_isTimeoutNotified = false;
                break;

            /* If received request connection message from PC Tool to itself */
            case PC_CONNECT_FORWARDER_ID:
                /* Reset timeout counter */
                MID_TimeoutService_ResetCounter(R_NODE_COMMINGDATA_CNT);

                App_Handle_RequestConnectFromPcToFw();
                break;

            /* If received request connection message from PC Tool to distance sensor node */
            case PC_CONNECT_DISTANCE_SENSOR_ID:
                App_Handle_RequestConnectFromPcToDistanceNode();
                break;

            /* If received request connection message from PC Tool to rotation sensor node */
            case PC_CONNECT_ROTATION_SENSOR_ID:

                App_Handle_RequestConnectFromPcToRotationNode();
                break;

            /* If received confirm data from PC Tools */
            case DISTANCE_DATA_ID:
                App_Handle_ConfirmDataFromPCTool();

                if (PcTool_Timer_Lock_State == D_LOCK)
                {
                    /* Disable timeout counter */
                    MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_GATE, DISABLE);
                    PcTool_Timer_Lock_State = UNLOCK;
                }

                break;

            /* If received confirm data from PC Tools */
            case ROTATION_DATA_ID:
                App_Handle_ConfirmDataFromPCTool();

                if (PcTool_Timer_Lock_State == R_LOCK)
                {
                    /* Disable timeout counter */
                    MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_GATE, DISABLE);
                    PcTool_Timer_Lock_State = UNLOCK;
                }

                break;

            default:
                break;
            }
        }

        /* Handle timeout function */
        App_Handle_TimeoutEvent();
    }
    return 0;
}

/**
 * @brief Handles CAN message reception and processing.
 *
 * This function checks for received CAN messages in predefined mailboxes,
 * retrieves their data, converts the data into a structure suitable for
 * further processing, clears the associated message event, and enqueues
 * the processed data for subsequent handling.
 *
 * @note This function operates on a predefined set of mailboxes and
 *       expects that the necessary message event and data handling
 *       functions are implemented.
 *
 * @param None
 * @retval None
 */
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
        RX_CONFIRM_FROM_ROTATION_NODE_MB,
        RX_CONFIRM_PING_DISTANCE_NODE_MB,
        RX_CONFIRM_PING_ROTATION_NODE_MB,
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

/**
 * @brief Handles UART transmit notification and data transmission.
 *
 * This function is triggered when a UART transmit interrupt occurs. It dequeues
 * data from the transmit queue, sends the data if valid, and manages the UART
 * transmit interrupt state. If the queue is empty or a null terminator is encountered,
 * the UART transmit interrupt is disabled.
 *
 * @param None
 * @retval None
 */
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

/**
 * @brief Handles UART receive notification and processes incoming data.
 *
 * This function is triggered when a UART receive interrupt occurs. It retrieves
 * incoming UART data byte by byte, stores it in a buffer, and processes the data
 * when the end of a frame (newline character) is detected. The processed data is
 * enqueued for further handling.
 *
 * @param None
 * @retval None
 */
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
        App_Parser_UARTFrame(Receive_Data_Str, Receive_Data_Idx, &l_Data_Receive);

        /* Push to receive Queue */
        (void)MID_Receive_EnQueue(&l_Data_Receive);

        /* Clear data index */
        Receive_Data_Idx = 0u;
    }
}

/**
  * @brief Handles data processing and communication for the distance sensor.
  *
  * This function manages data received from the distance sensor by sending a
  * confirmation message to the sensor node and forwarding the processed data
  * to the UART for transmission.
  *
  * @param None
  * @return None
  */
static void App_Handle_DataFromDistanceSensor(void)
{
    /* Send confirm message to distance sensor node */
    MID_CAN_SendCANMessage(TX_CONFIRM_DISTANCE_DATA_MB, TX_MSG_CONFIRM_DATA);

    Current_D_Value = Processing_Msg.Data;
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

/**
  * @brief Handles data processing and communication for the rotation sensor.
  *
  * This function manages data received from the rotation sensor by sending a
  * confirmation message to the sensor node and forwarding the processed data
  * to the UART for transmission.
  *
  * @param None
  * @retval None
  */
static void App_Handle_DataFromRotationSensor(void)
{
    /* Send confirm message to rotation sensor */
    MID_CAN_SendCANMessage(TX_CONFIRM_ROTATION_DATA_MB, TX_MSG_CONFIRM_DATA);

    Current_R_Value = Processing_Msg.Data;
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

/**
  * @brief Handles confirmation of connection from the distance sensor node.
  *
  * This function processes a confirmation connection message received from the
  * distance sensor node and forwards a corresponding confirmation message to the PC.
  *
  * @param None
  * @return None
  */
static void App_Handle_ConfirmConnectionFromDistanceSensor(void)
{
    /* FW receive Confirm Connection msg from Distance node */
    /* FW send Confirm Connection to PC */
    APP_Compose_UARTFrame(PC_CONNECT_DISTANCE_SENSOR_ID, CONFIRM_CONNECTION_DATA, Transmit_Data_Str);
    /* Push message to transmit queue and enable Tx interrupt to send */
    while(Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

/**
  * @brief Handles confirmation of connection from the rotation sensor node.
  *
  * This function processes a confirmation connection message received from the
  * rotation sensor node and forwards a corresponding confirmation message to the PC.
  *
  * @param None
  * @return None
  */
static void App_Handle_ConfirmConnectionFromRotationSensor(void)
{
    /* FW receive Confirm Connection msg from Rotation node */
    /* FW send Confirm Connection to PC */
    APP_Compose_UARTFrame(PC_CONNECT_ROTATION_SENSOR_ID, CONFIRM_CONNECTION_DATA, Transmit_Data_Str);
    /* Push message to transmit queue and enable Tx interrupt to send */
    while(Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

/**
  * @brief  Handles the connection request from PC to Forwarder (FW).
  *         This function composes a confirmation frame, enqueues it for transmission,
  * and enables the UART transmission interrupt to send the data.
  * @param  None
  * @retval None
  */
static void App_Handle_RequestConnectFromPcToFw(void)
{
    /* FW send Confirm Connection between itself and PC */
    APP_Compose_UARTFrame(PC_CONNECT_FORWARDER_ID, CONFIRM_CONNECTION_DATA, Transmit_Data_Str);
    /* Push message to transmit queue and enable Tx interrupt to send */
    while(Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

/**
  * @brief Handles a connection request from the PC to the distance sensor node.
  *
  * This function sends a CAN message to request a connection with the distance
  * sensor node, initiated by the PC.
  *
  * @param None
  * @return None
  */
static void App_Handle_RequestConnectFromPcToDistanceNode(void)
{
    /* Send connection request to distance sensor node */
    MID_CAN_SendCANMessage(TX_RQ_CONNECT_DISTANCE_NODE_MB, TX_MSG_REQUEST_DATA);
}

/**
  * @brief Handles a connection request from the PC to the rotation sensor node.
  *
  * This function sends a CAN message to request a connection with the rotation
  * sensor node, initiated by the PC.
  *
  * @param None
  * @return None
  */
static void App_Handle_RequestConnectFromPcToRotationNode(void)
{
    /* Send connection request to rotation sensor node */
    MID_CAN_SendCANMessage(TX_RQ_CONNECT_ROTATION_NODE_MB, TX_MSG_REQUEST_DATA);
}

/**
  * @brief Handles receiving a ping message from the distance sensor node.
  *
  * This function processes a "ping" message received from the distance sensor node
  * and sends back the current distance value to the UART, while managing the timeout
  * for the response counter.
  *
  * @param None
  * @return None
  */
static void App_Handle_ReceivePingFromDistanceNode(void)
{
    /**/
    APP_Compose_UARTFrame(DISTANCE_DATA_ID, Current_D_Value, Transmit_Data_Str);

    while (Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

/**
  * @brief Handles receiving a ping message from the rotation sensor node.
  *
  * This function processes a "ping" message received from the rotation sensor node
  * and sends back the current rotation value to the UART, while managing the timeout
  * for the response counter.
  *
  * @param None
  * @return None
  */
static void App_Handle_ReceivePingFromRotationNode(void)
{
    /* Convert message for uart transfer */
    APP_Compose_UARTFrame(ROTATION_DATA_ID, Current_R_Value, Transmit_Data_Str);
    /* Push message to transmit queue and enable Tx interrupt to send */
    while(Transmit_Data_Str[Transmit_Data_Idx] != '\0')
    {
        MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
        Transmit_Data_Idx++;
    }
    MID_UART_SetTxInterrupt(true);
    Transmit_Data_Idx = 0u;
}

/**
 * @brief Handles confirm message of rotation data from the Pc Tool.
 *
 * This function checks the operational state of the rotation sensor node and,
 * if it is stopped, sends a wake-up CAN message to resume its operation.
 *
 * @param None
 * @return None
 */
static void App_Handle_ConfirmDataFromPCTool(void)
{
    if(Node_State == STOP)
    {
        MID_CAN_SendCANMessage(TX_STOPOPR_DISTANCE_NODE_MB, TX_WAKEUP_DATA);

        MID_CAN_SendCANMessage(TX_STOPOPR_ROTATION_NODE_MB, TX_WAKEUP_DATA);

        Node_State = RUNNING;
    }
}

/**
  * @brief Handles timeout events related to the distance sensor node.
  *
  * This function processes timeout events, sending appropriate messages
  * or resetting counters and states as needed. It ensures that the system
  * responds to timeout scenarios for communication with the distance sensor node.
  *
  * @param None
  * @return None
  */
static void App_Handle_TimeoutEvent(void)
{
    if(MID_TimeoutService_GetEvent(D_NODE_TIMEOUT_EVENT) == EVENT_SET)
    {
        /* Send ping message to Distance sensor node */
        MID_CAN_SendCANMessage(TX_PING_DISTANCE_NODE_MB, TX_MSG_REQUEST_DATA);
        /* Start counter to calculate timeout for respond message from distance sensor node */
        MID_TimeoutService_CounterCmd(D_NODE_RESPONDCONNECTION_GATE, ENABLE);
        /* Reset state */
        MID_TimeoutService_WriteEvent(D_NODE_TIMEOUT_EVENT, EVENT_NONE);
        /* Reset timeout counter */
        MID_TimeoutService_ResetCounter(D_NODE_COMMINGDATA_CNT);
    }

    if(MID_TimeoutService_GetEvent(D_NODE_RESPOND_TIMEOUT_EVENT) == EVENT_SET)
    {
        if (g_Dnode_isTimeoutNotified == false)
        {
            /* Compress to UART String */
            APP_Compose_UARTFrame(DISTANCE_DATA_ID, SENSOR_DISCONNECT_DATA, Transmit_Data_Str);

            while (Transmit_Data_Str[Transmit_Data_Idx] != '\0')
            {
                MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
                Transmit_Data_Idx++;
            }
            MID_UART_SetTxInterrupt(true);
            Transmit_Data_Idx = 0u;

            g_Dnode_isTimeoutNotified = true;
        }

        MID_TimeoutService_CounterCmd(D_NODE_RESPONDCONNECTION_GATE, DISABLE);
        /* Reset state */
        MID_TimeoutService_WriteEvent(D_NODE_RESPOND_TIMEOUT_EVENT, EVENT_NONE);
    }

    if(MID_TimeoutService_GetEvent(R_NODE_TIMEOUT_EVENT) == EVENT_SET)
    {
        /* Send ping message to Rotation sensor node */
        MID_CAN_SendCANMessage(TX_PING_ROTATION_NODE_MB, TX_MSG_REQUEST_DATA);
        /* Start counter to calculate timeout for respond message from distance sensor node */
        MID_TimeoutService_CounterCmd(R_NODE_RESPONDCONNECTION_GATE, ENABLE);
        /* Reset state */
        MID_TimeoutService_WriteEvent(R_NODE_TIMEOUT_EVENT, EVENT_NONE);
        /* Reset timeout counter */
        MID_TimeoutService_ResetCounter(R_NODE_COMMINGDATA_CNT);
    }

    if(MID_TimeoutService_GetEvent(R_NODE_RESPOND_TIMEOUT_EVENT) == EVENT_SET)
    {
        if (g_Rnode_isTimeoutNotified == false)
        {
            /* Compress to UART String */
            APP_Compose_UARTFrame(ROTATION_DATA_ID, SENSOR_DISCONNECT_DATA, Transmit_Data_Str);

            while (Transmit_Data_Str[Transmit_Data_Idx] != '\0')
            {
                MID_Transmit_Enqueue(Transmit_Data_Str[Transmit_Data_Idx]);
                Transmit_Data_Idx++;
            }
            MID_UART_SetTxInterrupt(true);
            Transmit_Data_Idx = 0u;

            g_Rnode_isTimeoutNotified = true;
        }

        MID_TimeoutService_CounterCmd(R_NODE_RESPONDCONNECTION_GATE, DISABLE);
        /* Reset state */
        MID_TimeoutService_WriteEvent(R_NODE_RESPOND_TIMEOUT_EVENT, EVENT_NONE);
    }

    if(MID_TimeoutService_GetEvent(PC_RESPOND_DATA_TIMEOUT_EVENT) == EVENT_SET)
    {
        if(Node_State != STOP)
        {
            MID_CAN_SendCANMessage(TX_STOPOPR_DISTANCE_NODE_MB, TX_STOPOPR_DATA);
            MID_CAN_SendCANMessage(TX_STOPOPR_ROTATION_NODE_MB, TX_STOPOPR_DATA);

            MID_TimeoutService_CounterCmd(PC_RESPOND_DATA_GATE, DISABLE);
            /* Reset state */
            MID_TimeoutService_WriteEvent(PC_RESPOND_DATA_TIMEOUT_EVENT, EVENT_NONE);
            /* Set status of Distance node as Stop opreation */
            Node_State = STOP;
        }
    }
}
