/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "Queue_Receive.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Initialize for QueueReceive */
void QueueReceive_Init( CircularQueueReceive_t *const pQueue )
{
    if( pQueue != NULL )
    {
        pQueue->capacity = 0U;
        pQueue->front    = -1;
        pQueue->rear     = -1;
        pQueue->size     = QUEUE_RECEIVE_SIZE;
    }
    else
    {
        /* Do Nothing */
    }
}

/* Check if QueueReceive is full */
uint8_t QueueReceive_isFull( const CircularQueueReceive_t *const pQueue )
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

/* Check if QueueReceive is empty */
uint8_t QueueReceive_isEmpty( const CircularQueueReceive_t *const pQueue )
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

/* Handling push data into the QueueReceive */
QueueCheckOperation_t QueueReceive_enQueue( CircularQueueReceive_t *const pQueue,
                                            const ReceiveFrame_t *const pFrame )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( (pQueue != NULL) && (pFrame != NULL) )
    {
        if ( !QueueReceive_isFull(pQueue) )
        {
            if ( pQueue->front == -1 )
            {
                pQueue->front = 0;
            }
            else
            {
                /* Do Nothing */
            }
            pQueue->rear = ((pQueue->rear) + 1) % (pQueue->size);
            pQueue->queueArray[pQueue->rear] = *pFrame;
            (pQueue->capacity)++;
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

/* Handling pop data from the QueueReceive */
QueueCheckOperation_t QueueReceive_deQueue( CircularQueueReceive_t *pQueue,
                                            ReceiveFrame_t *pFrame )
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( (pQueue != NULL) && (pFrame != NULL) )
    {
        if ( !QueueReceive_isEmpty(pQueue) )
        {
            *pFrame = pQueue->queueArray[pQueue->front];
            (pQueue->capacity)--;
            if (pQueue->front == pQueue->rear)
            {
                pQueue->front = -1;
                pQueue->rear  = -1;
            }
            else
            {
                pQueue->front = ((pQueue->front) + 1) % (pQueue->size);
            }
            status = QUEUE_DONE_SUCCESS;
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
