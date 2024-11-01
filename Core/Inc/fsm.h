/*
 * fsm.h
 *
 *  Created on: Sep 26, 2024
 *      Author: apojo
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include <stdint.h>

// Define a function pointer type for condition functions with context
// This function evaluate the conditions that determine whether the FSM will
// transition from the current state to another. It evaluates a boolean statement.
typedef int (*ConditionFunc)(void *context);

// Define a function pointer type for action functions with context
// This function executes the actions required by a certain state.
typedef void (*ActionFunc)(void *context);

// Define a Transition structure to pair conditions with next states
typedef struct {
    ConditionFunc condition;  // Condition function
    unsigned long nextState;  // Corresponding next state if the condition is true
} Transition;


// This structure defines an FSM state
// Given we could potentially transition from this state to various different states,
// it accepts multiple transitions in the form of an array.
// It also defines an action, which can either point to a valid function or to NULL,
// if no action is needed for the state.
typedef struct {
    Transition *transitions;      // Array of transitions (condition -> next state)
    unsigned long numTransitions; // Number of transitions
    ActionFunc action;            // Action to be executed on entering the state
} FSMState;

// This structure defines an FSM
// An FSM is modeled by the states that compose it, along with its context.
// The context is a void*, which means it can be a pointer to any structure you choose.
// We will use this to avoid having to use global variables to query and modify the
// state of the world from within the condition functions of a transition,
// and action functions of each state, respectively.
typedef struct {
    const FSMState *states;
    unsigned long currentState;
    void *context;  // Context to pass to condition and action functions
} FSM;


// PUBLIC FUNCTIONS

/**
 * @brief Initializes the FSM.
 *
 * @param fsm Pointer to FSM structure.
 * @param states Array of FSMState that defines the FSM.
 * @param initial_state The starting state of the FSM.
 * @param context A pointer to the FSM's context (e.g., DebouncedSwitch instance).
 */
void fsm_init(FSM *fsm, const FSMState *states, unsigned long initial_state, void *context);

/**
 * @brief Updates the FSM based on evaluated conditions.
 *
 * @param fsm Pointer to FSM structure.
 */
void fsm_update(FSM *fsm);

#endif /* INC_FSM_H_ */

