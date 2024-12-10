#ifndef MID_CLOCK_INTERFACE_H_
#define MID_CLOCK_INTERFACE_H_

/*******************************************************************************
 * Include
 ******************************************************************************/

/*******************************************************************************
 * Definition
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

/**
 * @brief Initializes the clock module.
 *
 * This function is responsible for setting up and
 * initializing the clock system hardware. It ensures
 * that the clock is configured correctly and is ready
 * to be used by other parts of the system.
 *
 * @note This function should be called during the
 *       system initialization phase.
 *
 * @return void
 */
void MID_Clock_Init(void);

#endif /* MID_CLOCK_INTERFACE_H_ */
