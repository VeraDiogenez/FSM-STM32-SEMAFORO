/*
 * timer.c
 *
 *  Created on: Oct 31, 2024
 *      Author: Dell_G7A
 */

/*
 * timer.c
 *
 *  Created on: Sep 30, 2024
 *      Author: apojo
 */


#include "main.h"
#include "timer.h"

/**
 * @brief Starts a timer for the specified duration in milliseconds.
 */
void timer_start(Timer *timer, uint32_t duration_ms) {
    timer->start_tick = HAL_GetTick();    // Store the current tick
    timer->duration_ms = duration_ms; // Set the timeout duration
}

/**
 * @brief Restarts a timer.
 */
void timer_restart(Timer *timer) {
    timer->start_tick = HAL_GetTick();
}

/**
 * @brief Checks if the timer has expired.
 *
 * @return 1 if the timer has expired, 0 if not.
 */
int timer_has_expired(Timer *timer) {
    return (HAL_GetTick() - timer->start_tick >= timer->duration_ms);
}

/**
 * @brief Updates the timer duration without resetting the start tick.
 */
void timer_update_duration(Timer *timer, uint32_t new_duration_ms) {
    timer->duration_ms = new_duration_ms;
}
