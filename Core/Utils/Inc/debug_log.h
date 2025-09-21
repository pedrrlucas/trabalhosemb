/*
 * debug_log.h
 */

#ifndef INC_DEBUG_LOG_H_
#define INC_DEBUG_LOG_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

// --- Configuração do Log ---
// Comente esta linha para desativar completamente todos os logs e economizar espaço no flash
#define DEBUG_LOG_ENABLED

// --- Protótipos de Funções Públicas ---
void Log_Init(UART_HandleTypeDef* huart);
void Log_Message(const char* format, ...);

// --- Macro de Log ---
// Se o log estiver habilitado, a macro LOG(...) se expande para a chamada da função.
// Se desabilitado, a macro se expande para "nada", removendo as chamadas do código compilado.
#ifdef DEBUG_LOG_ENABLED
    #define LOG(...) Log_Message(__VA_ARGS__)
#else
    #define LOG(...) ((void)0)
#endif


#endif /* INC_DEBUG_LOG_H_ */

