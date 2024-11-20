/*
 *  Filename: MID_CAN_Interface.h
 *
 *  Created on: 11-08-2024
 *      Author: Ndhieu131020@gmail.com
*/

#ifndef MID_CAN_INTERFACE_H_
#define MID_CAN_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/

/*******************************************************************************
 * Definition
 ******************************************************************************/

#define FLEXCAN_INSTANCE        0u

/** @defgroup Initialize Connection Message ID
  * @{
  */
#define TX_RQ_CONNECT_DISTANCE_NODE_ID    0xE0
#define TX_RQ_CONNECT_ROTATION_NODE_ID    0xF0

#define RX_CONFIRM_FROM_DISTANCE_NODE_ID  0xE1
#define RX_CONFIRM_FROM_ROTATION_NODE_ID  0xF1

#define TX_STOPOPR_DISTANCE_NODE_ID       0x30
#define TX_STOPOPR_ROTATION_NODE_ID       0x31

/** @defgroup Ping Message ID
  * @{
  */
#define TX_PING_DISTANCE_NODE_ID          0x50
#define TX_PING_ROTATION_NODE_ID          0x60

#define RX_CONFIRM_PING_DISTANCE_NODE_ID  0x51
#define RX_CONFIRM_PING_ROTATION_NODE_ID  0x61

/** @defgroup Check Connection Message Data
  * @{
  */
#define TX_MSG_REQUEST_DATA    0x10
#define TX_MSG_CONFIRM_DATA    0xFF

/** @defgroup Data Message
  * @{
  */
#define RX_DISTANCE_DATA_ID  0x20
#define RX_ROTATION_DATA_ID  0x10

#define TX_CONFIRM_ROTATION_DATA_ID  0x11
#define TX_CONFIRM_DISTANCE_DATA_ID  0x21

/** @defgroup Allocate Tx mailboxs
  * @{
  */
#define TX_CONFIRM_DISTANCE_DATA_MB      0u
#define TX_CONFIRM_ROTATION_DATA_MB      1u

#define TX_RQ_CONNECT_DISTANCE_NODE_MB   2u
#define TX_RQ_CONNECT_ROTATION_NODE_MB   3u

#define TX_STOPOPR_DISTANCE_NODE_MB      4u
#define TX_STOPOPR_ROTATION_NODE_MB      5u

#define TX_PING_DISTANCE_NODE_MB         6u
#define TX_PING_ROTATION_NODE_MB         7u

/** @defgroup Allocate Rx mailboxs
  * @{
  */
#define RX_DISTANCE_DATA_MB                 10u
#define RX_ROTATION_DATA_MB                 11u

#define RX_CONFIRM_FROM_DISTANCE_NODE_MB    12u
#define RX_CONFIRM_FROM_ROTATION_NODE_MB    13u

#define RX_CONFIRM_PING_DISTANCE_NODE_MB    14u
#define RX_CONFIRM_PING_ROTATION_NODE_MB    15u

/** @defgroup New comming message state
  * @{
  */
#define CAN_MSG_RECEIVED      1u
#define CAN_MSG_NO_RECEIVED   0u

/** @defgroup Filter ID register mask
  * @{
  */
#define GMASK_FILTER_ALL_ID     0x1FFFFFFF
#define IMASK_FILTER_ALL_ID     0xFFFFFFFF

/* Comming Message Struct */
typedef struct MID_CAN_Interface
{
    uint32_t ID;        /* ID of message, it can be a value of @defgroup *_ID */
    uint32_t Data;      /* Data of message, it can be a value of @defgroup *_DATA */
}Data_Typedef;

/*******************************************************************************
 * API
 ******************************************************************************/

void MID_CAN_Init(void);

void MID_CAN_MailboxInit(void);

void MID_CAN_RegisterRxNotificationCallback(void (*cb_ptr)(void));

void MID_CAN_RegisterBusOffNotificationCallback(void (*cb_ptr)(void));

void MID_CAN_ReceiveMessage(uint8_t mbIdx, Data_Typedef *data);

void MID_CAN_SendCANMessage(uint8_t Tx_Mb, int16_t Data);

void MID_ClearMessageCommingEvent(uint8_t Mailbox);

uint8_t MID_CheckCommingMessageEvent(uint8_t Mailbox);

#endif /* MID_CAN_INTERFACE_H_ */