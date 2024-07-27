

                    # Define the project name
idf_component_register(
    SRCS "main.c"             # Source files
    INCLUDE_DIRS "."          # Include directory for headers
    REQUIRES esp_now          # ESP-NOW component
    REQUIRES esp_wifi         # Wi-Fi component
    REQUIRES esp_event        # Event component
    REQUIRES nvs_flash        # Non-volatile storage component
    REQUIRES spiffs
)

# Optionally, if you use SPIFFS for file handling, add the following lines
# Uncomment these lines if you are using SPIFFS
# idf_component_register(
#     SRCS "main.c"          # Source files
#     INCLUDE_DIRS "."       # Include directory for headers
#     REQUIRES esp_now       # ESP-NOW component
#     REQUIRES esp_wifi      # Wi-Fi component
#     REQUIRES esp_event     # Event component
#     REQUIRES nvs_flash     # Non-volatile storage component
#     REQUIRES spiffs        # SPIFFS component
# )
                    