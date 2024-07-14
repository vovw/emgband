  

## Overview

EMG Signal:

- 0.1 to 5 mV signal
- 20 - 2000 Hz frequency

  

The amplifier made is designed to:

- Gain: 2000(Bandpass filter)
- Bandpass cutoff frequency: 20Hz to 2kHz

  

Stages of development:

- First stage instrumentation amplifier - 50x amplification
- Second stage amplifier - 40x amplification
- Bandpass filter
- Driven right leg circuit
- DC level shifter (Ouptut ust be in 0-5V range for the adc to read it, thus we use the dc level shifter to raise the negative component of the filter above zero)
- Power regulator(+-9V voltage sources are reduced to 5V and then the circuit is powered)

  

  

## Power Regulation

  

The circuit of the 78I05 voltage regulator is used to reduce the input voltage(DC) from 9V to 5V

[https://pdf1.alldatasheet.com/datasheet-pdf/download/977005/FOSHAN/78I05.html](https://pdf1.alldatasheet.com/datasheet-pdf/download/977005/FOSHAN/78I05.html)

![[Screenshot_2024-07-02_at_1.19.57_PM.png]]

  

## Driven Right Leg circuit

Aim: To lower the common mode voltage

### Common Mode Voltage

![[Screenshot_2024-07-02_at_1.26.49_PM.png]]

Here the common mode voltage is: Vcm = 1/2(V1+V2) [average of v1 and v2]

Difference voltage = v1-v2

V1 = Vcm + 1/2Vd

V2 = Vcm - 1/2Vd

![[Screenshot_2024-07-02_at_1.32.58_PM.png]]

The output voltage is dependent on common mode voltage and the difference voltage.

The gain for common mode voltage is one.

The gain for Vd is (1/2 + Rf/R1)

In general you do not want to have common mode voltage on the output side.

  

**For EMG amplifier**:

- The need to reduce the common mode voltage is to remove the common noise fom the system.

![[Screenshot_2024-07-02_at_1.41.58_PM.png]]

Here if both the terminals will have the same amount of noise there will be a Voutput of common mode.

Hence to reduce this output we use a Driven right leg circuit.

  

### Driven Right Leg circuit

The circuit is an addition to the instrumentation amplifier so that the cmr is reduced.

![[Screenshot_2024-07-02_at_2.29.06_PM.png]]

11x gain instrumentation amplifier- BioAmp EXG Pill

  

Such a addition circuit is added to this instrumentation amplifier to make it a DRL:

![[Screenshot_2024-07-02_at_2.31.54_PM.png]]

The values of Rf and Ro are taken around 5MOhms to limit the current that passes through.

The output of this entire circuit is taken as the difference of the lines from the two main diodes.

This circuit will give us the output with reduced common mode voltage.

  

  

## Amplification

  

The amplification the project used was in two stages:

- First stage Instrumentation amplifier: AD623 ([https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf))
- Second stage inverting amplifier

  

### First Stage Instrumentation Amplifier

[https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf](https://www.analog.com/media/en/technical-documentation/data-sheets/ad623.pdf)(AD623)

Gain = 1 + 100k/Rg

Rg = 2.2kohm

Theoretical Gain = 1 + 100k/2.2k = 46.5

Tested Gain = 41.63

  

### Second Stage Inverting Amplifier

Operational Amplifier: [https://www.ti.com/lit/ds/symlink/lm741.pdf?ts=1719889941716&ref_url=https%3A%2F%2Fwww.google.com%2F](https://www.ti.com/lit/ds/symlink/lm741.pdf?ts=1719889941716&ref_url=https%3A%2F%2Fwww.google.com%2F)

![[Screenshot_2024-07-02_at_2.45.05_PM.png]]

Rin = 8.2kohm

Rf = 330kohm

Theoretical Gain = 1 + Rf/Rin = 40.2

Tested Gain = 31.8

  

==**Net Gain = 41.63 * 31.8 = 1323.834**==

  

  

## Bandpass Filter

  

![[Screenshot_2024-07-02_at_2.55.47_PM.png]]

1000x bandpass filter - EXG Pill

![[Screenshot_2024-07-02_at_3.12.09_PM.png]]

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

  

![[Screenshot_2024-07-02_at_3.27.02_PM.png]]

The yellow wave is the input signal and blue is the desired output

  

To convert the 96mV peak-to-peak to 4V we will use a op-amp to amplify it by 41.67 gain factor.

We use the op-amp in non-imverting closed loop amplifier setup after connecting a coupling capacitor.

Gain = 41.67

1 + Rf / Rg = 41.67

Rg = 1kohm

Rf = 40.67kohm

  

![[Screenshot_2024-07-02_at_3.37.25_PM.png]]

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