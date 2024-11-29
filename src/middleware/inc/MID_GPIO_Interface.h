/*
 *  Filename: MID_GPIO_Interface.h
 *
 *  Created on: 11-15-2024
 *      Author: luanduong1209@gmail.com
*/

#ifndef MID_GPIO_INTERFACE_H_
#define MID_GPIO_INTERFACE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/**
 * @brief An enumeration of the colors of the LEDs available on the board.
 *
 * This enumeration lists the colors of the LEDs available on the board. The
 * LED colors are LED_RED, LED_GREEN, and LED_BLUE. Each LED color is associated
 * with a specific GPIO pin on the board. The specific pin associated with each
 * color is defined in the BSP.
 */
typedef enum
{
    LED_RED,
    LED_GREEN,
    LED_BLUE
} led_color_t;

/*******************************************************************************
 * APIs
 ******************************************************************************/

/**
 * @brief Initializes the LED module by setting the LED pin as output
 *
 * This function initializes the LED module by setting the LED pin as output.
 * It should be called once before any other LED-related APIs are used.
 *
 * @note This function does not turn on the LED. To turn on the LED, use the
 *       API MID_TurnOnLed() after calling this function.
 */
void MID_Led_Init(void);

/**
 * @brief Turns on the specified LED
 *
 * This function turns on the specified LED.
 *
 * @param[in] ledtype The LED type to turn on. The supported LED types are
 *                    LED_RED, LED_GREEN, and LED_BLUE.
 *
 * @note This function assumes that the LED module has been initialized by
 *       calling MID_Led_Init() before calling this function.
 */
void MID_TurnOnLed(led_color_t ledtype);

/**
 * @brief Turns off the specified LED
 *
 * This function turns off the specified LED.
 *
 * @param[in] ledtype The LED type to turn off. The supported LED types are
 *                    LED_RED, LED_GREEN, and LED_BLUE.
 *
 * @note This function assumes that the LED module has been initialized by
 *       calling MID_Led_Init() before calling this function.
 */
void MID_TurnOffLed(led_color_t ledtype);

/**
 * @brief Toggles the specified LED on or off
 *
 * This function toggles the specified LED on or off. If the LED is currently
 * off, it will be turned on. If the LED is currently on, it will be turned off.
 *
 * @param[in] ledtype The LED type to toggle. The supported LED types are
 *                    LED_RED, LED_GREEN, and LED_BLUE.
 *
 * @note This function assumes that the LED module has been initialized by
 *       calling MID_Led_Init() before calling this function.
 */
void MID_ToggleLed(led_color_t ledtype);

#endif /* MID_GPIO_INTERFACE_H_ */
