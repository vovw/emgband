#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define SAMPLE_RATE 500
#define INPUT_PIN1 ADC1_CHANNEL_6
#define INPUT_PIN2 ADC1_CHANNEL_7
#define INPUT_PIN3 ADC1_CHANNEL_0
#define BUFFER_SIZE 64
#define SAMPLE_SIZE 1280

static int circular_buffer[BUFFER_SIZE];
static int samples1[SAMPLE_SIZE];
static int samples2[SAMPLE_SIZE];
static int samples3[SAMPLE_SIZE];
static int sample_index = 0;

static int data_index = 0, sum = 0;

static esp_adc_cal_characteristics_t adc_chars;

static int getEnvelope(int abs_emg);
static float EMGFilter(float input);
void emg(void);
void plotASCII(int value, int min, int max, int width);
void print_magnitudes();

const TickType_t xDelay = 1000 / SAMPLE_RATE / portTICK_PERIOD_MS;

void app_main(void)
{
    xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void emg(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN3s, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);

    while (1)
    {
        vTaskDelay(xDelay);
        if (sample_index != 0)
        {
            int adc_reading1 = adc1_get_raw(INPUT_PIN1);
            int adc_reading2 = adc1_get_raw(INPUT_PIN2);
            int adc_reading3 = adc1_get_raw(INPUT_PIN3);

            int voltage1 = esp_adc_cal_raw_to_voltage(adc_reading1, &adc_chars);
            int voltage2 = esp_adc_cal_raw_to_voltage(adc_reading2, &adc_chars);
            int voltage3 = esp_adc_cal_raw_to_voltage(adc_reading3, &adc_chars);

            // TODO : check and confront this
            // Convert voltage to original Arduino's 0-1023 scale
            int sensor_value1 = (voltage1 * 1023) / 4095;
            int sensor_value2 = (voltage2 * 1023) / 4095;
            int sensor_value3 = (voltage3 * 1023) / 4095;

            int signal1 = (int)EMGFilter((float)sensor_value1);
            int signal2 = (int)EMGFilter((float)sensor_value2);
            int signal3 = (int)EMGFilter((float)sensor_value3);

            samples1[sample_index] = signal1;
            samples2[sample_index] = signal2;
            samples3[sample_index] = signal3;

            sample_index++;
        }
        else
        {
            print_magnitudes();
            sample_index = 0;
        }
    }
}

int getEnvelope(int abs_emg)
{
    sum -= circular_buffer[data_index];
    sum += abs_emg;
    circular_buffer[data_index] = abs_emg;
    data_index = (data_index + 1) % BUFFER_SIZE;
    return (sum / BUFFER_SIZE) * 2;
}

float EMGFilter(float input)
{
    float output = input;
    {
        static float z1, z2; // filter section state
        float x = output - 0.05159732 * z1 - 0.36347401 * z2;
        output = 0.01856301 * x + 0.03712602 * z1 + 0.01856301 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -0.53945795 * z1 - 0.39764934 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - 0.47319594 * z1 - 0.70744137 * z2;
        output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    {
        static float z1, z2; // filter section state
        float x = output - -1.00211112 * z1 - 0.74520226 * z2;
        output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
        z2 = z1;
        z1 = x;
    }
    return output;
}

void plotASCII(int value, int min, int max, int width)
{
    int range = max - min;
    int position = (value - min) * width / range;

    for (int i = 0; i < width; i++)
    {
        if (i == position)
        {
            printf("*");
        }
        else
        {
            printf("-");
        }
    }
    printf(" %d\n", value);
}
void print_magnitudes()
{
    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        printf("%d,%d,%d", samples1[i], samples2[i], samples3[i]);
    }
    printf("\n");
}