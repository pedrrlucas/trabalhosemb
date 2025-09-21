/*
 * dsp_engine.c
 */

#include "../../Dsp/Inc/dsp_engine.h"

#include "debug_log.h" // Adicionado para logging

// --- Buffers e Flags para o ADC/DSP ---
volatile uint16_t g_adcBuffer[ADC_BUFFER_SIZE];
volatile bool g_toneDetected = false;

// --- Protótipos de Funções Privadas ---
static float Goertzel_Magnitude(uint16_t* samples, uint16_t num_samples);
static void ProcessGoertzel(uint16_t* buffer_to_process);


/**
  * @brief Inicia o processo de amostragem contínua do ADC via DMA.
  * @param hadc: Handle do ADC a ser iniciado.
  */
void DSP_Engine_Start(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start_DMA(hadc, (uint32_t*)g_adcBuffer, ADC_BUFFER_SIZE);
}


/**
  * @brief  Processa um bloco de amostras usando o algoritmo de Goertzel.
  * @param  samples: Ponteiro para o buffer com as amostras.
  * @param  num_samples: Número de amostras a processar.
  * @retval A magnitude ao quadrado da frequência alvo.
  */
static float Goertzel_Magnitude(uint16_t* samples, uint16_t num_samples)
{
    float q1 = 0.0f;
    float q2 = 0.0f;
    float normalized_frequency = TARGET_FREQUENCY / SAMPLING_RATE;
    float coeff = 2.0f * cosf(2.0f * M_PI * normalized_frequency);

    for (int i = 0; i < num_samples; i++)
    {
        // Centraliza a amostra em 0 para o processamento
        float sample = (float)samples[i] - 2047.5f;
        float q0 = coeff * q1 - q2 + sample;
        q2 = q1;
        q1 = q0;
    }

    // Retorna a magnitude ao quadrado para evitar a operação de raiz quadrada
    return (q1 * q1) + (q2 * q2) - (q1 * q2 * coeff);
}

/**
  * @brief  Função auxiliar para processar um buffer e definir a flag de detecção de tom.
  * @param  buffer_to_process: Ponteiro para a metade do buffer a ser analisada.
  */
static void ProcessGoertzel(uint16_t* buffer_to_process)
{
    float magnitude_sq = Goertzel_Magnitude(buffer_to_process, ADC_BUFFER_SIZE / 2);
    bool new_tone_status = (magnitude_sq > GOERTZEL_THRESHOLD);

    // Log apenas quando o estado do tom muda, para não sobrecarregar o console
    if (new_tone_status != g_toneDetected)
    {
        g_toneDetected = new_tone_status;
        LOG("DSP: Tom Detectado -> %s\r\n", g_toneDetected ? "SIM" : "NÃO");
    }
}

/**
  * @brief  Callback chamado pelo DMA quando a primeira metade do buffer do ADC está cheia.
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
    ProcessGoertzel((uint16_t*)&g_adcBuffer[0]);
}

/**
  * @brief  Callback chamado pelo DMA quando o buffer completo do ADC está cheio.
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    ProcessGoertzel((uint16_t*)&g_adcBuffer[ADC_BUFFER_SIZE / 2]);
}

