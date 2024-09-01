## Overview
This code is designed to process Electromyography (EMG) signals using an ESP32 microcontroller. It reads raw data from four ADC channels, applies a filtering algorithm, and stores the processed data in circular buffers. The code also includes functionality to detect and print signals that exceed a specified threshold.

## Features
- Reads EMG signals from four ADC channels.
- Applies a custom filtering algorithm to the raw signals.
- Stores processed signals in circular buffers.
- Detects and prints signals that exceed a predefined threshold.
- Uses FreeRTOS for task management.

## Hardware Requirements
- ESP32 microcontroller
- Four EMG sensors connected to ADC channels 6, 7, 4, and 5 (pins 34, 35, 32, and 33 respectively).

## Software Requirements
- ESP-IDF framework
- FreeRTOS

## Configuration
- `SAMPLE_RATE`: The sampling rate of the EMG signals (500 Hz).
- `INPUT_PIN1`, `INPUT_PIN2`, `INPUT_PIN3`, `INPUT_PIN4`: ADC channels for the EMG sensors.
- `BUFFER_SIZE`: The size of the circular buffers (300 samples).
- `THRESHOLD`: The threshold value for signal detection (12).

## Functions
- `app_main()`: Initializes the EMG processing task.
- `emg()`: Main function that configures ADC, reads raw data, applies filtering, and manages the circular buffers.
- `add()`: Adds new samples to the circular buffers.
- `delete_buffer()`: Clears the circular buffers.
- `print_buffer()`: Prints the contents of the circular buffers.
- `EMGFilter()`: Applies a custom filtering algorithm to the input signal.

## Usage
```bash
idf.py build
idf.py flash monitor
```

## Notes
- The filtering algorithm in `EMGFilter()` is designed to remove noise and artifacts from the EMG signals.
- The code uses FreeRTOS to manage the EMG processing task, ensuring efficient and reliable operation.

## License
This code is released under the MIT License. See the LICENSE file for more details.
