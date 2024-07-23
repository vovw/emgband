#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_timer.h"

#define CONSTRAIN_EMG_LOW 0
#define CONSTRAIN_EMG_HIGH 1000
#define SAMPLE_RATE 500
#define INPUT_PIN ADC1_CHANNEL_6
#define FFT_SIZE 256 // Must be a power of 2
#define BUFFER_SIZE 128

typedef struct {
    float real;
    float imag;
} complex_t;

float bound(float value, float low, float high);
float map(float x, float in_min, float in_max, float out_min, float out_max);
float bandpass_filter_75_150(float input);
int getEnvelop(int abs_emg);
void fft(complex_t *x, int n);
void bit_reverse(complex_t *x, int n);

int circular_buffer[BUFFER_SIZE];
int data_index = 0, sum = 0;

static complex_t samples[FFT_SIZE];
static int sample_index = 0;

void app_main(void)
{
    // Configure ADC width and channel attenuation
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN, ADC_ATTEN_DB_11);

    static unsigned long past = 0;
    unsigned long present;
    unsigned long interval;

    static long timer = 0;

    while (1)
    {
        if (sample_index < FFT_SIZE)
        {
            int sensor_value = adc1_get_raw(INPUT_PIN);
            sensor_value = bound(sensor_value, 0, 4095);
            sensor_value = map(sensor_value, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
            float signal = bandpass_filter_75_150(sensor_value);
            samples[sample_index].real = signal;
            samples[sample_index].imag = 0;
            sample_index++;
        }
        else
        {
            fft(samples, FFT_SIZE);
            for (int i = 0; i < FFT_SIZE / 2; i++)
            {
                float magnitude = sqrt(samples[i].real * samples[i].real + samples[i].imag * samples[i].imag);
                printf("Frequency bin %d: %f\n", i, magnitude);
            }

            sample_index = 0; // Reset sample index for next set of samples
        }
        vTaskDelay(1000 / SAMPLE_RATE / portTICK_PERIOD_MS);
    }
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

int getEnvelop(int abs_emg)
{
    sum -= circular_buffer[data_index];
    sum += abs_emg;
    circular_buffer[data_index] = abs_emg;
    data_index = (data_index + 1) % BUFFER_SIZE;
    return (sum / BUFFER_SIZE) * 2;
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

void fft(complex_t *x, int n)
{
    bit_reverse(x, n);

    for (int s = 1; s <= log2(n); s++)
    {
        int m = 1 << s; // 2 power s
        float omega = -2.0 * M_PI / m;

        for (int k = 0; k < n; k += m)
        {
            for (int j = 0; j < m/2; j++)
            {
                complex_t t;
                float omega_t = j * omega;
                t.real = cos(omega_t);
                t.imag = sin(omega_t);

                complex_t u = x[k + j];
                complex_t v;
                v.real = t.real * x[k + j + m/2].real - t.imag * x[k + j + m/2].imag;
                v.imag = t.real * x[k + j + m/2].imag + t.imag * x[k + j + m/2].real;

                x[k + j].real = u.real + v.real;
                x[k + j].imag = u.imag + v.imag;

                x[k + j + m/2].real = u.real - v.real;
                x[k + j + m/2].imag = u.imag - v.imag;
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
