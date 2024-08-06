#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define SAMPLE_RATE 500
#define INPUT_PIN ADC1_CHANNEL_6
#define BUFFER_SIZE 64

static int circular_buffer[BUFFER_SIZE];
static int data_index = 0, sum = 0;

static esp_adc_cal_characteristics_t adc_chars;

static int getEnvelope(int abs_emg);
static float EMGFilter(float input);
void emg(void);
void plotASCII(int value, int min, int max, int width);

const TickType_t xDelay = 1000 / SAMPLE_RATE / portTICK_PERIOD_MS;


void app_main(void)
{
    xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void emg(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);

    while (1) {
	vTaskDelay(xDelay);

        int adc_reading = adc1_get_raw(INPUT_PIN);
        int voltage = esp_adc_cal_raw_to_voltage(adc_reading, &adc_chars);

	// TODO : check and confront this
        // Convert voltage to original Arduino's 0-1023 scale
        int sensor_value = (voltage * 1023) / 4095;

        // Filtered EMG
        int signal = (int)EMGFilter((float)sensor_value);

        printf("%d\n", signal);
    }
}

int getEnvelope(int abs_emg)
{
    sum -= circular_buffer[data_index];
    sum += abs_emg;
    circular_buffer[data_index] = abs_emg;
    data_index = (data_index + 1) % BUFFER_SIZE;
    return (sum/BUFFER_SIZE) * 2;
}

float EMGFilter(float input)
{
    float output = input;
    {
        static float z1, z2; // filter section state
        float x = output - 0.05159732*z1 - 0.36347401*z2;
        output = 0.01856301*x + 0.03712602*z1 + 0.01856301*z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -0.53945795*z1 - 0.39764934*z2;
        output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - 0.47319594*z1 - 0.70744137*z2;
        output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -1.00211112*z1 - 0.74520226*z2;
        output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
        z2 = z1;
        z1 = x;
    }
    return output;
}

void plotASCII(int value, int min, int max, int width) {
    int range = max - min;
    int position = (value - min) * width / range;
    
    for (int i = 0; i < width; i++) {
        if (i == position) {
            printf("*");
        } else {
            printf("-");
        }
    }
    printf(" %d\n", value);
}

