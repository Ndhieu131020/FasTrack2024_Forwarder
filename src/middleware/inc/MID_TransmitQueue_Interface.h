/*
 *  Filename: MID_TransmitQueue_Interface.h
 *
 *  Created on: 11-17-2024
 *      Author: Ndhieu131020@gmail.com
*/
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
  * @brief  Checks if the transmit queue is full.
  * @param  None
  * @retval True if the queue is full, false otherwise.
  */
bool MID_Transmit_Queue_isFull(void);

/**
  * @brief  Checks if the transmit queue is empty.
  * @param  None
  * @retval true if the queue is empty, false otherwise.
  */
bool MID_Transmit_Queue_isEmpty(void);

/**
  * @brief  Adds a data byte to the transmit queue.
  * @param[in] data Data byte to enqueue.
  * @retval QUEUE_DONE_SUCCESS if enqueue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is full.
  */
QueueCheckOperation_t MID_Transmit_Queue_Enqueue(uint8_t data);

/**
  * @brief  Retrieves and removes the data byte from the front of the queue.
  * @param[out] data Pointer to store dequeued data.
  * @retval QUEUE_DONE_SUCCESS if dequeue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is empty.
  */
QueueCheckOperation_t MID_Transmit_Queue_Dequeue(uint8_t *data);

#endif /* MID_TRANSMITQUEUE_INTERFACE_H_ */