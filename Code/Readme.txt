Key Points:
MAC Address Matching:

Make sure to replace SENDER_MAC_ADDR in the receiver code with the actual MAC address of the sender device.
This MAC address should be retrieved from the sender device using the esp_read_mac function and then hardcoded in the receiver.


File System:

LittleFS vs. SPIFFS: Ensure that you are using the correct file system configuration that matches the partition scheme of your ESP32. If you are using SPIFFS, adjust the initialization accordingly.
Partition Label: Verify that the partition label for the file system matches the one defined in your partition table.
you are using the LittleFS file system