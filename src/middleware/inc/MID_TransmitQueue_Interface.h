#ifndef MID_TRANSMITQUEUE_INTERFACE_H_
#define MID_TRANSMITQUEUE_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/
#include "Queue_Common.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
  * @brief  Initializes the transmit queue by setting initial values
  *         for the Front, Rear, Size, and clearing the Data buffer.
  * @param  None
  * @retval None
  */
void MID_Transmit_Queue_Init(void);

/**
  * @brief  Adds a data byte to the transmit queue.
  * @param[in] data Data byte to enqueue.
  * @retval QUEUE_DONE_SUCCESS if enqueue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is full.
  */
QueueCheckOperation_t MID_Transmit_Enqueue(const uint8_t data);

/**
  * @brief  Retrieves and removes the data byte from the front of the queue.
  * @param[out] data Pointer to store dequeued data.
  * @retval QUEUE_DONE_SUCCESS if dequeue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is empty.
  */
QueueCheckOperation_t MID_Transmit_Dequeue(uint8_t * pOutData);

#endif /* MID_TRANSMITQUEUE_INTERFACE_H_ */
