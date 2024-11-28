#include "DRV_S32K144_PORT.h"
#include "DRV_S32K144_FLEXCAN.h"
#include "DRV_S32K144_MCU.h"
#include "MID_CAN_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/
#define FLEXCAN_BITRATE     (500000u)
#define FLEXCAN_D_LENGTH    (4u)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/**
  * @brief      Initialize the pins for FLEXCAN0
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Pin_Init(void);

/**
  * @brief      Initialize FLEXCAN0
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_ParamConfig(void);

/**
  * @brief      Initialize the transmit message buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Tx_Mb_Init(void);

/**
  * @brief      Initialize the receive message buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Rx_Mb_Init(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
static flexcan_handle_t handle;

static flexcan_mb_t Transmit_Message;

static flexcan_mb_t Receive_Message;

flexcan_mb_config_t mbCfg =
{
    .idType = FLEXCAN_MB_ID_STD,
    .dataLength = FLEXCAN_D_LENGTH
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  * @brief      Initialize the pins for FLEXCAN0
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Pin_Init(void)
{
    virtual_pin_id_t Flexcan_Tx_Pin = PTE5;
    virtual_pin_id_t Flexcan_Rx_Pin = PTE4;

    const PortConfig_t  PortConfigCAN =
    {
        .Mux         =  PORT_MUX_ALT5,                    /* Configure the pin mux mode as CAN          */
        .Interrupt   =  PORT_INT_DISABLED,                /* Disable interrupt for the pin              */
        .Pull        =  PORT_INTERNAL_PULL_NOT_ENABLED    /* Disable internal pull resistor for the pin */
    };
    DRV_PORT_Init((uint8_t)GET_PORT(Flexcan_Tx_Pin), (uint8_t)GET_PIN_NUM(Flexcan_Tx_Pin), &PortConfigCAN);
    DRV_PORT_Init((uint8_t)GET_PORT(Flexcan_Rx_Pin), (uint8_t)GET_PIN_NUM(Flexcan_Rx_Pin), &PortConfigCAN);
}

/**
  * @brief      Initialize FLEXCAN0
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_ParamConfig(void)
{
    uint32_t CAN_ClkFreq = 0u;

    flexcan_module_config_t moduleCfg =
    {
        .clkSrc = FLEXCAN_CLK_SRC_PERIPH,
        .flexcanClkFreq = CAN_ClkFreq,
        .bitrate = FLEXCAN_BITRATE,
        .rxMaskType = FLEXCAN_RX_MASK_INDIVIDUAL,
        .flexcanMode = FLEXCAN_NORMAL_MODE
    };

    if(moduleCfg.clkSrc == FLEXCAN_CLK_SRC_PERIPH)
    {
        DRV_Clock_GetFrequency(FlexCAN0_CLK, &CAN_ClkFreq);
    }
    else
    {
        DRV_Clock_GetFrequency(SOSCDIV2_CLK, &CAN_ClkFreq);
    }

    if(CAN_ClkFreq != 0u)
    {
        moduleCfg.flexcanClkFreq = CAN_ClkFreq;
    }

    DRV_FLEXCAN_Init(FLEXCAN_INSTANCE, &moduleCfg, &handle);
}

/**
  * @brief      Initialize the transmit message buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Tx_Mb_Init(void)
{
    /* Initialize Tx Message buffer to send confirm distance data to CAN Bus */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_CONFIRM_DISTANCE_DATA_MB, &mbCfg, TX_CONFIRM_DISTANCE_DATA_ID);

    /* Initialize Tx Message buffer to send confirm rotation data to CAN Bus */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_CONFIRM_ROTATION_DATA_MB, &mbCfg, TX_CONFIRM_ROTATION_DATA_ID);

    /* Initialize Tx Message buffer to request connection state from Distance Node */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_RQ_CONNECT_DISTANCE_NODE_MB, &mbCfg, TX_RQ_CONNECT_DISTANCE_NODE_ID);

    /* Initialize Tx Message buffer to request connection state from Rotation Node */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_RQ_CONNECT_ROTATION_NODE_MB, &mbCfg, TX_RQ_CONNECT_ROTATION_NODE_ID);

    /* Initialize Tx Message buffer to send stop operation to Distance Node */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_STOPOPR_DISTANCE_NODE_MB, &mbCfg, TX_STOPOPR_DISTANCE_NODE_ID);

    /* Initialize Tx Message buffer to send stop operation to Rotation Node */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_STOPOPR_ROTATION_NODE_MB, &mbCfg, TX_STOPOPR_ROTATION_NODE_ID);

    /* Initialize Tx Message buffer to send ping message to distance node to CAN Bus */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_PING_DISTANCE_NODE_MB, &mbCfg, TX_PING_DISTANCE_NODE_ID);

    /* Initialize Tx Message buffer to send ping message to rotation node to CAN Bus */
    DRV_FLEXCAN_ConfigTxMb(FLEXCAN_INSTANCE, TX_PING_ROTATION_NODE_MB, &mbCfg, TX_PING_ROTATION_NODE_ID);
}

