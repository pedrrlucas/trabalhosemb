/*
 * morse_transmitter.h
 */
#ifndef INC_MORSE_TRANSMITTER_H_
#define INC_MORSE_TRANSMITTER_H_

#include "stm32f4xx_hal.h"
#include "morse_common.h" // Adicionado para incluir as definições de tempo
#include <stdbool.h>

// --- Protótipos de Funções Públicas ---
void Morse_Transmitter_SendString(TIM_HandleTypeDef* htim, char* text_string);

#endif /* INC_MORSE_TRANSMITTER_H_ */
