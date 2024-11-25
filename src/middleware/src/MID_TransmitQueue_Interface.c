#include "MID_TransmitQueue_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Check if QueueTransmit is full */
static bool QueueTransmit_isFull( const CircularQueueTransmit_t *const pQueue );

/* Check if QueueTransmit is empty */
static bool QueueTransmit_isEmpty( const CircularQueueTransmit_t *const pQueue );

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Static instance of the queue structure used for transmit operations */
static CircularQueueTransmit_t transmitQueue;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Initializes the transmit queue by setting initial values
  *         for the Front, Rear, Size, Capacity and clearing the Data buffer.
  * @param  None
  * @retval None
  */
void MID_Transmit_Queue_Init(void)
{
    uint16_t index = 0u;

    transmitQueue.front    = -1;
    transmitQueue.rear     = -1;
    transmitQueue.capacity = 0u;
    transmitQueue.size     = TRANSMIT_QUEUE_SIZE;

    for (index = 0u; index < TRANSMIT_QUEUE_SIZE; index++)
    {
        transmitQueue.queueArray[index] = 0u;
    }
}

/**
  * @brief      Adds a data byte to the transmit queue.
  * @param[in]  data Data byte to enqueue.
  * @retval     QUEUE_DONE_SUCCESS if enqueue operation is successful.
  * @retval     QUEUE_DONE_FAILED if the queue is full.
  */
QueueCheckOperation_t MID_Transmit_Enqueue(const uint8_t data)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if ( !QueueTransmit_isFull(&transmitQueue) )
    {
        if ( transmitQueue.front == -1 )
        {
            transmitQueue.front = 0;
        }
        else
        {
            /* Do Nothing */
        }
        transmitQueue.rear = ((transmitQueue.rear) + 1) % (transmitQueue.size);
        transmitQueue.queueArray[transmitQueue.rear] = data;
        (transmitQueue.capacity)++;
        status = QUEUE_DONE_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/**
  * @brief      Retrieves and removes the data byte from the front of the queue.
  * @param[out] data Pointer to store dequeued data.
  * @retval     QUEUE_DONE_SUCCESS if dequeue operation is successful.
  * @retval     QUEUE_DONE_FAILED if the queue is empty.
  */
QueueCheckOperation_t MID_Transmit_Dequeue(uint8_t * pOutData)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pOutData != NULL )
    {
        if ( !QueueTransmit_isEmpty(&transmitQueue) )
        {
            *pOutData = transmitQueue.queueArray[transmitQueue.front];
            (transmitQueue.capacity)--;
            if (transmitQueue.front == transmitQueue.rear)
            {
            	transmitQueue.front = -1;
            	transmitQueue.rear  = -1;
            }
            else
            {
            	transmitQueue.front = ((transmitQueue.front) + 1) % (transmitQueue.size);
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
  * @brief  Checks if the transmit queue is full.
  * @param  None
  * @retval True if the queue is full, false otherwise.
  */
static bool QueueTransmit_isFull( const CircularQueueTransmit_t *const pQueue )
{
    bool retVal = false;

    if( pQueue != NULL )
    {
        if( (pQueue -> capacity) == (pQueue -> size) )
        {
            retVal = true;
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

    return retVal;
}

/**
  * @brief  Checks if the transmit queue is empty.
  * @param  None
  * @retval true if the queue is empty, false otherwise.
  */
static bool QueueTransmit_isEmpty( const CircularQueueTransmit_t *const pQueue )
{
    bool retVal = false;

    if( pQueue != NULL )
    {
        if( pQueue->capacity == 0U )
        {
            retVal = true;
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

    return retVal;
}
