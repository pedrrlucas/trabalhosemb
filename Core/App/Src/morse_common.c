/*
 * morse_common.c
 */
#include "morse_common.h"

// Tabela de conversão de texto para Morse.
// A ordem dos códigos deve corresponder exatamente à ordem dos caracteres em ASCII_LOOKUP_TABLE.
const char* const MORSE_CODE_TABLE[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..", // A-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...",
    "---..", "----.", // 0-9
    "/", "--..--", ".-.-.-", "..--..", " " // Espaço e sinais de pontuação comuns
};

// Tabela de consulta para encontrar o caractere correspondente.
// A ordem dos caracteres deve corresponder à ordem dos códigos em MORSE_CODE_TABLE.
const char* const ASCII_LOOKUP_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789/,? ";

// Constante com o tamanho da tabela, calculada em tempo de compilação
const uint32_t MORSE_TABLE_SIZE = sizeof(MORSE_CODE_TABLE) / sizeof(MORSE_CODE_TABLE[0]);
