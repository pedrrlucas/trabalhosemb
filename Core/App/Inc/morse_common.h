/*
 * morse_common.h
 */
#ifndef MORSE_COMMON_H_
#define MORSE_COMMON_H_

#include <stdint.h>

// Definições de tempos em milissegundos
#define DOT_TIME_MS      120
#define DASH_TIME_MS     (3 * DOT_TIME_MS)
#define SYMBOL_SPACE_MS  (1 * DOT_TIME_MS)
#define LETTER_SPACE_MS  (3 * DOT_TIME_MS)
#define WORD_SPACE_MS    (7 * DOT_TIME_MS)
#define RX_TIMEOUT_MS    (8 * DOT_TIME_MS)

// Declarações externas para as tabelas de conversão
// O 'extern' informa ao compilador que essas variáveis estão definidas em 'morse_common.c'
extern const char* const MORSE_CODE_TABLE[];
extern const char* const ASCII_LOOKUP_TABLE;
extern const uint32_t MORSE_TABLE_SIZE;

// Tipos de estado do receptor
typedef enum {
    STATE_RX_IDLE,
    STATE_RX_LISTENING_PREAMBLE,
    STATE_RX_LISTENING_PAYLOAD
} ReceiverState;

#endif /* MORSE_COMMON_H_ */
