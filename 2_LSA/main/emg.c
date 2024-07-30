// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include "esp_spiffs.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "driver/adc.h"
// #include "esp_timer.h"

// #define CONSTRAIN_EMG_LOW 0
// #define CONSTRAIN_EMG_HIGH 1000
// #define SAMPLE_RATE 300
// #define FFT_SIZE 256 // Must be a power of 2
// #define BUFFER_SIZE 128
// #define MAGNITUDE_THRESHOLD 0.01

// // ANSI color codes
// #define ANSI_COLOR_GREEN "\x1b[32m"
// #define ANSI_COLOR_BLUE "\x1b[34m"
// #define ANSI_COLOR_ORANGE "\x1b[33m"
// #define ANSI_COLOR_RED "\x1b[31m"
// #define ANSI_COLOR_RESET "\x1b[0m"

// typedef struct
// {
//     float real;
//     float imag;
// } complex_t;

// float bound(float value, float low, float high);
// float map(float x, float in_min, float in_max, float out_min, float out_max);
// float bandpass_filter_75_150(float input);
// void fft(complex_t *x, int n);
// void bit_reverse(complex_t *x, int n);
// void apply_hanning_window(complex_t *x, int n);
// void print_colored_magnitude();

// void emg();

// // ADC channels for the 3 sensors
// #define INPUT_PIN1 ADC1_CHANNEL_6
// // #define INPUT_PIN2 ADC1_CHANNEL_7
// // #define INPUT_PIN3 ADC1_CHANNEL_0

// int circular_buffer[BUFFER_SIZE];
// int data_index = 0, sum = 0;

// static complex_t samples1[FFT_SIZE];
// // static complex_t samples2[FFT_SIZE];
// // static complex_t samples3[FFT_SIZE];
// static int sample_index = 0;

// void app_main(void)
// {
//     xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
// }

// void emg()
// {
//     // Configure ADC width and channel attenuation for all three channels
//     adc1_config_width(ADC_WIDTH_BIT_12);
//     adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
//     adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
//     adc1_config_channel_atten(INPUT_PIN3, ADC_ATTEN_DB_11);
//     printf("START\n");

//     while (1)
//     {
//         if (sample_index < FFT_SIZE)
//         {
//             // Read sensor values
//             int sensor_value1 = adc1_get_raw(INPUT_PIN1);
//             int sensor_value2 = adc1_get_raw(INPUT_PIN2);
//             int sensor_value3 = adc1_get_raw(INPUT_PIN3);

//             // Apply bound and map functions
//             sensor_value1 = bound(sensor_value1, 0, 4095);
//             sensor_value2 = bound(sensor_value2, 0, 4095);
//             sensor_value3 = bound(sensor_value3, 0, 4095);

//             sensor_value1 = map(sensor_value1, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
//             sensor_value2 = map(sensor_value2, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
//             sensor_value3 = map(sensor_value3, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);

//             // Apply bandpass filter
//             float signal1 = bandpass_filter_75_150(sensor_value1);
//             float signal2 = bandpass_filter_75_150(sensor_value2);
//             float signal3 = bandpass_filter_75_150(sensor_value3);

//             // Store the signals in their respective arrays
//             samples1[sample_index].real = signal1;
//             samples1[sample_index].imag = 0;
//             samples2[sample_index].real = signal2;
//             samples2[sample_index].imag = 0;
//             samples3[sample_index].real = signal3;
//             samples3[sample_index].imag = 0;

//             sample_index++;
//         }
//         else
//         {
//             // Print the magnitude of each sample
//             print_colored_magnitude();

//             sample_index = 0; // Reset sample index for the next set of samples
//         }
//     }
// }

// void print_colored_magnitude()
// {
//     for (int i = 0; i < FFT_SIZE / 2; i++)
//     {
//         float magnitude1 = sqrt(samples1[i].real * samples1[i].real + samples1[i].imag * samples1[i].imag);
//         float magnitude2 = sqrt(samples2[i].real * samples2[i].real + samples2[i].imag * samples2[i].imag);
//         float magnitude3 = sqrt(samples3[i].real * samples3[i].real + samples3[i].imag * samples3[i].imag);

