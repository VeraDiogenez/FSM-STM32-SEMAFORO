#ifndef INC_SEMAFORO_PEATONAL_H_
#define INC_SEMAFORO_PEATONAL_H_

#include "fsm.h"
#include "timer.h"
#include "blink_control.h"
#include "debouncer.h"         // Cambiado a debouncer.h
#include "edge_detector.h"
#include "main.h"

// Definición de estados de la FSM del semáforo peatonal
typedef enum {
    VERDE,
    VERDE_PARPADEO,
    ROJO,
    ROJO_PARPADEO
} SemaforoPeatonalState;

// Estructura de control para el semáforo peatonal
typedef struct {
    FSM fsm;                    // FSM para el control del semáforo
    Timer timer;                // Temporizador para transiciones de estado
    BlinkControl green_blink;   // Control de parpadeo para el LED verde
    BlinkControl red_blink;     // Control de parpadeo para el LED rojo
} SemaforoPeatonal;

// Funciones públicas

/**
 * @brief Inicializa la FSM del semáforo peatonal.
 *
 * @param semaforo Puntero a la estructura SemaforoPeatonal.
 */
void semaforo_peatonal_init(SemaforoPeatonal *semaforo);

/**
 * @brief Actualiza la FSM del semáforo peatonal.
 *
 * Esta función debe ser llamada repetidamente en el bucle principal.
 *
 * @param semaforo Puntero a la estructura SemaforoPeatonal.
 */
void semaforo_peatonal_update(SemaforoPeatonal *semaforo);

#endif /* INC_SEMAFORO_PEATONAL_H_ */