/**
  * @brief      Initialize the receive message buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
static void FLEXCAN_Rx_Mb_Init(void)
{
    DRV_FLEXCAN_SetRxMbGlobalMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, GMASK_FILTER_ALL_ID);

    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_DISTANCE_DATA_MB, IMASK_FILTER_ALL_ID);
    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_ROTATION_DATA_MB, IMASK_FILTER_ALL_ID);
    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_CONFIRM_FROM_DISTANCE_NODE_MB, IMASK_FILTER_ALL_ID);
    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_CONFIRM_FROM_ROTATION_NODE_MB, IMASK_FILTER_ALL_ID);
    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_CONFIRM_PING_DISTANCE_NODE_MB, IMASK_FILTER_ALL_ID);
    DRV_FLEXCAN_SetRxMbIndividualMask(FLEXCAN_INSTANCE, FLEXCAN_MB_ID_STD, RX_CONFIRM_PING_ROTATION_NODE_MB, IMASK_FILTER_ALL_ID);

    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_DISTANCE_DATA_MB, &mbCfg, RX_DISTANCE_DATA_ID);
    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_ROTATION_DATA_MB, &mbCfg, RX_ROTATION_DATA_ID);
    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_CONFIRM_FROM_DISTANCE_NODE_MB, &mbCfg, RX_CONFIRM_FROM_DISTANCE_NODE_ID);
    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_CONFIRM_FROM_ROTATION_NODE_MB, &mbCfg, RX_CONFIRM_FROM_ROTATION_NODE_ID);
    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_CONFIRM_PING_DISTANCE_NODE_MB, &mbCfg, RX_CONFIRM_PING_DISTANCE_NODE_ID);
    DRV_FLEXCAN_ConfigRxMb(FLEXCAN_INSTANCE, RX_CONFIRM_PING_ROTATION_NODE_MB, &mbCfg, RX_CONFIRM_PING_ROTATION_NODE_ID);

    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_DISTANCE_DATA_MB);
    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_ROTATION_DATA_MB);
    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_CONFIRM_FROM_DISTANCE_NODE_MB);
    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_CONFIRM_FROM_ROTATION_NODE_MB);
    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_CONFIRM_PING_DISTANCE_NODE_MB);
    DRV_FLEXCAN_EnableMbInt(FLEXCAN_INSTANCE, RX_CONFIRM_PING_ROTATION_NODE_MB);
}

/**
  * @brief      Initialize FLEXCAN0 module and all MBs used
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_CAN_Init(void)
{
    FLEXCAN_Pin_Init();
    FLEXCAN_ParamConfig();
    FLEXCAN_Tx_Mb_Init();
    FLEXCAN_Rx_Mb_Init();
}

/**
  * @brief      Initialize all Message Buffers for CAN communication
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_CAN_MailboxInit(void)
{
    FLEXCAN_Tx_Mb_Init();
    FLEXCAN_Rx_Mb_Init();
}

/**
  * @brief      Register a callback function for receiving CAN messages
  * @param[in]  cb_ptr Pointer to the callback function
  * @param[out] None
  * @retval     None
  */
void MID_CAN_RegisterRxNotificationCallback(void (*cb_ptr)(void))
{
    DRV_FLEXCAN_RegisterMbCallback(FLEXCAN_INSTANCE, cb_ptr);
}

/**
  * @brief      Register a callback function for the CAN bus-off event
  * @param[in]  cb_ptr Pointer to the callback function
  * @param[out] None
  * @retval     None
  */
void MID_CAN_RegisterBusOffNotificationCallback(void (*cb_ptr)(void))
{
    DRV_FLEXCAN_RegisterBusOffCallback(FLEXCAN_INSTANCE, cb_ptr);
}

/**
  * @brief      Receive a CAN message from the specified mailbox
  * @param[in]  mbIdx Index of the mailbox to receive from
  * @param[out] data Pointer to the data structure to store received message
  * @retval     None
  */
void MID_CAN_ReceiveMessage(uint8_t mbIdx, Data_Typedef *data)
{
    DRV_FLEXCAN_ReceiveInt(FLEXCAN_INSTANCE, mbIdx, &Receive_Message);

    data->ID = Receive_Message.msgId;
    data->Data = Receive_Message.data[0];
}

/**
  * @brief      Send a CAN message from the specified mailbox
  * @param[in]  Tx_Mb Index of the transmit mailbox
  * @param[in]  Data  Data to be sent
  * @param[out] None
  * @retval     None
  */
void MID_CAN_SendCANMessage(uint8_t Tx_Mb, int16_t Data)
{
    Transmit_Message.data[0] = Data;

    DRV_FLEXCAN_Transmit(FLEXCAN_INSTANCE, Tx_Mb, &Transmit_Message);
}

/**
  * @brief      Clear message event for a specific mailbox
  * @param[in]  Mailbox Index of the mailbox
  * @param[out] None
  * @retval     None
  */
void MID_ClearMessageCommingEvent(uint8_t Mailbox)
{
    DRV_FLEXCAN_ClearMbIntFlag(FLEXCAN_INSTANCE, Mailbox);
}

/**
  * @brief      Check if a message event occurred in the specified mailbox
  * @param[in]  Mailbox Index of the mailbox
  * @param[out] None
  * @retval     Status of the message event (1 if occurred, 0 otherwise)
  */
uint8_t MID_CheckCommingMessageEvent(uint8_t Mailbox)
{
    return DRV_FLEXCAN_GetMbIntFlag(FLEXCAN_INSTANCE, Mailbox);
}
