#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "esp_spiffs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_timer.h"

#define CONSTRAIN_EMG_LOW 0
#define CONSTRAIN_EMG_HIGH 1000
#define FFT_SIZE 1280 // more than twise the number of readings the esp gets in one second

// ANSI color codes
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_ORANGE "\x1b[33m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct
{
    float real;
    float imag;
} complex_t;

float bandpass_filter_75_150(float input);
void print_colored_magnitude();
void print_beauty(float magnitude);
void init_adc();

void emg();

// ADC channels for the 3 sensors
#define INPUT_PIN1 ADC1_CHANNEL_6 // pin 34
#define INPUT_PIN2 ADC1_CHANNEL_7 // pin 35
#define INPUT_PIN3 ADC1_CHANNEL_0 // pin 36

static complex_t samples1[FFT_SIZE];
static complex_t samples2[FFT_SIZE];
static complex_t samples3[FFT_SIZE];
static int sample_index = 0;

void app_main(void)
{
    xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void init_adc()
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN3, ADC_ATTEN_DB_11);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, adc_chars);
}

void emg()
{
    // Configure ADC width and channel attenuation for all three channels
    init_adc();
    printf("START\n");

    while (1)
    {
        if (sample_index < FFT_SIZE)
        {
            // Read sensor values
            float sensor_value1 = adc1_get_raw(INPUT_PIN1);
            float sensor_value2 = adc1_get_raw(INPUT_PIN2);
            float sensor_value3 = adc1_get_raw(INPUT_PIN3);

            // output of 0 - 4095
            float signal1 = sensor_value1 * 2.45 / 4095;
            float signal2 = sensor_value2 * 2.45 / 4095;
            float signal3 = sensor_value3 * 2.45 / 4095;

            // Apply bandpass filter
            // float signal1 = sensor_value1;
            signal1 = bandpass_filter_75_150(sensor_value1);
            signal2 = bandpass_filter_75_150(sensor_value2);
            signal3 = bandpass_filter_75_150(sensor_value3);

            // Store the signals in their respective arrays
            samples1[sample_index].real = signal1;
            samples1[sample_index].imag = 0;
            samples2[sample_index].real = signal2;
            samples2[sample_index].imag = 0;
            samples3[sample_index].real = signal3;
            samples3[sample_index].imag = 0;

            sample_index++;
        }
        else
        {
            // Print the magnitude of each sample
            print_colored_magnitude();

            sample_index = 0; // Reset sample index for the next set of samples
        }
    }
}

void print_colored_magnitude()
{
    for (int i = 0; i < FFT_SIZE / 2; i++)
    {
        float magnitude1 = sqrt(samples1[i].real * samples1[i].real + samples1[i].imag * samples1[i].imag);
        float magnitude2 = sqrt(samples2[i].real * samples2[i].real + samples2[i].imag * samples2[i].imag);
        float magnitude3 = sqrt(samples3[i].real * samples3[i].real + samples3[i].imag * samples3[i].imag);

        // printf("%.2f\n", magnitude1);
        printf("%.2f,%.2f,%.2f\n", magnitude1, magnitude2, magnitude3);
    }
    printf("\n"); // End of line for the next set of data
}
void print_beauty(float magnitude)
{
    const char *color;
    if (magnitude < 5)
    {
        color = ANSI_COLOR_GREEN;
    }
    else if (magnitude < 10)
    {
        color = ANSI_COLOR_BLUE;
    }
    else if (magnitude < 20)
    {
        color = ANSI_COLOR_ORANGE;
    }
    else
    {
        color = ANSI_COLOR_RED;
    }
    printf("%.2f,", magnitude);
    // printf("%s %.2f%s\n", color, magnitude, ANSI_COLOR_RESET);
}

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500.0 Hz, frequency: [75.0, 150.0] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
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
