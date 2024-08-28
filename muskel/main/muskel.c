#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define SAMPLE_RATE 500

#define INPUT_PIN1 ADC1_CHANNEL_6 // pin 34
#define INPUT_PIN2 ADC1_CHANNEL_7 // pin 35
#define INPUT_PIN3 ADC1_CHANNEL_4 // pin 32
#define INPUT_PIN4 ADC1_CHANNEL_5 // pin 33

#define BUFFER_SIZE 300
#define THRESHOLD 12

static int circular_buffer1[BUFFER_SIZE];
static int circular_buffer2[BUFFER_SIZE];
static int circular_buffer3[BUFFER_SIZE];
static int circular_buffer4[BUFFER_SIZE];

static int data_index = 0, times = 0, single = 0;

static esp_adc_cal_characteristics_t adc_chars;

static int getEnvelope(int abs_emg);
static float EMGFilter(float input);
void emg(void);
void print_buffer();

const TickType_t xDelay = 1000 / SAMPLE_RATE / portTICK_PERIOD_MS;

void app_main(void)
{
    xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void add(int element1, int element2, int element3, int element4)
{
    // Shift elements to the left to make space for the new element
    for (int i = 0; i < BUFFER_SIZE - 1; i++)
    {
        circular_buffer1[i] = circular_buffer1[i + 1];
        circular_buffer2[i] = circular_buffer2[i + 1];
        circular_buffer3[i] = circular_buffer3[i + 1];
        circular_buffer4[i] = circular_buffer4[i + 1];
    }

    // Add the new element at the end of the circular_buffer
    circular_buffer1[BUFFER_SIZE - 1] = element1;
    circular_buffer2[BUFFER_SIZE - 1] = element2;
    circular_buffer3[BUFFER_SIZE - 1] = element3;
    circular_buffer4[BUFFER_SIZE - 1] = element4;
}

void delete_buffer()
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        circular_buffer1[i] = 0;
        circular_buffer2[i] = 0;
        circular_buffer3[i] = 0;
        circular_buffer4[i] = 0;
    }
}

void emg(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN4, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);
    printf("START\n");
    while (1)
    {
        vTaskDelay(xDelay);

        int reading1 = adc1_get_raw(INPUT_PIN1);
        int reading2 = adc1_get_raw(INPUT_PIN2);
        int reading3 = adc1_get_raw(INPUT_PIN3);
        int reading4 = adc1_get_raw(INPUT_PIN4);

        int voltage1 = esp_adc_cal_raw_to_voltage(reading1, &adc_chars);
        int voltage2 = esp_adc_cal_raw_to_voltage(reading2, &adc_chars);
        int voltage3 = esp_adc_cal_raw_to_voltage(reading3, &adc_chars);
        int voltage4 = esp_adc_cal_raw_to_voltage(reading4, &adc_chars);

        int sensor_value1 = (voltage1 * 1023) / 4095;
        int sensor_value2 = (voltage2 * 1023) / 4095;
        int sensor_value3 = (voltage3 * 1023) / 4095;
        int sensor_value4 = (voltage4 * 1023) / 4095;

        int signal1 = (int)EMGFilter((float)sensor_value1);
        int signal2 = (int)EMGFilter((float)sensor_value2);
        int signal3 = (int)EMGFilter((float)sensor_value3);
        int signal4 = (int)EMGFilter((float)sensor_value4);

        if (data_index != BUFFER_SIZE - 1)
        {
            circular_buffer1[data_index] = signal1;
            circular_buffer2[data_index] = signal2;
            circular_buffer3[data_index] = signal3;
            circular_buffer4[data_index] = signal4;
            data_index++;
        }
        else
        {
            add(signal1, signal2, signal3, signal4);
        }

        for (int i = BUFFER_SIZE / 4; i < BUFFER_SIZE / 2; i++)
        {
            if (abs(circular_buffer1[i]) > THRESHOLD ||
                abs(circular_buffer2[i]) > THRESHOLD ||
                abs(circular_buffer3[i]) > THRESHOLD ||
                abs(circular_buffer4[i]) > THRESHOLD)
            {

                print_buffer();
                printf("\n");
                delete_buffer();
                data_index = 0;
                break;
            }
        }
    }
}

void print_buffer()
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        printf("%d, %d, %d, %d\n", circular_buffer1[i], circular_buffer2[i], circular_buffer3[i], circular_buffer4[i]);
    }
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
