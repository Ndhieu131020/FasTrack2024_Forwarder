#include "DRV_S32K144_LPIT.h"
#include "DRV_S32K144_MCU.h"
#include "MID_Timer_Interface.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define RELOAD_PERIOD_MS   100u  /* Timer period in milliseconds */
#define MS_TO_SECOND       1000u /* Conversion factor from milliseconds to seconds */

#define TIMEOUT_THRESHOLD    10u /* Timeout threshold for triggering events */
#define COUNTER_INSTANCE     5u

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void TimeoutCounter_Notification(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Array contain timeout counter of all instance */
static uint8_t Timeout_Counter[COUNTER_INSTANCE] = {0u};

/* Array contain counter gate of all instance */
static Functional_State Counter_Gate[COUNTER_INSTANCE] = {DISABLE};

/* Array contain timeout event of all instance */
static Event_Typedef Timeout_Event[COUNTER_INSTANCE] = {EVENT_NONE};

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Initializes the LPIT timer for managing timeout events.
  *
  * @param  None
  * @retval None
  */
void MID_Timer_Init(void)
{
    uint32_t reloadValue = 0u;
    uint32_t LPIT_Freq   = 0u;

    LPIT_InitTypedef LPIT_InitStructure;

    /* Enable the LPIT module */
    DRV_LPIT_EnableModule(LPIT_INSTANCE);

    /* Stop the timer channel */
    DRV_LPIT_StopTimerChannel(LPIT_INSTANCE, TIMEOUT_COUNTER_CHANNEL);

    /* Configure LPIT parameters */
    LPIT_InitStructure.LPIT_ChainChannel = DISABLE;
    LPIT_InitStructure.LPIT_OperationMode = Periodic_Cnt_32b;
    LPIT_InitStructure.LPIT_Interupt = ENABLE;

    DRV_LPIT_Init(LPIT_INSTANCE, TIMEOUT_COUNTER_CHANNEL, &LPIT_InitStructure);

    /* Get the clock frequency of LPIT */
    DRV_Clock_GetFrequency(LPIT0_CLK, &LPIT_Freq);

    if (LPIT_Freq != 0U)
    {
        /* Calculate the reload value */
        reloadValue = (LPIT_Freq * RELOAD_PERIOD_MS) / MS_TO_SECOND;
    }
    else
    {
        /* Error handling: Clock frequency is zero */
    }

    /* Set the reload value for the LPIT timer */
    DRV_LPIT_SetReloadValue(LPIT_INSTANCE, TIMEOUT_COUNTER_CHANNEL, reloadValue);

    /* Register the callback for timer expiration */
    DRV_LPIT0_RegisterIntCallback(TIMEOUT_COUNTER_CHANNEL, TimeoutCounter_Notification);
}

/**
  * @brief  Starts the LPIT timer.
  * @param  None
  * @retval None
  */
void MID_Timer_StartTimer(void)
{
    DRV_LPIT_StartTimerChannel(LPIT_INSTANCE, TIMEOUT_COUNTER_CHANNEL);
}

/**
  * @brief  Stops the LPIT timer.
  * @param  None
  * @retval None
  */
void MID_Timer_StopTimer(void)
{
    DRV_LPIT_StopTimerChannel(LPIT_INSTANCE, TIMEOUT_COUNTER_CHANNEL);
}

/**
  * @brief  Callback function triggered when the timer expires.
  *
  * @param  None
  * @retval None
  */
static void TimeoutCounter_Notification(void)
{
    uint8_t index = 0u;

    /* Increment counters if the respective gates are enabled */
    for(index = 0u; index < COUNTER_INSTANCE; index++)
    {
        if(Counter_Gate[index] == ENABLE)
        {
            /* Start Counter */
            (Timeout_Counter[index])++;
        }
    }

    /* Check and set timeout events if thresholds are exceeded */
    for(index = 0u; index < COUNTER_INSTANCE; index++)
    {
        if(Timeout_Counter[index] > TIMEOUT_THRESHOLD)
        {
            /* Raise event */
            Timeout_Event[index] = EVENT_SET;
        }
    }
}

/**
  * @brief     Writes a timeout event for a specific instance.
  *
  * @param[in] instance: The instance index for which the event is being set.
  *            this parameter can be a value of @defgroup Flag represent to Timeout Event
  * @param[in] event:    The event type to be set (e.g., EVENT_SET, EVENT_NONE).
  * @retval    None
  */
void MID_TimeoutService_WriteEvent(uint8_t instance, Event_Typedef event)
{
    Timeout_Event[instance] = event;
}

/**
  * @brief     Get a timeout event for a specific instance.
  *
  * @param[in] instance: The instance index for which the event is being set.
  *            this parameter can be a value of @defgroup Flag represent to Timeout Event
  * @retval    event:    The event type to be return (e.g., EVENT_SET, EVENT_NONE).
  */
Event_Typedef MID_TimeoutService_GetEvent(uint8_t instance)
{
    return Timeout_Event[instance];
}

/**
  * @brief     Resets the timeout counter for a specific instance.
  *
  * @param[in] instance The instance index whose timeout counter needs to be reset.
  *            this parameter can be a value of @defgroup Counter for Timeout counting process
  * @retval    None
  */
void MID_TimeoutService_ResetCounter(uint8_t instance)
{
    Timeout_Counter[instance] = 0u;
}

/**
  * @brief     Enables or disables the counter gate for a specific instance.
  *
  * @param[in] instance: The instance index whose timeout counter needs to be reset.
  *            this parameter can be a value of @defgroup Gate controller for Timeout Counter
  * @param[in] state: The desired state of the counter gate (ENABLE or DISABLE).
  * @retval    None
  */
void MID_TimeoutService_CounterCmd(uint8_t instance, Functional_State state)
{
    Counter_Gate[instance] = state;
    Timeout_Counter[instance] = 0u;
}
