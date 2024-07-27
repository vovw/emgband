#include <stdio.h>
#include <string.h>
#include <esp_log.h>
#include <esp_err.h>
#include <esp_now.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <fs/vfs.h>
#include <fs/littlefs.h>

#define EXAMPLE_RECEIVER_CHANNEL 0
#define ESP_NOW_MAX_DATA_LEN 250 // Maximum data length for ESP-NOW
#define SENDER_MAC_ADDR {0x24, 0x6F, 0x28, 0x61, 0x58, 0x9F}  // Replace with your sender's MAC address


static const char *TAG = "esp_now_receiver";

static void example_receive_data(const uint8_t *data, size_t len) {
    FILE *f = fopen("/spiffs/output.txt", "a");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for appending");
        return;
    }

    fprintf(f, "Received data: ");
    for (size_t i = 0; i < len; ++i) {
        fprintf(f, "%02x ", data[i]);
    }
    fprintf(f, "\n");
    fclose(f);
}

static void example_espnow_recv_cb(const uint8_t *mac_addr, const uint8_t *data, int len) {
    ESP_LOGI(TAG, "Data received from " MACSTR, MAC2STR(mac_addr));
    example_receive_data(data, len);
}
void app_main(void) {
    esp_err_t ret;

    // Initialize NVS (Non-Volatile Storage)
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize LittleFS (or SPIFFS) for file operations
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/spiffs",                // Path for LittleFS
        .partition_label = "storage",         // Label of the partition (make sure it matches your partition label)
        .format_if_mount_failed = true,       // Format if mount fails
        .dont_mount = false
    };
    ESP_ERROR_CHECK(esp_vfs_littlefs_register(&conf));

    // Initialize ESP-NOW
    ESP_ERROR_CHECK(esp_now_init());

    // Register the callback for receiving data
    ESP_ERROR_CHECK(esp_now_register_recv_cb(example_espnow_recv_cb));

    // Add the peer
    esp_now_peer_info_t peer_info = {0};
    uint8_t sender_mac_addr[6] = SENDER_MAC_ADDR;
    memcpy(peer_info.peer_addr, sender_mac_addr, 6);
    peer_info.channel = EXAMPLE_RECEIVER_CHANNEL;  // Must match the sender’s channel
    peer_info.encrypt = false;  // Encryption status should match the sender’s configuration

    ESP_ERROR_CHECK(esp_now_add_peer(&peer_info));

    // Initialize file system and create the file
    FILE *f = fopen("/spiffs/output.txt", "w");  // Open file in write mode
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to create file");
        return;
    }
    fprintf(f, "Data received from ESP-NOW:\n");  // Write header to file
    fclose(f);  // Close the file
}