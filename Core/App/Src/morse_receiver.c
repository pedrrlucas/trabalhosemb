/*
 * morse_receiver.c
 */
#include "morse_receiver.h"
#include "morse_common.h"
#include "morse_transmitter.h" // Para as definições de tempo
#include "debug_log.h"
#include <string.h>
#include <ctype.h>
#include "../../Dsp/Inc/dsp_engine.h"


// --- Variáveis para a Lógica do Receptor ---
static ReceiverState g_rxState = STATE_RX_IDLE;
static uint32_t g_lastActivityTime = 0;
static uint32_t g_signalChangeTime = 0;
static char g_rxSymbolBuffer[10] = {0};
static uint8_t g_rxSymbolIndex = 0;

// --- Protótipos de Funções Privadas ---
static char DecodeMorseChar(char* morse);


/**
  * @brief Decodifica uma string Morse para um caractere ASCII.
  * @param morse: A string Morse a ser decodificada (ex: ".-").
  * @retval O caractere ASCII, ou ' ' se não for encontrado.
  */
static char DecodeMorseChar(char* morse)
{
    // A tabela agora está sincronizada, então a busca funciona corretamente
    for (int i = 0; i < MORSE_TABLE_SIZE; i++)
    {
        if (strcmp(morse, MORSE_CODE_TABLE[i]) == 0)
        {
            return ASCII_LOOKUP_TABLE[i];
        }
    }
    return '?'; // Caractere não encontrado
}

/**
  * @brief Reseta a máquina de estados do receptor para o estado inicial.
  */
void Morse_Receiver_ResetState(void)
{
    if (g_rxState != STATE_RX_IDLE)
    {
        LOG("Receptor: Estado resetado para IDLE.\r\n");
    }
    g_rxState = STATE_RX_IDLE;
    g_rxSymbolIndex = 0;
    memset(g_rxSymbolBuffer, 0, sizeof(g_rxSymbolBuffer));
}

/**
  * @brief Processa a lógica do receptor Morse. Deve ser chamada continuamente.
  * @param huart: Handle da UART para transmitir o caractere decodificado.
  */
void Morse_Receiver_Process(UART_HandleTypeDef* huart)
{
    static bool last_tone_status = false;
    uint32_t current_time = HAL_GetTick();

    // Se não houver atividade por muito tempo, reseta para o estado inicial.
    if (g_rxState != STATE_RX_IDLE && (current_time - g_lastActivityTime > RX_TIMEOUT_MS))
    {
        Morse_Receiver_ResetState();
        return;
    }

    // Processa apenas se houver uma mudança no sinal (som -> silêncio ou vice-versa)
    if (g_toneDetected != last_tone_status)
    {
        uint32_t duration = current_time - g_signalChangeTime;

        // Evento: O SOM ACABOU (transição de true para false)
        if (g_toneDetected == false)
        {
            // Classifica a duração do TOM e adiciona ao buffer
            if (duration > (DOT_TIME_MS - 40) && duration < (DOT_TIME_MS + 40))
            {
                if(g_rxSymbolIndex < sizeof(g_rxSymbolBuffer) - 1) g_rxSymbolBuffer[g_rxSymbolIndex++] = '.';
            }
            else if (duration > (DASH_TIME_MS - 40) && duration < (DASH_TIME_MS + 40))
            {
                if(g_rxSymbolIndex < sizeof(g_rxSymbolBuffer) - 1) g_rxSymbolBuffer[g_rxSymbolIndex++] = '-';
            }
        }
        // Evento: O SOM COMEÇOU (transição de false para true)
        else
        {
            if (g_rxState == STATE_RX_IDLE) {
                g_rxState = STATE_RX_LISTENING_PREAMBLE; // Começa a ouvir
                LOG("Receptor: Atividade detectada. Estado -> OUVINDO PREÂMBULO\r\n");
            } else {
                // Classifica a duração do SILÊNCIO para decodificar letras/palavras
                if (duration > (LETTER_SPACE_MS - 50) && duration < (WORD_SPACE_MS - 50))
                {
                    g_rxSymbolBuffer[g_rxSymbolIndex] = '\0'; // Finaliza a string do símbolo

                    if (g_rxState == STATE_RX_LISTENING_PREAMBLE)
                    {
                        // Aqui, em vez de um preâmbulo fixo, você pode usar um caractere de início
                        char decoded_char = DecodeMorseChar(g_rxSymbolBuffer);
                        if (decoded_char == ' ') { // Ou algum caractere de controle
                            g_rxState = STATE_RX_LISTENING_PAYLOAD;
                            LOG("Receptor: Preâmbulo CORRETO! Estado -> OUVINDO PAYLOAD\r\n");
                        } else {
                            LOG("Receptor: Preâmbulo INCORRETO ('%s'). Tentando novamente.\r\n", g_rxSymbolBuffer);
                        }
                    }
                    else if (g_rxState == STATE_RX_LISTENING_PAYLOAD)
                    {
                        char decoded_char = DecodeMorseChar(g_rxSymbolBuffer);
                        LOG("Receptor: Símbolo decodificado '%s' -> '%c'\r\n", g_rxSymbolBuffer, decoded_char);
                        HAL_UART_Transmit(huart, (uint8_t*)&decoded_char, 1, 100);
                    }
                    g_rxSymbolIndex = 0;
                    memset(g_rxSymbolBuffer, 0, sizeof(g_rxSymbolBuffer));
                }
                else if (duration >= (WORD_SPACE_MS - 50))
                {
                    LOG("Receptor: Espaço de palavra detectado.\r\n");
                    if (g_rxState == STATE_RX_LISTENING_PAYLOAD) {
                         HAL_UART_Transmit(huart, (uint8_t*)" ", 1, 100);
                    }
                    g_rxSymbolIndex = 0;
                    memset(g_rxSymbolBuffer, 0, sizeof(g_rxSymbolBuffer));
                }
            }
        }

        g_signalChangeTime = current_time;
        g_lastActivityTime = current_time;
        last_tone_status = g_toneDetected;
    }
}
