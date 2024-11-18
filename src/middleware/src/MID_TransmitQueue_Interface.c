/*
 *  Filename: MID_TransmitQueue_Interface.c
 *
 *  Created on: 11-17-2024
 *      Author: Ndhieu131020@gmail.com
*/
#include "MID_TransmitQueue_Interface.h"

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Static instance of the queue structure used for transmit operations */
static Transmit_Queue_Typedef QUEUE_TypedefStructure;

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
  * @brief  Initializes the transmit queue by setting initial values
  *         for the Front, Rear, Size, and clearing the Data buffer.
  * @param  None
  * @retval None
  */
void MID_Transmit_Queue_Init(void)
{
    int16_t index = 0u;

    QUEUE_TypedefStructure.Front    = -1;
    QUEUE_TypedefStructure.Rear     = -1;
    QUEUE_TypedefStructure.Capacity = 0u;
    QUEUE_TypedefStructure.Size     = TRANSMIT_QUEUE_SIZE;

    for (index = 0; index < TRANSMIT_QUEUE_SIZE; index++)
    {
        QUEUE_TypedefStructure.Data[index] = 0u;
    }
}

/**
  * @brief  Checks if the transmit queue is full.
  * @param  None
  * @retval True if the queue is full, false otherwise.
  */
bool MID_Transmit_Queue_isFull(void)
{
    bool retVal = false;
    /* Check if Front pointer is just ahead of Rear pointer or wraps around */
    if((QUEUE_TypedefStructure.Front == QUEUE_TypedefStructure.Rear + 1) || (QUEUE_TypedefStructure.Front == 0 && QUEUE_TypedefStructure.Rear == QUEUE_TypedefStructure.Size -1))
    {
        retVal = true;
    }

    return retVal;
}

/**
  * @brief  Checks if the transmit queue is empty.
  * @param  None
  * @retval true if the queue is empty, false otherwise.
  */
bool MID_Transmit_Queue_isEmpty(void)
{
    bool retVal = false;

    /* Queue is empty if Front pointer is -1 */
    if(QUEUE_TypedefStructure.Front == -1)
    {
        retVal = true;
    }

    return retVal;
}

/**
  * @brief  Adds a data byte to the transmit queue.
  * @param[in] data Data byte to enqueue.
  * @retval QUEUE_DONE_SUCCESS if enqueue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is full.
  */
QueueCheckOperation_t MID_Transmit_Queue_Enqueue(uint8_t data)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if(!MID_Transmit_Queue_isFull())
    {
        if(QUEUE_TypedefStructure.Front == -1)
        {
            QUEUE_TypedefStructure.Front = 0;
        }

        /* Increase rear pointer */
        QUEUE_TypedefStructure.Rear = (QUEUE_TypedefStructure.Rear + 1) % QUEUE_TypedefStructure.Size;
        (QUEUE_TypedefStructure.Capacity)++;

        QUEUE_TypedefStructure.Data[QUEUE_TypedefStructure.Rear] = data;

        status = QUEUE_DONE_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}

/**
  * @brief  Retrieves and removes the data byte from the front of the queue.
  * @param[out] data Pointer to store dequeued data.
  * @retval QUEUE_DONE_SUCCESS if dequeue operation is successful.
  * @retval QUEUE_DONE_FAILED if the queue is empty.
  */
QueueCheckOperation_t MID_Transmit_Queue_Dequeue(uint8_t *data)
{
    QueueCheckOperation_t status = QUEUE_DONE_FAILED;

    if(!MID_Transmit_Queue_isEmpty())
    {
        *data = QUEUE_TypedefStructure.Data[QUEUE_TypedefStructure.Front];
        (QUEUE_TypedefStructure.Capacity)--;
        
        if(QUEUE_TypedefStructure.Front == QUEUE_TypedefStructure.Rear)
        {
            QUEUE_TypedefStructure.Front = -1;
            QUEUE_TypedefStructure.Rear = -1;
        }
        else
        {
            QUEUE_TypedefStructure.Front = (QUEUE_TypedefStructure.Front + 1) % QUEUE_TypedefStructure.Size;
        }
        status = QUEUE_DONE_SUCCESS;
    }
    else
    {
        /* Do Nothing */
    }

    return status;
}