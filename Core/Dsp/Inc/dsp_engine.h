/*
 * dsp_engine.h
 */

#ifndef INC_DSP_ENGINE_H_
#define INC_DSP_ENGINE_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <math.h>

// --- Definições para o DSP (Goertzel) ---
#define TARGET_FREQUENCY    1000.0f  // Frequência do tom a ser detectado (1 kHz)
#define SAMPLING_RATE       8000.0f  // Frequência de amostragem do ADC (deve ser > 2x TARGET_FREQUENCY)
#define ADC_BUFFER_SIZE     256      // Tamanho do buffer de amostras do DMA
#define GOERTZEL_THRESHOLD  5.0e7f   // Limiar de detecção (precisa de calibração experimental)

// --- Variáveis Globais Externas ---
extern volatile bool g_toneDetected;
extern volatile uint16_t g_adcBuffer[ADC_BUFFER_SIZE];

// --- Protótipos de Funções Públicas ---
void DSP_Engine_Start(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);


#endif /* INC_DSP_ENGINE_H_ */
