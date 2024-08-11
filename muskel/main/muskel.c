#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_wifi.h" // For Wi-Fi connectivity
#include "nvs_flash.h"
#include "esp_http_server.h" // For the HTTP server
#include "esp_log.h"         // For logging
#include "esp_system.h"      // For system-related functions

#define SAMPLE_RATE 500
#define INPUT_PIN1 ADC1_CHANNEL_6
#define INPUT_PIN2 ADC1_CHANNEL_7
#define INPUT_PIN3 ADC1_CHANNEL_0
#define INPUT_PIN4 ADC1_CHANNEL_3 // Added a new channel
#define BUFFER_SIZE 64
#define SAMPLE_SIZE 1280
#define WIFI_SSID "dekhlo_tum_hi"
#define WIFI_PASS "mai_nahi_bataunga"

static int circular_buffer[BUFFER_SIZE];
static int samples1[SAMPLE_SIZE];
static int samples2[SAMPLE_SIZE];
static int samples3[SAMPLE_SIZE];
static int samples4[SAMPLE_SIZE]; // Added a new sample array
static int sample_index = 0;
static const char *TAG = "emgband_log";

static int data_index = 0, sum = 0;

static esp_adc_cal_characteristics_t adc_chars;

static int getEnvelope(int abs_emg);
static float EMGFilter(float input);
void emg();
void plotASCII(int value, int min, int max, int width);
void print_magnitudes();
void web_server();
esp_err_t get_handler(httpd_req_t *req);

httpd_uri_t uri_get = {
    .uri = "/uri",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL,
};

const TickType_t xDelay = 1000 / SAMPLE_RATE / portTICK_PERIOD_MS;

void emg(void *pvParameters)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(INPUT_PIN1, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN2, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN3, ADC_ATTEN_DB_11);
    adc1_config_channel_atten(INPUT_PIN4, ADC_ATTEN_DB_11);
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 0, &adc_chars);

    while (1)
    {
        vTaskDelay(xDelay);
        if (sample_index < SAMPLE_SIZE)
        {
            int adc_reading1 = adc1_get_raw(INPUT_PIN1);
            int adc_reading2 = adc1_get_raw(INPUT_PIN2);
            int adc_reading3 = adc1_get_raw(INPUT_PIN3);
            int adc_reading4 = adc1_get_raw(INPUT_PIN4);

            int voltage1 = esp_adc_cal_raw_to_voltage(adc_reading1, &adc_chars);
            int voltage2 = esp_adc_cal_raw_to_voltage(adc_reading2, &adc_chars);
            int voltage3 = esp_adc_cal_raw_to_voltage(adc_reading3, &adc_chars);
            int voltage4 = esp_adc_cal_raw_to_voltage(adc_reading4, &adc_chars);

            int sensor_value1 = (voltage1 * 1023) / 4095;
            int sensor_value2 = (voltage2 * 1023) / 4095;
            int sensor_value3 = (voltage3 * 1023) / 4095;
            int sensor_value4 = (voltage4 * 1023) / 4095;

            int signal1 = (int)EMGFilter((float)sensor_value1);
            int signal2 = (int)EMGFilter((float)sensor_value2);
            int signal3 = (int)EMGFilter((float)sensor_value3);
            int signal4 = (int)EMGFilter((float)sensor_value4);

            samples1[sample_index] = signal1;
            samples2[sample_index] = signal2;
            samples3[sample_index] = signal3;
            samples4[sample_index] = signal4;

            sample_index++;
        }
        else
        {
            print_magnitudes();
            sample_index = 0;
        }
    }
}

void wifi_for_server(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connecting to WiFi...");

    // Wait for IP address
    esp_netif_ip_info_t ip_info;
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (netif)
    {
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK)
        {
            ESP_LOGI(TAG, "IP Address: " IPSTR, IP2STR(&ip_info.ip));
        }
        else
        {
            ESP_LOGE(TAG, "Failed to get IP address");
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to get network interface");
    }
}

esp_err_t get_handler(httpd_req_t *req)
{
    char response[8192]; // Ensure the buffer is large enough to hold the sample data
    int pos = 0;

    for (int i = 0; i < SAMPLE_SIZE; i++)
    {
        pos += snprintf(response + pos, sizeof(response) - pos, "%d,%d,%d,%d\n", samples1[i], samples2[i], samples3[i], samples4[i]);
        if (pos >= sizeof(response))
        {
            break; // Avoid buffer overflow
        }
    }

    httpd_resp_send(req, response, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
    }
    return server;
}

void web_server(void *pvParameters)
{
    httpd_handle_t server = start_webserver();
    if (server == NULL)
    {
        ESP_LOGI("Web Server", "Failed to start web server");
    }
    else
    {
        ESP_LOGI("Web Server", "Web server started");
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
        printf("%d,%d,%d,%d\n", samples1[i], samples2[i], samples3[i], samples4[i]);
    }
    printf("\n");
}

void app_main(void)
{
    wifi_for_server();
    xTaskCreatePinnedToCore(emg, "does emg things", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(web_server, "web server", 4096, NULL, 1, NULL, 1);
}
