/*
 * timer.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Dell_G7A
 */

/*
 * timer.h
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

/**
 * @brief Timer structure.
 */
typedef struct {
    uint32_t start_tick;
    uint32_t duration_ms;
} Timer;

// PUBLIC FUNCTIONS

/**
 * @brief Starts a timer for the specified duration in milliseconds.
 *
 * @param timer Pointer to the Timer structure.
 * @param duration_ms Duration of the timer in milliseconds.
 */
void timer_start(Timer *timer, uint32_t duration_ms);

/**
 * @brief Restarts a timer.
 *
 * @param timer Pointer to the Timer structure.
 */
void timer_restart(Timer *timer);

/**
 * @brief Checks if the timer has expired.
 *
 * @param timer Pointer to the Timer structure.
 * @return 1 if the timer has expired, 0 if not.
 */
int timer_has_expired(Timer *timer);

/**
 * @brief Updates the timer duration without resetting the start tick.
 *
 * @param timer Pointer to the Timer structure.
 * @param new_duration_ms New duration for the timer in milliseconds.
 */
void timer_update_duration(Timer *timer, uint32_t new_duration_ms);

#endif /* INC_TIMER_H_ */
