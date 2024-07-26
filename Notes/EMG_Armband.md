---
title: EMG_Armband

---

# EMG_Armband

To create an EMG armband that detects hand movements and orientation via forearm muscle impulses, design PCBs with EMG sensors, integrate them with microcontrollers, and train the system using machine learning.

Measuring muscle activation via electric potential, referred to as [electromyography (EMG)](http://en.wikipedia.org/wiki/Electromyography) , has traditionally been used for medical research and diagnosis of neuromuscular disorders.

## Project Workflow

- Learn to initialize the ADC and read digital outputs from an analog sensor.
- Study the ESP_NOW communication protocol and send messages between ESP devices.
- Learn to design flexible PCBs.
- Study EMG sensors and integrate them with ESP32 microcontrollers.

# TABLE OF CONTENT:

1. TRANSMITTER SETUP
    - [EMG signal acquisition](https://www.notion.so/EMG-signal-acquisition)
    - [Amplifying circuit](https://www.notion.so/Amplifying-circuit)
    - [Bandpass filter](https://www.notion.so/Bandpass-filter)
    - [EMG signals to ADC channels of ESP-32](https://www.notion.so/EMG-signals-to-ADC-channels-of-ESP-32)
    - [Errors and It's Solutions.](https://www.notion.so/ERRORS-and-It's-Solutions)
2. RECEIVER SETUP
    - [Data acquisition form ADC channel(esp-32).](https://www.notion.so/Data-acquisition-form-ADC-channel-(esp-32))
3. ESP-NOW
4. ML MODEL
5. PCB DESIGNING
    - [Board images.](https://www.notion.so/%5BBoard-images)
    - [changes need to be Done in Board.](https://www.notion.so/changes-need-to-be-Done-in-Board)

## TRANSMITTER SETUP

- EMG signal acquisition.

### Wet electrodes

conductive gel or electrolyte solution enhances the signal quality by reducing impedance.

Wet electrodes offer superior signal quality and lower noise compared to dry electrodes.

### Dry electrode

Dry electrodes do not require any additional gel .They are designed to make direct contact with the skin's surface.

They are more user-friendly and convenient for quick and easy application . Dry electrodes may have slightly higher impedance compared to wet electrodes, which can lead to lower signal quality, especially in challenging conditions.
**Dry electrode are ideal for applications where convenience and ease of use are prioritized over the absolute highest signal quality.
In this project we use wet electrode as we are prioritized over the absolute highest signal quality.**

![image](https://hackmd.io/_uploads/B1aFUqPD0.png)


- Amplifying circuit.

EMG signals are weak, low-amplitude electrical signals produced by muscle contractions. Amplification increases signal strength, reduces noise, enhances sensitivity, and allows for precise data analysis. This is critical for applications like monitoring muscle activity, and ensuring high-quality, reliable EMG data.

EMG signal acquisition system, we utilize five electrodes, with each amplifying module being dedicated to one specific electrode. Therefore, we require a total of five amplifying modules, each serving as an interface for an individual electrode. This setup allows us to amplify and process the signals from all five electrodes independently and enhances the accuracy and specificity of our EMG data collection.

### Instrumental Amplifier

https://github.com/anuj-akotkar/EMG_armband/assets/142881764/628677fb-256e-48c7-add5-0407b27e1081

Instrumentation amplifiers (In-Amps) are important because they amplify weak signals accurately while ignoring noise that affects both input lines equally. They ensure precise amplification with low noise, making them perfect for critical applications like medical devices and data collection systems. Their high input impedance means they don’t interfere with the signal source, and their gain can be easily adjusted to meet specific needs, maintaining signal quality and accuracy.

### amplifying circuit

for an Amplifying circuits check out this links below:

- https://www.instructables.com/Muscle-EMG-Sensor-for-a-Microcontroller/
- https://github.com/upsidedownlabs/BioAmp-EXG-Pill

### Bandpass filter

A bandpass filter for EMG signals is an electronic circuit that focuses on a specific range of frequencies within the EMG signal. It reduces noise and highlights muscle activity, improving the quality of the EMG data. By setting the lower and upper frequency limits, the filter can be adjusted to match the EMG signal's frequency range, making it essential for accurately analyzing muscle activity in medical and research settings.

### 4th order bandpass filter

https://github.com/anuj-akotkar/EMG_armband/assets/142881764/091cea03-248b-4bb8-b407-fafddba9b6de

https://github.com/anuj-akotkar/EMG_armband/assets/142881764/00ee053e-c1bb-4873-9779-60e22ef640a6

The "4th order" refers to the filter having four reactive components (inductors or capacitors) in its design, which results in a more selective and sharper cutoff. This means it more effectively isolates the desired frequency range and reduces unwanted noise compared to lower-order filters.

### ESP32 PINOUT

![Untitled (1)](https://hackmd.io/_uploads/H1dLL9wDC.png)

### EMG signals to ADC channels of ESP-32.(Analog to Digital converter)

The ESP32 integrates 2 SAR (Successive Approximation Register) ADCs, supporting a total of 18 measurement channels (analog enabled pins)

These channels are supported:

- 8 channels: GPIO32 - GPIO39
- 10 channels: GPIO0, GPIO2, GPIO4, GPIO12 - GPIO15, GOIO25 - GPIO27.

https://github.com/anuj-akotkar/EMG_armband/assets/142881764/b8764f82-aeae-4df4-95bf-fa711ba12b2e

### ADC Attenuation and Conversion

1. **ADC Attenuation:**
    - **Purpose:** ADCs in devices like the ESP32 have a maximum input voltage range that is typically limited by the reference voltage (Vref). If the input voltage exceeds Vref, attenuation can be applied to scale down the input voltage to fit within the ADC's measurable range (0 to Vref).
    - **Options:** ESP32 offers four attenuation options, with higher attenuation allowing measurement of higher input voltages by scaling them down effectively.
2. **ADC Conversion:**
    - **Process:** ADC conversion involves taking an analog voltage input and converting it into a digital value. ESP32 ADCs typically provide 12-bit resolution in single-read mode, meaning they can represent analog voltages across 4096 discrete levels (2^12).

### ESP32 ADC Configuration and Readings

- **`adc1_config_width(adc_bits_width_t width_bit)`**: This function configures the ADC1 capture width across all channels, setting the resolution for subsequent ADC readings.
- **`adc1_get_raw(adc1_channel_t channel)`**: Retrieves a raw ADC reading from a specified ADC1 channel, converting the analog input into a digital value.

### EMG Signal Processing Issues and Solutions

1. **Issue: Output of an amplifying circuit does not vary with muscle movement.**
    - **Solution:** Employ a two-step amplification approach:
        - **First Stage:** Amplify the EMG signal to optimize for initial processing and noise reduction.
        - **Second Stage (muscle_sensor_v3):** Further amplify the refined signal to enhance signal quality and accuracy.
2. **Issue: Variation observed in EMG signal readings with high noise.**
    - **Solution:** Implement a 4th-order bandpass filter:
        - **Purpose:** Refine EMG signal data by selectively passing a specific range of frequencies relevant to muscle activity while suppressing frequencies outside this range.
        - **Implementation:** Utilize a 4th-order Butterworth filter, which is effective for EMG signal processing.

### Additional Resources

- **GitHub Link for 4th Order Butterworth Filter:** [EMGFilter.ino](https://github.com/upsidedownlabs/BioAmp-EXG-Pill/blob/main/software/EMGFilter/EMGFilter.ino)

This filter implementation helps in improving the quality of EMG signal readings by filtering out unwanted noise and focusing on the relevant signal frequencies associated with muscle activity. Using such filters ensures that the processed EMG data is more accurate and useful for further analysis or applications.

## RECEIVER SETUP

When we move data from one ESP module to another, we do it in chunks or blocks. Each block contains about 1000 values and covers a span of 5 to 6 seconds. These blocks are really important because they form the datasets we use to train and test our Machine Learning (ML) model.

Data transfer process between ESP modules, we transmit the data in the form of sequential blocks, each containing approximately 1000 values, and each block spans a duration of around 5-6 seconds. These data blocks are significant as they serve as the training and testing datasets for our Machine Learning (ML) model. By transferring data in this organized manner, we ensure that the ML model receives a consistent and substantial dataset for training and evaluation, which is crucial for achieving accurate and meaningful results in our application. This structured data transfer approach supports the development and validation of our ML model for its intended purpose.

## ESP-NOW

ESP-NOW is a way for Wi-Fi devices, like those made by Espressif (such as ESP8266 or ESP32), to communicate with each other without needing to be connected to a regular Wi-Fi network. It's useful for devices like smart lights, remote controls, and sensors.

- **Connectionless Communication:** Devices using ESP-NOW can send data directly to each other without being part of a larger Wi-Fi network. It's like sending messages between friends directly rather than through a middleman.
- **Security:** To keep these messages safe from unauthorized access, ESP-NOW uses a security protocol called CTR with CBC-MAC (CCMP). This ensures that only the intended devices can read and use the messages.
- **MAC Header:** Each message includes a MAC address, which is a unique identifier for each ESP module. It helps in routing messages to the right device.

### Key Actions and Functions

- **Initialization and De-initialization:**
    - Use `esp_now_init()` to set up ESP-NOW and `esp_now_deinit()` to stop using it. When deinitialized, all paired device information is deleted.
- **Adding Paired Devices:**
    - Before sending data to a device, you need to add it to the paired device list using `esp_now_add_peer()`.
- **Sending Data:**
    - Data is sent using `esp_now_send()`. You can also register a callback function (`esp_now_register_send_cb()`) to know if the data was sent successfully or not.
- **Receiving Data:**
    - Register a callback function (`esp_now_register_recv_cb()`) to handle incoming data. This function is called whenever ESP-NOW receives data.

For detailed implementation examples and further reading, you can check the ESP-IDF documentation [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32).

## PCB DESIGNING

- BOARD IMAGES
- Amplifying circuit(front side)
- amplifying circuit and esp-32 intregrted module:
- front side
- back side
- routing

## ML MODEL

In this project, a machine learning model is implemented to predict real-time data, specifically discerning between two hand movements: one for a clenched fist and the other for a relaxed hand. The ML model is designed to classify the ongoing hand movements into these two distinct classes.

For improved accuracy, the model is trained on a dataset that captures diverse instances of both fist-clenching and hand relaxation


# EMG_ARMBAND -2

EMG Process: 

```mermaid
flowchart LR
S(surface electrodes) --> Amp(Amplifier)
Amp(Amplifier) --> Anti(Anti-Aliasing filter)
Anti(Anti-Aliasing filter) --> ADC(ADC)
ADC(ADC) --> F(Fast Fourier transformer)
F --> B(Bandpass Filter)
B --> FR(Full-wave rectifier)
FR --> P(Processed Data : Needs Smoothing and averaging)

```

## Anti-Aliasing Filter

An anti-aliasing filter prevents errors in digital signal processing by smoothing out rapid changes or high-frequency parts of analog signals before they are converted into digital form. This ensures accurate capture and avoids distortions that could occur due to the digital system not capturing fast changes properly. It's crucial in fields like audio recording, medical imaging, and telecommunications to maintain fidelity in signal conversion.

## FTT(Fast Fourier Transformer)

FFT converts signals into their frequency components, showing what frequencies are present and their strengths. It's fast and essential for analyzing sound, electrical signals, and images. By revealing patterns and enabling precise filtering, FFT supports tasks like noise reduction and signal processing in fields from music to telecommunications.

## Circuit Diagram

[circuit_Diagram.pdf](https://prod-files-secure.s3.us-west-2.amazonaws.com/bcd4ab55-e796-4c9b-ba77-e19b718558c1/520ae94c-47fc-4c38-8d18-59c5096be3f5/circuit_Diagram.pdf)

## Operational Amplifier(Op-Amp)

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/bcd4ab55-e796-4c9b-ba77-e19b718558c1/eba1f259-d3f2-47c2-a6d7-f95eaa856697/Untitled.png)

common op-amp : LM741

### Op-Amps

An operational amplifier, or op-amp, is a small electronic component that amplifies (increases) the strength of electrical signals. It's designed to take in a weak signal and output a much stronger version of that signal. Op-amps are built using transistors, resistors, and capacitors inside a single integrated circuit (IC) package.

### How Op-Amps Work

Op-amps typically have two main inputs:

- **Inverting Input (-):** This input reduces the signal amplitude or inverts its phase (flips it upside down).
- **Non-Inverting Input (+):** This input keeps the signal's original amplitude and phase.

The op-amp compares the voltages at these two inputs and multiplies the difference by a very large number (called the open-loop gain). This process greatly increases the signal strength at the output.

See this page for understanding op-amps properly.

[Mathematics](https://www.notion.so/Mathematics-34bff6493b16463f9aeca5930b13a542?pvs=21)

### Key Characteristics

1. **High Gain**
2. **Input and Output Impedance**
3. **Frequency Response**
4. **Power Supply**

### Negative Feedback

- **Purpose:** Negative feedback is used to stabilize the op-amp's gain, linearity, bandwidth, and other characteristics.
- **Operation:** When a portion of the output voltage is fed back to the inverting input through a resistor network:
    - It reduces the overall gain of the op-amp circuit.
    - It reduces distortion and improves linearity.
    - It increases the stability of the op-amp by reducing sensitivity to variations in components and temperature.
    

### Benefits of Feedback

- **Improved Performance:** linearity, reduces distortion, and increases the bandwidth of the op-amp.
- **Stability**
- **Control**


# EMG Amplifier

Amplitude of EMG Signal : 0.1-5mV

Frequency of EMG Signal: 20-2000Hz , still there are few signals whose freq vary from 2k to 4k Hz.

Designed Amp gain : 2000

Designed bandpass cutoff frequency: 20Hz, 2kHz

Amplification in 2stages :

1st stage : 50 times

2nd stage: 40 times

since the lower cutoff frequency of bandpass filter is 20Hz , movement interacts are almost completely filtered out.

50Hz mains power interference are dealt with by driven right leg circuit.

Output signal - 0 to 5 V to be read by ADC 

This requires a DC level shifter to raise the negative amplitude of signal above zero, following two 9V batteries are used to power the circuit since mains power is not safe. the plus and minus 9V sources are then down regulated +- 5V then power the circuit

## Power Regulation

78105 and 79105 power regulator to down-regulate.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/bcd4ab55-e796-4c9b-ba77-e19b718558c1/05514e78-ee6c-4350-b734-7b0315515a68/Untitled.png)

## 

## Amplification

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/bcd4ab55-e796-4c9b-ba77-e19b718558c1/485795ed-91e5-4a16-b3d7-c53e810eab3e/Untitled.png)

## Bandpass filter

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/bcd4ab55-e796-4c9b-ba77-e19b718558c1/d978cea2-e08e-436d-8cd0-08c644240598/Untitled.png)

2 Sensing Electrode: Bicep of right arm

1 Reference Electrode: Forearms

apply periodic force on right arm and this gives the desired output signal.

[Driven Right Leg Circuit](https://www.notion.so/Driven-Right-Leg-Circuit-f58c898013e947df9197c724cfc4edbf?pvs=21)

## Amplification

The amplification the project used was in two stages:

- First stage Instrumentation amplifier: AD623 (https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf)
- Second stage inverting amplifier

### First Stage Instrumentation Amplifier

https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf(AD623)

Gain = 1 + 100k/Rg

Rg = 2.2kohm

Theoretical Gain = 1 + 100k/2.2k = 46.5

Tested Gain = 41.63

### Second Stage Inverting Amplifier

Operational Amplifier: https://www.ti.com/lit/ds/symlink/lm741.pdf?ts=1719889941716&ref_url=https%3A%2F%2Fwww.google.com%2F

![Screenshot 2024-07-02 at 2.45.05 PM.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5a217a00-4ffc-45e2-950a-26e07f38dade/6d75c064-e135-4a95-afd2-0bb9357104ec/Screenshot_2024-07-02_at_2.45.05_PM.png)

Rin = 8.2kohm

Rf = 330kohm

Theoretical Gain = 1 + Rf/Rin = 40.2

Tested Gain = 31.8

**Net Gain = 41.63 * 31.8 = 1323.834**

## Bandpass Filter

![1000x bandpass filter - EXG Pill](https://prod-files-secure.s3.us-west-2.amazonaws.com/5a217a00-4ffc-45e2-950a-26e07f38dade/0b909a4f-0155-4b09-9d84-0754d77d65d9/Screenshot_2024-07-02_at_2.55.47_PM.png)

1000x bandpass filter - EXG Pill

![Bandpass filter ](https://prod-files-secure.s3.us-west-2.amazonaws.com/5a217a00-4ffc-45e2-950a-26e07f38dade/c3db994b-5a5a-4937-a6a3-cf028f01d11f/Screenshot_2024-07-02_at_3.12.09_PM.png)

Bandpass filter 

The Bandpass filter used in the project(playlist)

R1 = 8.2kohm

C1 = 1uF

R2 = 330kohm

C2 = 0.22nF

Theoretical : 

Lower Cutoff = 19.4Hz

Upper Cutoff = 2.2kHz

## DC Level Shifter

Since the output of the whole circuit should fit the 0 - 5V requirement of the Analog to Digital COnverter(ADC) - The DC level of the amplified signal must be shifted.

![Screenshot 2024-07-02 at 3.27.02 PM.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5a217a00-4ffc-45e2-950a-26e07f38dade/98050f9b-c52b-4a3b-bdd3-a3814a17ffce/Screenshot_2024-07-02_at_3.27.02_PM.png)

The yellow wave is the input signal and blue is the desired output

To convert the 96mV peak-to-peak to 4V we will use a op-amp to amplify it by 41.67 gain factor.

We use the op-amp in non-imverting closed loop amplifier setup after connecting a coupling capacitor.

Gain = 41.67

1 + Rf / Rg = 41.67

Rg = 1kohm

Rf = 40.67kohm

![Screenshot 2024-07-02 at 3.37.25 PM.png](https://prod-files-secure.s3.us-west-2.amazonaws.com/5a217a00-4ffc-45e2-950a-26e07f38dade/28339773-7772-4a6e-84b7-cd8b92e5b1f8/Screenshot_2024-07-02_at_3.37.25_PM.png)

This is the design of the DC level shifter

## Integration Testing

The integration testing showed that the DC level shift failed as the DC signal genrated was close to zero.

The signal amplitude was about 10V

And the DC offset was found to be 158mV

The output DC level was found to be 0.

The integrated high pass filter in the second stage amplifier was not considered by the playlist in integration testing

Adding a DC level shifter after the high pass filter is needed( after second stage amplification

The output signal to noise ratio was expectional

If they had time:

- Modify the instrumentation amplifier gain to get the final output within 5V i.e half the gain.
- Add a DC level shifter at the end stage of the circuit to raise the DC offset.

75mV to 3V gain = 3000/ 75  = 40

1 + Rf/Rg = 40 

Rf / Rg = 39

Rf = 39kohm

Rg = 1kohm