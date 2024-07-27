#include <stdio.h>
#include <string.h>
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_spiffs.h"

#define ESPNOW_SEND_CHANNEL 1       // ESP-NOW channel
#define ESPNOW_SEND_LEN 10          // Length of data to be sent
#define FILE_PATH "/output.txt"     // Path to the file in SPIFFS

static const char *TAG = "ESP-NOW Sender";

// Function to initialize Wi-Fi in station mode
void init_wifi(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

// Function to initialize SPIFFS
void init_spiffs(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

    size_t total = 0;
    size_t used = 0;
    esp_err_t ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information");
        return;
    }
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
}

// Function to deinitialize SPIFFS
void deinit_spiffs(void) {
    ESP_ERROR_CHECK(esp_vfs_spiffs_unregister(NULL));
}

// Callback function for ESP-NOW send status
void on_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "Send Status: %s", status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init()); // Initialize NVS flash
    init_wifi(); // Initialize Wi-Fi
    init_spiffs(); // Initialize SPIFFS

    ESP_ERROR_CHECK(esp_now_init()); // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_register_send_cb(on_sent)); // Register send callback

    esp_now_peer_info_t peer_info = {}; // Structure to hold peer info
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t)); // Zero out the peer info structure
    peer_info.channel = ESPNOW_SEND_CHANNEL; // Set the ESP-NOW channel
    peer_info.ifidx = ESP_IF_WIFI_STA; // Set the Wi-Fi interface index to station mode
    peer_info.encrypt = false; // Disable encryption

    // Define the MAC address of the peer device
    uint8_t target_mac[ESP_NOW_ETH_ALEN] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}; // Replace with the actual MAC address of the peer

    // Copy the MAC address into the peer_info structure
    memcpy(peer_info.peer_addr, target_mac, ESP_NOW_ETH_ALEN);

    // Add the peer to the ESP-NOW network
    ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));

    // Open the file in SPIFFS for reading
    FILE* f = fopen(FILE_PATH, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        deinit_spiffs(); // Deinitialize SPIFFS if file open fails
        return;
    }

    uint8_t data[ESPNOW_SEND_LEN] = {0}; // Array to hold the data to be sent
    size_t i = 0;
    // Read data from the file
    while (fscanf(f, "%hhu", &data[i]) == 1 && i < ESPNOW_SEND_LEN) {
        ++i; // Increment index
    }
    fclose(f); // Close the file

    if (i == 0) {
        ESP_LOGW(TAG, "No data read from file"); // Log warning if no data was read
    } else {
        ESP_LOGI(TAG, "Sending data...");
        ESP_ERROR_CHECK(esp_now_send(target_mac, data, i)); // Send the data using ESP-NOW
    }

    deinit_spiffs(); // Deinitialize SPIFFS
}

