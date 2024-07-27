#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <driver/adc.h>

#define EXAMPLE_SENDER_CHANNEL 0
#define EXAMPLE_SENDER_SENSOR_GPIO 34 // Adjust to your GPIO pin
#define ESP_NOW_MAX_DATA_LEN 250 // Maximum data length for ESP-NOW

static const char *TAG = "esp_now_sender";

static void example_send_data(const uint8_t *data, size_t len) {
    esp_err_t ret;
    esp_now_peer_info_t peer_info = {0};
    uint8_t broadcast_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; // Broadcast address

    // Register peer
    memcpy(peer_info.peer_addr, broadcast_addr, 6);
    peer_info.channel = EXAMPLE_SENDER_CHANNEL;
    peer_info.encrypt = false;

    ret = esp_now_add_peer(&peer_info);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add peer: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_now_send(broadcast_addr, data, len);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send data: %s", esp_err_to_name(ret));
    }
}

static void example_sensor_init(void) {
    // Initialize sensor here (e.g., ADC setup)
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(EXAMPLE_SENDER_SENSOR_GPIO, ADC_ATTEN_DB_0);
}

static void example_send_task(void *pvParameter) {
    uint16_t sensor_data;
    uint8_t data[ESP_NOW_MAX_DATA_LEN];
    
    while (1) {
        sensor_data = adc1_get_raw(EXAMPLE_SENDER_SENSOR_GPIO);
        memcpy(data, &sensor_data, sizeof(sensor_data));
        example_send_data(data, sizeof(sensor_data));
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay between sends
    }
}

void app_main(void) {
    esp_err_t ret;

    // Initialize NVS , NVS is used to store PHY calibration data
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(example_send_data));

    // Initialize sensor
    example_sensor_init();

    // Create task to send data
    xTaskCreate(example_send_task, "example_send_task", 2048, NULL, 5, NULL);
}
