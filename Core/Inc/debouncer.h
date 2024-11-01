/*
 * debounced_switch.h
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#ifndef INC_DEBOUNCER_H_
#define INC_DEBOUNCER_H_

#include "fsm.h"
#include "timer.h"

typedef enum {
    SWITCH_IDLE,
    SWITCH_PRESSED,
    SWITCH_RELEASED
} SwitchState;

// Structure to hold the debounced switch FSM and associated data
typedef struct {
    FSM fsm;               // FSM instance
    Timer debounce_timer;   // Timer for debouncing
    GPIO_TypeDef *GPIOx;    // GPIO port for the switch
    uint16_t GPIO_Pin;      // GPIO pin for the switch
} DebouncedSwitch;

// PUBLIC FUNCTIONS

/**
 * @brief Initializes the debounced switch FSM.
 *
 * @param debounced_switch Pointer to DebouncedSwitch structure.
 * @param GPIOx GPIO port of the switch.
 * @param GPIO_Pin GPIO pin of the switch.
 */
void debounced_switch_init(DebouncedSwitch *debounced_switch, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief Updates the debounced switch FSM.
 *
 * @param debounced_switch Pointer to DebouncedSwitch structure.
 */
void debounced_switch_update(DebouncedSwitch *debounced_switch);

/**
 * @brief Gets the debounced GPIO state.
 *
 * @param debounced_switch Pointer to DebouncedSwitch structure.
 * @return GPIO_PIN_SET or GPIO_PIN_RESET based on the debounced state.
 */
GPIO_PinState get_debounced_switch_state(DebouncedSwitch *debounced_switch);

#endif /* INC_DEBOUNCER_H_ */
