#include <stdio.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// samples per second
#define sample_rate 500

// change if not using gpio36 (adc1_channel_0)
#define input_pin adc_channel_0

// envelope buffer size
// high value -> smooth but less responsive
// low value -> not smooth but responsive
#define buffer_size 64

static int circular_buffer[buffer_size];
static int data_index, sum;

// adc handle
static adc_oneshot_unit_handle_t adc1_handle;
static adc_cali_handle_t adc1_cali_handle = null;

// function prototypes
static int getenvelope(int abs_emg);
static float emgfilter(float input);
static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle);

void app_main(void)
{
    // adc1 config
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = adc_unit_1,
    };
    esp_error_check(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = adc_bitwidth_12,
        .atten = adc_atten_db_11,
    };
    esp_error_check(adc_oneshot_config_channel(adc1_handle, input_pin, &config));

    // adc calibration
    bool cali_enable = adc_calibration_init(adc_unit_1, input_pin, adc_atten_db_11, &adc1_cali_handle);

    // main loop
    while (1) {
        // calculate elapsed time
        static int64_t past = 0;
        int64_t present = esp_timer_get_time();
        int64_t interval = present - past;
        past = present;

        // run timer
        static int64_t timer = 0;
        timer -= interval;

        // sample and get envelope
        if (timer < 0) {
            timer += 1000000 / sample_rate;
            
            // raw emg
            int adc_raw;
            esp_error_check(adc_oneshot_read(adc1_handle, input_pin, &adc_raw));
            
            int voltage;
            if (cali_enable) {
                esp_error_check(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw, &voltage));
            } else {
                voltage = adc_raw; // use raw value if calibration is not enabled
            }
            
            // filtered emg
            int signal = (int)emgfilter((float)voltage);
            
            // emg envelope
            int envelope = getenvelope(abs(signal));

            // print results
            printf("%d,%d\n", signal, envelope);
        }

        // small delay to prevent watchdog timer from triggering
        vtaskdelay(1);
    }
}

// envelope detection algorithm
static int getenvelope(int abs_emg) {
    sum -= circular_buffer[data_index];
    sum += abs_emg;
    circular_buffer[data_index] = abs_emg;
    data_index = (data_index + 1) % buffer_size;
    return (sum/buffer_size) * 2;
}

// band-pass butterworth iir digital filter
static float emgfilter(float input)
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

static bool adc_calibration_init(adc_unit_t unit, adc_channel_t channel, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = null;
    esp_err_t ret = esp_fail;
    bool calibrated = false;

#if adc_cali_scheme_curve_fitting_supported
    if (!calibrated) {
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = adc_bitwidth_default,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == esp_ok) {
            calibrated = true;
        }
    }
#endif

#if adc_cali_scheme_line_fitting_supported
    if (!calibrated) {
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = adc_bitwidth_default,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == esp_ok) {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    return calibrated;
}
