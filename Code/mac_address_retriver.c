#include "esp_system.h"
#include "esp_log.h"

void app_main(void) {
    uint8_t mac_addr[6];
    esp_read_mac(mac_addr, ESP_MAC_WIFI_STA); // Retrieve MAC address for Wi-Fi Station
    
    printf("Wi-Fi Station MAC Address: " 
           "%02X:%02X:%02X:%02X:%02X:%02X\n",
           mac_addr[0], mac_addr[1], mac_addr[2],
           mac_addr[3], mac_addr[4], mac_addr[5]);
}
