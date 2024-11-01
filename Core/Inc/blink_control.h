/*
 * blink_control.h
 *
 *  Created on: Oct 2, 2024
 *      Author: apojo
 */

#ifndef INC_BLINK_CONTROL_H_
#define INC_BLINK_CONTROL_H_

#include "fsm.h"
#include "timer.h"

// Blink states for the FSM
typedef enum {
    LED_OFF,
    LED_ON
} BlinkState;

// Structure to hold the BlinkControl FSM and associated data
typedef struct {
    FSM fsm;               // FSM instance for LED blinking
    Timer blink_timer;      // Timer for LED blink period
    GPIO_TypeDef *LED_Port; // GPIO port for the LED
    uint16_t LED_Pin;       // GPIO pin for the LED
} BlinkControl;

// PUBLIC FUNCTIONS

/**
 * @brief Initializes the BlinkControl FSM.
 *
 * @param blink_control Pointer to BlinkControl structure.
 * @param LED_Port GPIO port of the LED.
 * @param LED_Pin GPIO pin of the LED.
 * @param initial_period Initial blink period in milliseconds.
 */
void blink_control_init(BlinkControl *blink_control, GPIO_TypeDef *LED_Port, uint16_t LED_Pin, uint32_t initial_period);

/**
 * @brief Updates the BlinkControl FSM.
 *
 * @param blink_control Pointer to BlinkControl structure.
 */
void blink_control_update(BlinkControl *blink_control);

/**
 * @brief Sets the blink period for the BlinkControl.
 *
 * @param blink_control Pointer to BlinkControl structure.
 * @param period New blink period in milliseconds.
 */
void set_blink_period(BlinkControl *blink_control, uint32_t period);


#endif /* INC_BLINK_CONTROL_H_ */
