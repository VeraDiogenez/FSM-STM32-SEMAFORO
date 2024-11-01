/*
 * debounced_switch.c
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#include <debouncer.h>
#include "timer.h"

// Condition functions for state transitions
static int is_button_pressed(void *context) {
    DebouncedSwitch *debounced_switch = (DebouncedSwitch *)context;
    return HAL_GPIO_ReadPin(debounced_switch->GPIOx, debounced_switch->GPIO_Pin) == GPIO_PIN_SET;}

static int is_button_pressed_and_timer_expired(void *context) {
    DebouncedSwitch *debounced_switch = (DebouncedSwitch *)context;
    return timer_has_expired(&debounced_switch->debounce_timer) && HAL_GPIO_ReadPin(debounced_switch->GPIOx, debounced_switch->GPIO_Pin) == GPIO_PIN_SET;}

static int is_button_released_and_timer_expired(void *context) {
    DebouncedSwitch *debounced_switch = (DebouncedSwitch *)context;
    return timer_has_expired(&debounced_switch->debounce_timer) && HAL_GPIO_ReadPin(debounced_switch->GPIOx, debounced_switch->GPIO_Pin) == GPIO_PIN_RESET;
}

// Transition arrays for each state
static Transition IdleTransitions[] = {
    {is_button_pressed, SWITCH_PRESSED}
};

static Transition PressedTransitions[] = {
    {is_button_released_and_timer_expired, SWITCH_RELEASED}
};

static Transition ReleasedTransitions[] = {
    {is_button_pressed_and_timer_expired, SWITCH_PRESSED}
};


// Action functions
void on_state_pressed(void *context) {
    DebouncedSwitch *debounced_switch = (DebouncedSwitch *)context;
    timer_start(&debounced_switch->debounce_timer, 50);  // Start debounce timer
}

void on_state_released(void *context) {
    DebouncedSwitch *debounced_switch = (DebouncedSwitch *)context;
    timer_start(&debounced_switch->debounce_timer, 50);  // Start debounce timer
}

// FSM states with actions
static FSMState DebouncedSwitchFSM[] = {
    {IdleTransitions, 1, NULL},                 // SWITCH_IDLE state, no action function is needed here, so we pass NULL
    {PressedTransitions, 1, on_state_pressed},  // SWITCH_PRESSED state
    {ReleasedTransitions, 1, on_state_released} // SWITCH_RELEASED state
};

// Initialize the debounced switch FSM
void debounced_switch_init(DebouncedSwitch *debounced_switch, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    fsm_init(&debounced_switch->fsm, DebouncedSwitchFSM, SWITCH_IDLE, debounced_switch);  // Pass context
    debounced_switch->GPIOx = GPIOx;
    debounced_switch->GPIO_Pin = GPIO_Pin;
    timer_start(&debounced_switch->debounce_timer, 50);  // Example: Start debounce timer for 50ms
}

// Update the FSM
void debounced_switch_update(DebouncedSwitch *debounced_switch) {
    // Perform state transitions
    fsm_update(&debounced_switch->fsm);
}

// Get the debounced GPIO state
GPIO_PinState get_debounced_switch_state(DebouncedSwitch *debounced_switch) {
    return (debounced_switch->fsm.currentState == SWITCH_PRESSED) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}
