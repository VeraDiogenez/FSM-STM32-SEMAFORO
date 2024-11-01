/*
 * edge_detector.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#include "edge_detector.h"

// Condition functions for edge detection FSM
static int is_high(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_SET;
}

static int is_low(void *context) {
    EdgeDetector *edge_detector = (EdgeDetector *)context;
    return get_debounced_switch_state(edge_detector->switch_instance) == GPIO_PIN_RESET;
}

// on_state functions for each state
void on_state_idle_high(void *context) {
    // No specific action required for idle_high
}

void on_state_idle_low(void *context) {
    // No specific action required for idle_low
}

void on_state_rising_edge(void *context) {
    // Edge detected, execute any necessary actions (logging, etc.)
}

void on_state_falling_edge(void *context) {
    // Edge detected, execute any necessary actions (logging, etc.)
}

// Transition arrays for each state
static Transition IdleHighTransitions[] = {
    {is_low, FALLING_EDGE}   // Transition to FALLING_EDGE on low input
};

static Transition IdleLowTransitions[] = {
    {is_high, RISING_EDGE}   // Transition to RISING_EDGE on high input
};

static Transition RisingEdgeTransitions[] = {
    {is_high, IDLE_HIGH},    // Transition to IDLE_HIGH on high input
    {is_low, FALLING_EDGE}   // Transition to FALLING_EDGE on low input
};

static Transition FallingEdgeTransitions[] = {
    {is_low, IDLE_LOW},      // Transition to IDLE_LOW on low input
    {is_high, RISING_EDGE}   // Transition to RISING_EDGE on high input
};

// FSM states with action functions
static FSMState EdgeFSMStates[] = {
    {IdleHighTransitions, 1, on_state_idle_high},         // IDLE_HIGH state
    {IdleLowTransitions, 1, on_state_idle_low},           // IDLE_LOW state
    {RisingEdgeTransitions, 2, on_state_rising_edge},     // RISING_EDGE state
    {FallingEdgeTransitions, 2, on_state_falling_edge}    // FALLING_EDGE state
};

// Initialize the edge detector
void edge_detector_init(EdgeDetector *edge_detector, DebouncedSwitch *switch_instance) {
    fsm_init(&edge_detector->fsm, EdgeFSMStates, IDLE_LOW, edge_detector);  // Start in IDLE_LOW
    edge_detector->switch_instance = switch_instance;
}

// Update the edge detector FSM
void edge_detector_update(EdgeDetector *edge_detector) {
    fsm_update(&edge_detector->fsm);
}

// Get the current state based on the edge detector FSM state
EdgeFSMState get_edge_detector_state(EdgeDetector *edge_detector) {
    return (EdgeFSMState)edge_detector->fsm.currentState;
}
