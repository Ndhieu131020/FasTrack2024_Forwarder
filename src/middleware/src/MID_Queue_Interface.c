/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "MID_Queue_Interface.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* The Queue is used to store all received data */
static CircularQueueReceive_t receiveQueue;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Check if QueueReceive is full */
static uint8_t QueueReceive_isFull( const CircularQueueReceive_t *const pQueue );

/* Check if QueueReceive is empty */
static uint8_t QueueReceive_isEmpty( const CircularQueueReceive_t *const pQueue );

/* Initialize for QueueReceive */
void MID_Receive_Queue_Init( void )
{
    receiveQueue.front    = -1;
    receiveQueue.rear     = -1;
    receiveQueue.capacity = 0U;
    receiveQueue.size     = QUEUE_RECEIVE_SIZE;
}


QueueCheckOperation_t MID_Receive_EnQueue( const ReceiveFrame_t *const pInData )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pInData != NULL )
    {
        if ( !QueueReceive_isFull(&receiveQueue) )
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


QueueCheckOperation_t MID_Receive_DeQueue( ReceiveFrame_t *const pOutData )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pOutData !=NULL )
    {
        if ( !QueueReceive_isEmpty(&receiveQueue) )
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


static uint8_t QueueReceive_isFull( const CircularQueueReceive_t *const pQueue )
{
    uint8_t status = FALSE;

    if( pQueue != NULL )
    {
        if( (pQueue -> capacity) == (pQueue -> size) )
        {
            status = TRUE;
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


static uint8_t QueueReceive_isEmpty( const CircularQueueReceive_t *const pQueue )
{
    uint8_t status = FALSE;

    if( pQueue != NULL )
    {
        if( pQueue->capacity == 0U )
        {
            status = TRUE;
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
