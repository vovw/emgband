#include <stdio.h>
#include <esp_system.h>

void app_main(void) {
    uint8_t mac[6];
    
    // Get the MAC address of the station interface (Wi-Fi)
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    printf("Wi-Fi Station MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}