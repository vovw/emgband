#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"

#define CONSTRAIN_EMG_LOW 0
#define CONSTRAIN_EMG_HIGH 1000

// Function prototypes
float bound(float value, float low, float high);
float map(float x, float in_min, float in_max, float out_min, float out_max);
float bandpass_filter_75_150(float input);

void app_main(void)
{
    int raw_emg_value;
    float emg_value;
    // Configure ADC width and channel attenuation
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    while (1)
    {
        raw_emg_value = adc1_get_raw(ADC1_CHANNEL_6);
        printf("%d\n", raw_emg_value);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        
        float raw_emg_float = (float)raw_emg_value;
        raw_emg_float = bound(raw_emg_float, 0, 4095);
        raw_emg_float = map(raw_emg_float, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
        emg_value = bandpass_filter_75_150(raw_emg_float);
    }
}

float bound(float value, float low, float high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float bandpass_filter_75_150(float input)
{
    float output = input;
    {
        static float z1, z2; // filter section state
        float x = output - -1.64891342 * z1 - 0.77932149 * z2;
        output = 0.00014412 * x + 0.00028824 * z1 + 0.00014412 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -1.75188517 * z1 - 0.82786889 * z2;
        output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -1.69281282 * z1 - 0.88994696 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -1.88326655 * z1 - 0.93936089 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    return output;
}
