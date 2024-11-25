/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "MID_ReceiveQueue_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

/* The Queue is used to store all received data */
static CircularQueueReceive_t receiveQueue;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Check if QueueReceive is full */
static bool QueueReceive_isFull(const CircularQueueReceive_t *const pQueue);

/* Check if QueueReceive is empty */
static bool QueueReceive_isEmpty(const CircularQueueReceive_t *const pQueue);

/*******************************************************************************
 * Code
 ******************************************************************************/
/**
  * @brief      Initialize for QueueReceive
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_Receive_Queue_Init( void )
{
    uint16_t index = 0u;

    receiveQueue.front    = -1;
    receiveQueue.rear     = -1;
    receiveQueue.capacity = 0U;
    receiveQueue.size     = QUEUE_RECEIVE_SIZE;

    for (index = 0u; index < QUEUE_RECEIVE_SIZE; index++)
    {
        receiveQueue.queueArray[index].ID   = 0u;
        receiveQueue.queueArray[index].Data = 0u;
    }
}

/**
  * @brief      Add a data frame to the receive queue
  * @param[in]  pInData: Pointer to the input data frame
  * @param[out] None
  * @retval     QUEUE_DONE_SUCCESS if enqueue operation is successful, otherwise QUEUE_DONE_FAILED
  */
QueueCheckOperation_t MID_Receive_EnQueue( const ReceiveFrame_t *const pInData )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pInData != NULL )
    {
        if (!QueueReceive_isFull(&receiveQueue))
        {
            if ( receiveQueue.front == -1 )
            {
            	receiveQueue.front = 0;
            }
            else
            {
                /* Do Nothing */
            }
            receiveQueue.rear = ((receiveQueue.rear) + 1) % (receiveQueue.size);
            receiveQueue.queueArray[receiveQueue.rear] = *pInData;
            (receiveQueue.capacity)++;
            status = QUEUE_DONE_SUCCESS;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/**
  * @brief      Remove a data frame from the receive queue
  * @param[in]  None
  * @param[out] pOutData: Pointer to the output data frame
  * @retval     QUEUE_DONE_SUCCESS if dequeue operation is successful, otherwise QUEUE_DONE_FAILED
  */
QueueCheckOperation_t MID_Receive_DeQueue( ReceiveFrame_t *const pOutData )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pOutData !=NULL )
    {
        if (!QueueReceive_isEmpty(&receiveQueue))
        {
            *pOutData = receiveQueue.queueArray[receiveQueue.front];

            (receiveQueue.capacity)--;
            if (receiveQueue.front == receiveQueue.rear)
            {
            	receiveQueue.front = -1;
            	receiveQueue.rear  = -1;
            }
            else
            {
            	receiveQueue.front = ((receiveQueue.front) + 1) % (receiveQueue.size);
            }
            status = QUEUE_DONE_SUCCESS;
        }
        else
        {
        	/* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/**
  * @brief      Check if the receive queue is full
  * @param[in]  pQueue: Pointer to the receive queue
  * @param[out] None
  * @retval     true if the queue is full, otherwise false
  */
static bool QueueReceive_isFull( const CircularQueueReceive_t *const pQueue )
{
    bool status = false;

    if( pQueue != NULL )
    {
        if( (pQueue->capacity) == (pQueue->size) )
        {
            status = true;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/**
  * @brief      Check if the receive queue is empty
  * @param[in]  pQueue Pointer to the receive queue
  * @param[out] None
  * @retval     true if the queue is empty, otherwise false
  */
static bool QueueReceive_isEmpty( const CircularQueueReceive_t *const pQueue )
{
    bool status = false;

    if( pQueue != NULL )
    {
        if( pQueue->capacity == 0U )
        {
            status = true;
        }
        else
        {
            /* Do Nothing */
        }
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
