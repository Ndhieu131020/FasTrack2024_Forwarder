#ifndef MID_TIMER_INTERFACE_H_
#define MID_TIMER_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include "common_typedef.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/** @defgroup Hardware definition
  * @{
  */
#define LPIT_INSTANCE     0u
#define TIMEOUT_COUNTER_CHANNEL  LPIT_CH0

/** @defgroup Counter for Timeout counting process
  * @{
  */
#define D_NODE_COMMINGDATA_CNT          0u  /* Counter for incoming data from Distance Sensor Node */
#define R_NODE_COMMINGDATA_CNT          1u  /* Counter for incoming data from Rotation Sensor Node */
#define D_NODE_RESPONDCONNECTION_CNT    2u  /* Counter for Distance Sensor Node connection response */
#define R_NODE_RESPONDCONNECTION_CNT    3u  /* Counter for Rotation Sensor Node connection response */
#define PC_RESPOND_DATA_CNT             4u  /* Counter for respond message of data from Pc Tool */

/** @defgroup Gate controller for Timeout Counter
  * @{
  */
#define D_NODE_COMMINGDATA_GATE          0u  /* Gate control for Distance Sensor Node incoming data */
#define R_NODE_COMMINGDATA_GATE          1u  /* Gate control for Rotation Sensor Node incoming data */
#define D_NODE_RESPONDCONNECTION_GATE    2u  /* Gate control for Rotation Sensor Node response */
#define R_NODE_RESPONDCONNECTION_GATE    3u  /* Gate control for Rotation Sensor Node response */
#define PC_RESPOND_DATA_GATE             4u  /* Gate control for respond message from Pc Tool */

/** @defgroup Flag represent to Timeout Event
  * @{
  */
#define D_NODE_TIMEOUT_EVENT                  0u  /* Timeout event for Distance Sensor Node */
#define R_NODE_TIMEOUT_EVENT                  1u  /* Timeout event for Rotation Sensor Node */
#define D_NODE_RESPOND_TIMEOUT_EVENT          2u  /* Timeout event for Distance Sensor Node response */
#define R_NODE_RESPOND_TIMEOUT_EVENT          3u  /* Timeout event for Rotation Sensor Node response */
#define PC_RESPOND_DATA_TIMEOUT_EVENT         4u  /* Timeout event for respond message from Pc Tool */

typedef enum
{
    EVENT_NONE,
    EVENT_SET,
} Event_Typedef;

/*******************************************************************************
 * API
 ******************************************************************************/
/**
  * @brief  Initializes the LPIT timer for managing timeout events.
  *
  * @param  None
  * @retval None
  */
void MID_Timer_Init(void);

/**
  * @brief  Starts the LPIT timer.
  * @param  None
  * @retval None
  */
void MID_Timer_StartTimer(void);

/**
  * @brief  Stops the LPIT timer.
  * @param  None
  * @retval None
  */
void MID_Timer_StopTimer(void);

/**
  * @brief     Writes a timeout event for a specific instance.
  *
  * @param[in] instance: The instance index for which the event is being set.
  *            this parameter can be a value of @defgroup Flag represent to Timeout Event
  * @param[in] event:    The event type to be set (e.g., EVENT_SET, EVENT_NONE).
  * @retval    None
  */
void MID_TimeoutService_WriteEvent(uint8_t instance, Event_Typedef event);

/**
  * @brief     Get a timeout event for a specific instance.
  *
  * @param[in] instance: The instance index for which the event is being set.
  *            this parameter can be a value of @defgroup Flag represent to Timeout Event
  * @retval    event:    The event type to be return (e.g., EVENT_SET, EVENT_NONE).
  */
Event_Typedef MID_TimeoutService_GetEvent(uint8_t instance);

/**
  * @brief     Resets the timeout counter for a specific instance.
  *
  * @param[in] instance The instance index whose timeout counter needs to be reset.
  *            this parameter can be a value of @defgroup Counter for Timeout counting process
  * @retval    None
  */
void MID_TimeoutService_ResetCounter(uint8_t instance);

/**
  * @brief     Enables or disables the counter gate for a specific instance.
  *
  * @param[in] instance: The instance index whose timeout counter needs to be reset.
  *            this parameter can be a value of @defgroup Gate controller for Timeout Counter
  * @param[in] state: The desired state of the counter gate (ENABLE or DISABLE).
  * @retval    None
  */
void MID_TimeoutService_CounterCmd(uint8_t instance, Functional_State state);

#endif /* MID_TIMER_INTERFACE_H_ */
