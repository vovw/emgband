#include <stdio.h>
#include <string.h>
#include "esp_now.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define ESPNOW_RECV_CHANNEL 1       // ESP-NOW channel
#define ESPNOW_MAX_DATA_LEN 10      // Maximum length of received data

static const char *TAG = "ESP-NOW Receiver"; // Log tag for the receiver

// Function to initialize Wi-Fi in station mode
void init_wifi(void) {
    ESP_ERROR_CHECK(esp_netif_init()); // Initialize network interface
    ESP_ERROR_CHECK(esp_event_loop_create_default()); // Create default event loop
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // Initialize Wi-Fi configuration with default settings
    ESP_ERROR_CHECK(esp_wifi_init(&cfg)); // Initialize Wi-Fi driver
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Set Wi-Fi mode to station
    ESP_ERROR_CHECK(esp_wifi_start()); // Start Wi-Fi
}

// Callback function to handle incoming data
void on_data_recv(const uint8_t *mac_addr, const uint8_t *data, int len) {
    ESP_LOGI(TAG, "Received data from: " MACSTR ", length: %d", MAC2STR(mac_addr), len);
    // Log the received data
    for (int i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Function to add a peer device address (sender) to the ESP-NOW network
void add_peer(const uint8_t *peer_addr) {
    esp_now_peer_info_t peer_info = {}; // Structure to hold peer info
    memset(&peer_info, 0, sizeof(esp_now_peer_info_t)); // Zero out the peer info structure
    peer_info.channel = ESPNOW_RECV_CHANNEL; // Set the ESP-NOW channel
    peer_info.ifidx = ESP_IF_WIFI_STA; // Set the Wi-Fi interface index to station mode
    peer_info.encrypt = false; // Disable encryption

    // Copy the MAC address of the sender device into the peer_info structure
    memcpy(peer_info.peer_addr, peer_addr, ESP_NOW_ETH_ALEN);

    // Add the peer to the ESP-NOW network
    ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init()); // Initialize NVS flash
    init_wifi(); // Initialize Wi-Fi

    ESP_ERROR_CHECK(esp_now_init()); // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_register_recv_cb(on_data_recv)); // Register callback for receiving data

    // Define the MAC address of the sender device
    uint8_t sender_mac[ESP_NOW_ETH_ALEN] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}; // Replace with the actual MAC address of the sender

    // Add the sender device as a peer
    add_peer(sender_mac);

    ESP_LOGI(TAG, "ESP-NOW Receiver Initialized");
}