//         printf("%.2f,%.2f,%.2f\n", magnitude1, magnitude2, magnitude3);
//     }
//     printf("\n"); // End of line for the next set of data
// }

// float bound(float value, float low, float high)
// {
//     if (value < low)
//         return low;
//     if (value > high)
//         return high;
//     return value;
// }

// float map(float x, float in_min, float in_max, float out_min, float out_max)
// {
//     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
// }

// // Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// // Sampling rate: 500.0 Hz, frequency: [75.0, 150.0] Hz.
// // Filter is order 4, implemented as second-order sections (biquads).
// // Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html
// float bandpass_filter_75_150(float input)
// {
//     float output = input;
//     {
//         static float z1, z2; // filter section state
//         float x = output - -1.64891342 * z1 - 0.77932149 * z2;
//         output = 0.00014412 * x + 0.00028824 * z1 + 0.00014412 * z2;
//         z2 = z1;
//         z1 = x;
//     }
//     {
//         static float z1, z2; // filter section state
//         float x = output - -1.75188517 * z1 - 0.82786889 * z2;
//         output = 1.00000000 * x + 2.00000000 * z1 + 1.00000000 * z2;
//         z2 = z1;
//         z1 = x;
//     }
//     {
//         static float z1, z2; // filter section state
//         float x = output - -1.69281282 * z1 - 0.88994696 * z2;
//         output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
//         z2 = z1;
//         z1 = x;
//     }
//     {
//         static float z1, z2; // filter section state
//         float x = output - -1.88326655 * z1 - 0.93936089 * z2;
//         output = 1.00000000 * x + -2.00000000 * z1 + 1.00000000 * z2;
//         z2 = z1;
//         z1 = x;
//     }
//     return output;
// }

// void apply_hanning_window(complex_t *x, int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//         float w = 0.5 * (1 - cos(2 * M_PI * i / (n - 1)));
//         x[i].real *= w;
//         x[i].imag *= w;
//     }
// }

// void fft(complex_t *x, int n)
// {
//     bit_reverse(x, n);

//     for (int s = 1; s <= log2(n); s++)
//     {
//         int m = 1 << s; // 2 power s
//         float omega = -2.0 * M_PI / m;

//         for (int k = 0; k < n; k += m)
//         {
//             for (int j = 0; j < m / 2; j++)
//             {
//                 complex_t t;
//                 float omega_t = j * omega;
//                 t.real = cos(omega_t);
//                 t.imag = sin(omega_t);

//                 complex_t u = x[k + j];
//                 complex_t v;
//                 v.real = t.real * x[k + j + m / 2].real - t.imag * x[k + j + m / 2].imag;
//                 v.imag = t.real * x[k + j + m / 2].imag + t.imag * x[k + j + m / 2].real;

//                 x[k + j].real = u.real + v.real;
//                 x[k + j].imag = u.imag + v.imag;

//                 x[k + j + m / 2].real = u.real - v.real;
//                 x[k + j + m / 2].imag = u.imag - v.imag;
//             }
//         }
//     }
// }

// void bit_reverse(complex_t *x, int n)
// {
//     for (int i = 0; i < n; i++)
//     {
//         int j = 0;
//         for (int k = 0; k < log2(n); k++)
//         {
//             j = (j << 1) | ((i >> k) & 1);
//         }
//         if (j > i)
//         {
//             complex_t temp = x[i];
//             x[i] = x[j];
//             x[j] = temp;
//         }
//     }
// }
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
#define SAMPLE_RATE 300
#define FFT_SIZE 256 // Must be a power of 2
#define BUFFER_SIZE 128
#define MAGNITUDE_THRESHOLD 0.01

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

float bound(float value, float low, float high);
float map(float x, float in_min, float in_max, float out_min, float out_max);
float bandpass_filter_75_150(float input);
void fft(complex_t *x, int n);
void bit_reverse(complex_t *x, int n);
void apply_hanning_window(complex_t *x, int n);
void print_colored_magnitude();
void print_beauty(float magnitude);

void emg();

