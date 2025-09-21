/*
 * debug_log.c
 */
#include "debug_log.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static UART_HandleTypeDef* g_logUart = NULL;
static char g_logBuffer[256];

/**
  * @brief Inicializa o sistema de log.
  * @param huart: O handle da UART que será usada para enviar os logs.
  */
void Log_Init(UART_HandleTypeDef* huart)
{
    g_logUart = huart;
}

/**
  * @brief Envia uma mensagem de log formatada via UART.
  * @param format: A string de formato (estilo printf).
  * @param ...: Argumentos variáveis para a formatação.
  */
void Log_Message(const char* format, ...)
{
#ifdef DEBUG_LOG_ENABLED
    if (g_logUart == NULL) {
        return; // Não faz nada se a UART não foi inicializada
    }

    va_list args;
    va_start(args, format);

    // Formata a string de log
    int len = vsnprintf(g_logBuffer, sizeof(g_logBuffer), format, args);

    va_end(args);

    if (len > 0)
    {
        // Transmite a string formatada via UART
        HAL_UART_Transmit(g_logUart, (uint8_t*)g_logBuffer, len, 100); // Timeout de 100ms
    }
#endif
}

