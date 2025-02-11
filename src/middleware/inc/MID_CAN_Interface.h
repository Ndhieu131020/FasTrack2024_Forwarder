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
#define TX_STOPOPR_ROTATION_NODE_ID       0x40

#define RX_CONFIRM_STOPOPR_DNODE_ID       0x31
#define RX_CONFIRM_STOPOPR_RNODE_ID       0x41

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

#define TX_STOPOPR_DATA   0x10
#define TX_WAKEUP_DATA    0xFF

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
#define RX_CONFIRM_STOPOPR_DNODE_MB         8u
#define RX_CONFIRM_STOPOPR_RNODE_MB         9u

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
} Data_Typedef;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
  * @brief      Initialize FLEXCAN0 module and all MBs used
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_CAN_Init(void);

/**
  * @brief      Initialize all Message Buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_CAN_MailboxInit(void);

/**
  * @brief      Register a callback function for receiving CAN messages
  * @param[in]  cb_ptr: Pointer to the callback function
  * @param[out] None
  * @retval     None
  */
void MID_CAN_RegisterRxNotificationCallback(void (*cb_ptr)(void));

/**
  * @brief      Register a callback function for the CAN bus-off event
  * @param[in]  cb_ptr: Pointer to the callback function
  * @param[out] None
  * @retval     None
  */
void MID_CAN_RegisterBusOffNotificationCallback(void (*cb_ptr)(void));

/**
  * @brief      Receive a CAN message from the specified mailbox
  * @param[in]  mbIdx: Index of the mailbox to receive from
  * @param[out] data: Pointer to the data structure to store received message
  * @retval     None
  */
void MID_CAN_ReceiveMessage(uint8_t mbIdx, Data_Typedef *data);

/**
  * @brief      Send a CAN message from the specified mailbox
  * @param[in]  Tx_Mb: Index of the transmit mailbox
  * @param[in]  Data:  Data to be sent
  * @param[out] None
  * @retval     None
  */
void MID_CAN_SendCANMessage(uint8_t Tx_Mb, int16_t Data);

/**
  * @brief      Clear message event for a specific mailbox
  * @param[in]  Mailbox: Index of the mailbox
  * @param[out] None
  * @retval     None
  */
void MID_ClearMessageCommingEvent(uint8_t Mailbox);

/**
  * @brief      Check if a message event occurred in the specified mailbox
  * @param[in]  Mailbox: Index of the mailbox
  * @param[out] None
  * @retval     Status of the message event (1 if occurred, 0 otherwise)
  */
uint8_t MID_CheckCommingMessageEvent(uint8_t Mailbox);

#endif /* MID_CAN_INTERFACE_H_ */
