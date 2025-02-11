#ifndef QUEUE_COMMON_H_
#define QUEUE_COMMON_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*******************************************************************************
 * Defines
 ******************************************************************************/

#define QUEUE_RECEIVE_SIZE   22u
#define TRANSMIT_QUEUE_SIZE  500u

/*******************************************************************************
 * Typedef structs
 ******************************************************************************/
typedef enum
{
    QUEUE_DONE_SUCCESS,
    QUEUE_DONE_FAILED
} QueueCheckOperation_t;

/* Structure of a element in  receive Queue */
typedef struct
{
    uint8_t  ID;
    uint16_t Data;
} ReceiveFrame_t;

/* Circular queue for receiving frames */
typedef struct
{
    int16_t        front;
    int16_t        rear;
    uint16_t       size;
    ReceiveFrame_t queueArray[QUEUE_RECEIVE_SIZE];
    uint16_t       capacity;
} CircularQueueReceive_t;

/* Circular queue for sending UART frames */
typedef struct
{
    int16_t  front;
    int16_t  rear;
    int16_t  size;
    uint8_t  queueArray[TRANSMIT_QUEUE_SIZE];
    uint16_t capacity;
}CircularQueueTransmit_t;

#endif /* QUEUE_COMMON_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
