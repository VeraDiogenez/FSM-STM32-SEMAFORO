/*
 * edge_detector.h
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#ifndef INC_EDGE_DETECTOR_H_
#define INC_EDGE_DETECTOR_H_

#include <debouncer.h>
#include "fsm.h"

// Enum for the edge detector states
typedef enum {
    IDLE_HIGH,      // Idle with input high
    IDLE_LOW,       // Idle with input low
    RISING_EDGE,    // Rising edge detected
    FALLING_EDGE    // Falling edge detected
} EdgeFSMState;


typedef struct {
    FSM fsm;               // FSM instance for edge detection
    DebouncedSwitch *switch_instance;  // Reference to the debounced switch
} EdgeDetector;

// PUBLIC FUNCTIONS

/**
 * @brief Initializes the edge detector FSM.
 *
 * @param edge_detector Pointer to the EdgeDetector structure.
 * @param switch_instance Pointer to the debounced switch instance.
 */
void edge_detector_init(EdgeDetector *edge_detector, DebouncedSwitch *switch_instance);

/**
 * @brief Updates the edge detector FSM to detect rising or falling edges.
 *
 * @param edge_detector Pointer to the EdgeDetector structure.
 */
void edge_detector_update(EdgeDetector *edge_detector);

/**
 * @brief Gets the last detected edge.
 *
 * @param edge_detector Pointer to the EdgeFSM structure.
 * @return EdgeFSMState The current FSM state.
 */
EdgeFSMState get_edge_detector_state(EdgeDetector *edge_detector);

#endif /* INC_EDGE_DETECTOR_H_ */

