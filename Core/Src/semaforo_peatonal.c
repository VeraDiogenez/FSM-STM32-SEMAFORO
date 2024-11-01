#include "semaforo_peatonal.h"
#include "timer.h"
#include "blink_control.h"
#include "debouncer.h"
#include "edge_detector.h"
#include <stdio.h>

// Pines para LEDs y botón
#define GREEN_LED_PORT GPIOB
#define GREEN_LED_PIN GPIO_PIN_12
#define RED_LED_PORT GPIOC
#define RED_LED_PIN GPIO_PIN_13
#define BUTTON_PORT GPIOB
#define BUTTON_PIN GPIO_PIN_5

// Variables globales para el botón con antirrebote y el detector de bordes
DebouncedSwitch button_switch;
EdgeDetector edge_detector;

// Función de condición para transiciones
static int button_pressed(void *context) {
    // Asegúrate de que solo detectas el borde de subida del botón
    return get_edge_detector_state(&edge_detector) == RISING_EDGE;
}


static int timer_expired(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    return timer_has_expired(&semaforo->timer);
}

// Funciones de acción para cada estado
void on_state_verde(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_RESET);  // LED verde encendido
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_SET);        // LED rojo apagado
    set_blink_period(&semaforo->green_blink, 0);                       // Detener parpadeo del verde
    set_blink_period(&semaforo->red_blink, 0);                         // Detener parpadeo del rojo
}



void on_state_verde_parpadeo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    set_blink_period(&semaforo->green_blink, 200);                     // Parpadeo del LED verde a 5 Hz
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_SET);        // Asegurarse de que el LED rojo esté apagado
    set_blink_period(&semaforo->red_blink, 0);                         // Asegurarse de detener el parpadeo del LED rojo
    timer_update_duration(&semaforo->timer, 1000);                     // Temporizador de 1 segundo
}

void on_state_rojo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);    // LED verde apagado
    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, GPIO_PIN_RESET);      // LED rojo encendido
    set_blink_period(&semaforo->green_blink, 0);                       // Detener parpadeo del verde
    set_blink_period(&semaforo->red_blink, 0);                         // Detener parpadeo del rojo
}

void on_state_rojo_parpadeo(void *context) {
    SemaforoPeatonal *semaforo = (SemaforoPeatonal *)context;
    set_blink_period(&semaforo->red_blink, 200);                       // Parpadeo del LED rojo a 5 Hz
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, GPIO_PIN_SET);    // Asegurarse de que el LED verde esté apagado
    set_blink_period(&semaforo->green_blink, 0);                       // Asegurarse de detener el parpadeo del LED verde
    timer_update_duration(&semaforo->timer, 1000);                     // Temporizador de 1 segundo
}


// Arreglos de transición para cada estado
static Transition verdeTransitions[] = {
    {button_pressed, VERDE_PARPADEO}  // Botón presionado (borde de subida): transición a VERDE_PARPADEO
};

static Transition verdeParpadeoTransitions[] = {
    {timer_expired, ROJO}  // Temporizador de 1 segundo expira: transición a ROJO
};

static Transition rojoTransitions[] = {
    {timer_expired, ROJO_PARPADEO}  // Temporizador de 3 segundos expira: transición a ROJO_PARPADEO
};

static Transition rojoParpadeoTransitions[] = {
    {timer_expired, VERDE}  // Temporizador de 1 segundo expira: transición a VERDE
};

// Estados de la FSM
static FSMState semaforoStates[] = {
    {verdeTransitions, 1, on_state_verde},
    {verdeParpadeoTransitions, 1, on_state_verde_parpadeo},
    {rojoTransitions, 1, on_state_rojo},
    {rojoParpadeoTransitions, 1, on_state_rojo_parpadeo}
};

// Inicialización de la FSM del semáforo peatonal
void semaforo_peatonal_init(SemaforoPeatonal *semaforo) {
    fsm_init(&semaforo->fsm, semaforoStates, VERDE, semaforo);  // Comienza en el estado VERDE

    // Inicializar los controles de parpadeo para cada LED
    blink_control_init(&semaforo->green_blink, GREEN_LED_PORT, GREEN_LED_PIN, 0);  // Parpadeo inicial apagado
    blink_control_init(&semaforo->red_blink, RED_LED_PORT, RED_LED_PIN, 0);        // Parpadeo inicial apagado

    // Inicializar el temporizador
    timer_start(&semaforo->timer, 0);  // Temporizador inicializado en 0

    // Inicializar el botón con antirrebote y el detector de bordes
    debounced_switch_init(&button_switch, BUTTON_PORT, BUTTON_PIN);
    edge_detector_init(&edge_detector, &button_switch);
}

// Actualización de la FSM y controles de parpadeo
void semaforo_peatonal_update(SemaforoPeatonal *semaforo) {
    debounced_switch_update(&button_switch);
    edge_detector_update(&edge_detector);
    fsm_update(&semaforo->fsm);

    // Imprime el estado actual de la FSM para depuración
    printf("Estado actual de la FSM: %ld\n", semaforo->fsm.currentState);

    if (semaforo->fsm.currentState == VERDE_PARPADEO) {
        blink_control_update(&semaforo->green_blink);
    } else if (semaforo->fsm.currentState == ROJO_PARPADEO) {
        blink_control_update(&semaforo->red_blink);
    }
}

