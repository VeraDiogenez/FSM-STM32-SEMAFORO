/*
 * blink_control.c
 *
 *  Created on: Oct 2, 2024
 *      Author: apojo
 */

#include "blink_control.h"
#include "timer.h"
#include "main.h"

// Funciones de condición para las transiciones de estado
static int timer_expired(void *context) {
    BlinkControl *blink_control = (BlinkControl *)context;
    return timer_has_expired(&blink_control->blink_timer);
}

// Funciones de acción para cada estado
void on_state_led_off(void *context) {
    BlinkControl *blink_control = (BlinkControl *)context;
    HAL_GPIO_WritePin(blink_control->LED_Port, blink_control->LED_Pin, GPIO_PIN_SET);  // Apagar LED
    timer_restart(&blink_control->blink_timer);
}

void on_state_led_on(void *context) {
    BlinkControl *blink_control = (BlinkControl *)context;
    HAL_GPIO_WritePin(blink_control->LED_Port, blink_control->LED_Pin, GPIO_PIN_RESET);  // Encender LED
    timer_restart(&blink_control->blink_timer);
}

// Arreglos de transición para cada estado
static Transition LEDOffTransitions[] = {
    {timer_expired, LED_ON}  // Transición de OFF a ON cuando el temporizador expira
};

static Transition LEDOnTransitions[] = {
    {timer_expired, LED_OFF}  // Transición de ON a OFF cuando el temporizador expira
};

// Estados de la FSM con acciones
static FSMState BlinkFSMStates[] = {
    {LEDOffTransitions, 1, on_state_led_off},           // Estado LED_OFF
    {LEDOnTransitions, 1, on_state_led_on}              // Estado LED_ON
};

// Inicializar la FSM de BlinkControl
void blink_control_init(BlinkControl *blink_control, GPIO_TypeDef *LED_Port, uint16_t LED_Pin, uint32_t initial_period) {
    fsm_init(&blink_control->fsm, BlinkFSMStates, LED_OFF, blink_control);  // Iniciar FSM en LED_OFF
    blink_control->LED_Port = LED_Port;
    blink_control->LED_Pin = LED_Pin;
    timer_start(&blink_control->blink_timer, initial_period);  // Iniciar temporizador de parpadeo
}

// Actualizar la FSM
void blink_control_update(BlinkControl *blink_control) {
    fsm_update(&blink_control->fsm);
}

// Establecer el período de parpadeo
void set_blink_period(BlinkControl *blink_control, uint32_t period) {
    timer_update_duration(&blink_control->blink_timer, period);
}

