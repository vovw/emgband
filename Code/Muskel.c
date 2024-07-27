#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_timer.h"

#define CONSTRAIN_EMG_LOW 0
#define CONSTRAIN_EMG_HIGH 1000
#define SAMPLE_RATE 500 // Adjusted to match the filter design
#define INPUT_PIN ADC1_CHANNEL_6
#define FFT_SIZE 256 // Must be a power of 2

// ANSI color codes
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_ORANGE  "\x1b[33m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct {
    float real;
    float imag;
} complex_t;

float map(float x, float in_min, float in_max, float out_min, float out_max);
float bandpass_filter_75_150(float input);
void fft(complex_t *x, int n);
void bit_reverse(complex_t *x, int n);
void apply_hanning_window(complex_t *x, int n);
void print_colored_magnitude(int frequency_bin, float magnitude);
void emg_task(void *pvParameters);

static complex_t samples[FFT_SIZE];
static int sample_index = 0;

void app_main(void) {
    xTaskCreate(emg_task, "EMG Processing Task", 4096, NULL, tskIDLE_PRIORITY, NULL);
}

void emg_task(void *pvParameters) {
    // Configure ADC width and channel attenuation
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN, ADC_ATTEN_DB_11);

    printf("EMG Acquisition Setup Complete\n");

    while (1) {
        if (sample_index < FFT_SIZE) {
            int sensor_value = adc1_get_raw(INPUT_PIN);
            sensor_value = map(sensor_value, 0, 4095, CONSTRAIN_EMG_LOW, CONSTRAIN_EMG_HIGH);
            float filtered_signal = bandpass_filter_75_150(sensor_value);
            samples[sample_index].real = filtered_signal;
            samples[sample_index].imag = 0;
            sample_index++;
        } else {
            apply_hanning_window(samples, FFT_SIZE);
            fft(samples, FFT_SIZE);

            for (int i = 0; i < FFT_SIZE / 2; i++) {
                float magnitude = sqrt(samples[i].real * samples[i].real + samples[i].imag * samples[i].imag);
                print_colored_magnitude(i, magnitude);
            }

            sample_index = 0; // Reset sample index for next set of samples
        }
        vTaskDelay(pdMS_TO_TICKS(1000 / SAMPLE_RATE));
    }
}

void print_colored_magnitude(int frequency_bin, float magnitude) {
    const char* color;
    if (magnitude < 10) {
        color = ANSI_COLOR_GREEN;
    } else if (magnitude < 130) {
        color = ANSI_COLOR_BLUE;
    } else if (magnitude < 200) {
        color = ANSI_COLOR_ORANGE;
    } else {
        color = ANSI_COLOR_RED;
    }
    
    printf("%sFrequency bin %d: %.2f%s\n", color, frequency_bin, magnitude, ANSI_COLOR_RESET);
}
float bound(float value, float low, float high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}
// Implementations of map, bandpass_filter_75_150, apply_hanning_window, fft, and bit_reverse go here

// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500.0 Hz, frequency: [75.0, 150.0] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
// Reference: https://docs.scipy.org/doc/scipy/reference/generated/scipy.signal.butter.html

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float bandpass_filter_75_150(float input) {
    static float z1[4] = {0}, z2[4] = {0};
    float output = input;
    float coefficients[4][6] = {
        {0.00014412, 0.00028824, 0.00014412, -1.64891342, -0.77932149},
        {1.00000000, 2.00000000, 1.00000000, -1.75188517, -0.82786889},
        {1.00000000, -2.00000000, 1.00000000, -1.69281282, -0.88994696},
        {1.00000000, -2.00000000, 1.00000000, -1.88326655, -0.93936089}
    };

    for (int i = 0; i < 4; i++) {
        float x = output - coefficients[i][3] * z1[i] - coefficients[i][4] * z2[i];
        output = coefficients[i][0] * x + coefficients[i][1] * z1[i] + coefficients[i][2] * z2[i];
        z2[i] = z1[i];
        z1[i] = x;
    }

    return output;
}

void apply_hanning_window(complex_t *x, int n) {
    for (int i = 0; i < n; i++) {
        float w = 0.5 * (1 - cos(2 * M_PI * i / (n - 1)));
        x[i].real *= w;
        x[i].imag *= w;
    }
}

void fft(complex_t *x, int n) {
    bit_reverse(x, n);

    for (int s = 1; s <= log2(n); s++) {
        int m = 1 << s; // 2 power s
        float omega = -2.0 * M_PI / m;

        for (int k = 0; k < n; k += m) {
            for (int j = 0; j < m / 2; j++) {
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

void bit_reverse(complex_t *x, int n) {
    int bits = (int)log2(n);
    for (int i = 0; i < n; i++) {
        int j = 0;
        for (int k = 0; k < bits; k++) {
            j = (j << 1) | ((i >> k) & 1);
        }
        if (j > i) {
            complex_t temp = x[i];
            x[i] = x[j];
            x[j] = temp;
        }
    }
}