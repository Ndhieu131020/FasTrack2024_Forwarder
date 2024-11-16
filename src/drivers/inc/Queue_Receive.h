#ifndef QUEUE_RECEIVE_H_
#define QUEUE_RECEIVE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "Queue_Common.h"

/*******************************************************************************
 * APIs
 ******************************************************************************/
/* Initialize for QueueReceive */
void QueueReceive_Init( CircularQueueReceive_t *const pQueue );

/* Check full of queue */
uint8_t QueueReceive_isFull( const CircularQueueReceive_t *const pQueue );

/* Check empty of queue */
uint8_t QueueReceive_isEmpty( const CircularQueueReceive_t *const pQueue );

QueueCheckOperation_t QueueReceive_enQueue( CircularQueueReceive_t *const pQueue,
                                            const ReceiveFrame_t *const pFrame );

QueueCheckOperation_t QueueReceive_deQueue( CircularQueueReceive_t *pQueue,
                                            ReceiveFrame_t *pFrame );

#endif /* QUEUE_RECEIVE_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/
