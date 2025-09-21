/*
 * morse_receiver.h
 */
#ifndef INC_MORSE_RECEIVER_H_
#define INC_MORSE_RECEIVER_H_

#include "stm32f4xx_hal.h"
#include "morse_common.h" // Adicionado para incluir as definições de tempo e estados
#include <stdbool.h>

// --- Protótipos de Funções Públicas ---
void Morse_Receiver_Process(UART_HandleTypeDef* huart);
void Morse_Receiver_ResetState(void);


#endif /* INC_MORSE_RECEIVER_H_ */
