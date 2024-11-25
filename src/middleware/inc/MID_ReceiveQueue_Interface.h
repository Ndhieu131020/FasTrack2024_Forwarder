#ifndef MID_RECEIVEQUEUE_INTERFACE_H_
#define MID_RECEIVEQUEUE_INTERFACE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "Queue_Common.h"

/*******************************************************************************
 * APIs
 ******************************************************************************/

/**
  * @brief      Initialize for QueueReceive
  * @param[in]  None
  * @param[out] None
  * @retval     None
  */
void MID_Receive_Queue_Init( void );

/**
  * @brief      Add a data frame to the receive queue
  * @param[in]  pInData: Pointer to the input data frame
  * @param[out] None
  * @retval     QUEUE_DONE_SUCCESS if enqueue operation is successful, otherwise QUEUE_DONE_FAILED
  */
QueueCheckOperation_t MID_Receive_EnQueue( const ReceiveFrame_t *const pInData );

/**
  * @brief      Remove a data frame from the receive queue
  * @param[in]  None
  * @param[out] pOutData: Pointer to the output data frame
  * @retval     QUEUE_DONE_SUCCESS if dequeue operation is successful, otherwise QUEUE_DONE_FAILED
  */
QueueCheckOperation_t MID_Receive_DeQueue( ReceiveFrame_t *const pOutData );

#endif /* MID_RECEIVEQUEUE_INTERFACE_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
