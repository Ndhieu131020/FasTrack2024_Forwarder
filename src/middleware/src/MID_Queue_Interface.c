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

void MID_Receive_Queue_Init( void )
{
    QueueReceive_Init(&receiveQueue);
}


QueueCheckOperation_t MID_Receive_EnQueue( const ReceiveFrame_t *const pInData )
{
	QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if( pInData != NULL )
    {
    	status = QueueReceive_enQueue(&receiveQueue, pInData);
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
        status = QueueReceive_deQueue(&receiveQueue, pOutData);
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
