/*
 * fsm.c
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#include "fsm.h"
#include "main.h"

/**
 * @brief Initializes the FSM with given states, initial state, and context.
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state, void *context) {
    fsm->states = states;
    fsm->currentState = initial_state;
    fsm->context = context;
}

/**
 * @brief Updates the FSM based on the conditions of the current state.
 */
void fsm_update(FSM *fsm) {
    unsigned long oldState = fsm->currentState;  // Store the current state before checking transitions
    FSMState currentState = fsm->states[fsm->currentState];

    // Check all transitions (one for each of the potential next states)
    for (unsigned long i = 0; i < currentState.numTransitions; i++) {
        if (currentState.transitions[i].condition(fsm->context)) {  // Pass the context to the condition function
            fsm->currentState = currentState.transitions[i].nextState;  // Transition to the corresponding next state
            break;  // Exit after the first matched condition
        }
    }

    // Check if the state has changed
    if (fsm->currentState != oldState) {
        FSMState newState = fsm->states[fsm->currentState];
        if (newState.action) {
            newState.action(fsm->context);  // Execute the action on state entry
        }
    }
}
