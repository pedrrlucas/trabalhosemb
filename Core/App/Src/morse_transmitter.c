/*
 * morse_transmitter.c
 */
#include "morse_transmitter.h"
#include "morse_common.h"
#include "morse_receiver.h"
#include "debug_log.h"
#include <string.h>
#include <ctype.h>

/**
 * @brief Converte um caractere para seu código Morse correspondente.
 * @param character: O caractere ASCII a ser convertido.
 * @retval Um ponteiro para a string do código Morse, ou NULL se não encontrado.
 */
static const char* find_morse_code(char character) {
    // Normaliza para maiúsculo para cobrir A-Z
    char upper_char = toupper((unsigned char)character);

    // Procura na tabela de A-Z e 0-9
    for (int i = 0; i < MORSE_TABLE_SIZE; i++) {
        if (upper_char == ASCII_LOOKUP_TABLE[i]) {
            return MORSE_CODE_TABLE[i];
        }
    }

    return NULL; // Caractere não encontrado
}


/**
  * @brief Converte e transmite uma string de texto em código Morse via PWM.
  * @param htim: Handle do Timer (PWM) a ser utilizado.
  * @param text_string: A string de texto a ser transmitida.
  */
void Morse_Transmitter_SendString(TIM_HandleTypeDef* htim, char* text_string)
{
    LOG("Transmissor: Iniciando transmissão de '%s'\r\n", text_string);
    // Antes de transmitir, reseta o estado do receptor para evitar auto-detecção
    Morse_Receiver_ResetState();

    // Loop principal: itera sobre cada caractere da string de texto
    for (int i = 0; i < strlen(text_string); i++)
    {
        char current_char = text_string[i];

        // Trata o espaço entre palavras
        if (current_char == ' ')
        {
            LOG("  Espaco de palavra.\r\n");
            HAL_Delay(WORD_SPACE_MS - LETTER_SPACE_MS); // Ajusta o delay para manter a proporção
            continue;
        }

        // Encontra o código Morse para o caractere atual
        const char* morse_code = find_morse_code(current_char);
        if (morse_code == NULL) {
            LOG("  Caractere '%c' nao suportado. Pulando...\r\n", current_char);
            continue;
        }

        LOG("  Transmitindo '%c' -> '%s'\r\n", current_char, morse_code);

        // Transmite a sequência de pontos e traços
        for (int j = 0; j < strlen(morse_code); j++) {
            char symbol = morse_code[j];

            if (symbol == '.') {
                HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
                HAL_Delay(DOT_TIME_MS);
                HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
                LOG("    . (Ponto)\r\n");
            } else if (symbol == '-') {
                HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
                HAL_Delay(DASH_TIME_MS);
                HAL_TIM_PWM_Stop(htim, TIM_CHANNEL_1);
                LOG("    - (Traço)\r\n");
            }

            // Adiciona o espaço padrão entre cada símbolo (dit-space)
            HAL_Delay(SYMBOL_SPACE_MS);
        }

        // Adiciona o espaço entre as letras
        HAL_Delay(LETTER_SPACE_MS - SYMBOL_SPACE_MS);
    }

    LOG("Transmissor: Transmissão concluída.\r\n");
}
