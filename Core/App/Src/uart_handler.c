/*
 * uart_handler.c
 */

#include "uart_handler.h"
#include "debug_log.h"
#include <string.h>

// --- Variáveis Globais ---
volatile bool g_morseTxFlag = 0; // Flag para sinalizar nova mensagem para transmitir
char g_morseTxBuffer[256];       // Buffer para a mensagem Morse a ser enviada
static uint8_t g_uartRxBuffer[1];  // Buffer para receber 1 byte da UART

/**
  * @brief Inicia a primeira escuta da UART via interrupção.
  * @param huart: Handle da UART a ser utilizada.
  */
void UART_Handler_Init(UART_HandleTypeDef* huart)
{
    HAL_UART_Receive_IT(huart, g_uartRxBuffer, 1);
}

/**
  * @brief Callback de Recepção da UART. Chamado a cada byte recebido.
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static char command_buffer[256];
    static int idx = 0;

    if (huart->Instance == USART1)
    {
        // Se receber um caractere de nova linha, a mensagem Morse terminou
        if (g_uartRxBuffer[0] == '\n' || g_uartRxBuffer[0] == '\r')
        {
            if (idx > 0) // Processa apenas se a mensagem não estiver vazia
            {
                command_buffer[idx] = '\0';
                strcpy(g_morseTxBuffer, command_buffer);
                g_morseTxFlag = 1; // Ativa a flag para a main loop iniciar a transmissão
                LOG("UART: Mensagem recebida para transmissão: '%s'\r\n", command_buffer);
            }
            idx = 0; // Reseta o índice para a próxima mensagem
        }
        // Se for qualquer outro caractere, adiciona ao buffer
        else if (idx < sizeof(command_buffer) - 1)
        {
            command_buffer[idx++] = g_uartRxBuffer[0];
        }

        // Reativa a interrupção para esperar pelo próximo byte. ESSENCIAL!
        HAL_UART_Receive_IT(huart, g_uartRxBuffer, 1);
    }
}