// ADC channels for the 3 sensors
#define INPUT_PIN1 ADC1_CHANNEL_6
// #define INPUT_PIN2 ADC1_CHANNEL_7
// #define INPUT_PIN3 ADC1_CHANNEL_0

int circular_buffer[BUFFER_SIZE];
int data_index = 0, sum = 0;

static complex_t samples1[FFT_SIZE];
// static complex_t samples2[FFT_SIZE];
// static complex_t samples3[FFT_SIZE];
static int sample_index = 0;

void app_main(void)
{
    xTaskCreate(emg, "does emg things", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void emg()
{
    // Configure ADC width and channel attenuation for all three channels
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
    // adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
    // adc1_config_channel_atten(INPUT_PIN3, ADC_ATTEN_DB_11);
    printf("START\n");

    while (1)
    {
        if (sample_index < FFT_SIZE)
        {
            // Read sensor values
            int sensor_value1 = adc1_get_raw(INPUT_PIN1);
            // int sensor_value2 = adc1_get_raw(INPUT_PIN2);
            // int sensor_value3 = adc1_get_raw(INPUT_PIN3);

            // Apply bound and map functions
            sensor_value1 = bound(sensor_value1, 0, 4095);
            // sensor_value2 = bound(sensor_value2, 0, 4095);
            // sensor_value3 = bound(sensor_value3, 0, 4095);

            // sensor_value1 = map(sensor_value1, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
            // sensor_value2 = map(sensor_value2, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
            // sensor_value3 = map(sensor_value3, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);

            // Apply bandpass filter
            float signal1 = bandpass_filter_75_150(sensor_value1);
            // float signal1 = sensor_value1;
            // float signal2 = bandpass_filter_75_150(sensor_value2);
            // float signal3 = bandpass_filter_75_150(sensor_value3);

            // Store the signals in their respective arrays
            samples1[sample_index].real = signal1;
            samples1[sample_index].imag = 0;
            // samples2[sample_index].real = signal2;
            // samples2[sample_index].imag = 0;
            // samples3[sample_index].real = signal3;
            // samples3[sample_index].imag = 0;

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
        // float magnitude1 = samples1[i].real;
        // float magnitude2 = sqrt(samples2[i].real * samples2[i].real + samples2[i].imag * samples2[i].imag);
        // float magnitude3 = sqrt(samples3[i].real * samples3[i].real + samples3[i].imag * samples3[i].imag);

        // printf("%.2f\n", magnitude1);
        print_beauty(magnitude1);
        // printf("%.2f,%.2f,%.2f\n", magnitude1, magnitude2, magnitude3);
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
float bound(float value, float low, float high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

void apply_hanning_window(complex_t *x, int n)
{
    for (int i = 0; i < n; i++)
    {
        float w = 0.5 * (1 - cos(2 * M_PI * i / (n - 1)));
        x[i].real *= w;
        x[i].imag *= w;
    }
}

void fft(complex_t *x, int n)
{
    bit_reverse(x, n);

    for (int s = 1; s <= log2(n); s++)
    {
        int m = 1 << s; // 2 power s
        float omega = -2.0 * M_PI / m;

        for (int k = 0; k < n; k += m)
        {
            for (int j = 0; j < m / 2; j++)
            {
                complex_t t;
                float omega_t = j * omega;
                t.real = cos(omega_t);
                t.imag = sin(omega_t);

                complex_t u = x[k + j];
                complex_t v;
                v.real = t.real * x[k + j + m / 2].real - t.imag * x[k + j + m / 2].imag;
                v.imag = t.real * x[k + j + m / 2].imag + t.imag * x[k + j + m / 2].real;

                x[k + j].real = u.real + v.real;
                x[k + j].imag = u.imag + v.imag;

                x[k + j + m / 2].real = u.real - v.real;
                x[k + j + m / 2].imag = u.imag - v.imag;
            }
        }
    }
}

void bit_reverse(complex_t *x, int n)
{
    for (int i = 0; i < n; i++)
    {
        int j = 0;
        for (int k = 0; k < log2(n); k++)
        {
            j = (j << 1) | ((i >> k) & 1);
        }
        if (j > i)
        {
            complex_t temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }
}
