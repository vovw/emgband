# Edit the following lines to set component requirements (see docs)

set(COMPONENT_REQUIRES)
set(COMPONENT_PRIV_REQUIRES )

# Set the source files and include directories for the component
set(COMPONENT_SRCS "emg.c")
set(COMPONENT_ADD_INCLUDEDIRS "")

# Include the SPIFFS component
# set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/components/spiffs)

# Create SPIFFS partition image

# Register the component with the build system
register_component()
spiffs_create_partition_image(storage ../partition FLASH_IN_PROJECT)
