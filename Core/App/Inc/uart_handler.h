/*
 * uart_handler.h
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// --- Variáveis Globais Externas ---
extern volatile bool g_morseTxFlag;
extern char g_morseTxBuffer[256];


// --- Protótipos de Funções Públicas ---
void UART_Handler_Init(UART_HandleTypeDef* huart);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif /* INC_UART_HANDLER_H_ */

