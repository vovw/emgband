## emg - electromyograph
- used for measuring electrical impluses from muscular activity
- frequencies = 0hz to 500 hz
- flow 
surface electrode -> Amplifier -> filter -> ADC -> FFT -> bandpass filter -> full wave rectifier -> smoothing and moving average -> processed emg data

### ADC 
- analog to digital converter
- 16big or 8 bit sampling
- done in da microcontroller

### FFT and bandpass filter
- done in da microcontroller
- find the cutoff frequencies

- $`W = f_{high} - f_{low}`$

- $`f_0 = \frac{f_{high} + f_{low}}{2}`$

- $`|H_{BP}(f)|^2 = \frac{1}{1 + \left(\frac{\left(\frac{f}{f_0} - \frac{f_0}{f}\right)^2}{W^2}\right)^N}`$


--------------------------------------------------------------------
### Operational Amplifiers (Op-Amps):
1. Definition: electronic component that amplifies the difference between two input voltages. typically represented as a triangle in circuit diagrams.
2. Inputs: 
   - Has two inputs: inverting (-) and non-inverting (+)
   - High input impedance, meaning it barely affects the input signals

3. Output:
   - Single output
   - Can produce a much larger voltage than the input difference

4. Key characteristics:
   - Very high gain (amplification factor)
   - Wide bandwidth (works across a broad range of frequencies)
   - Low output impedance (can drive various loads)

5. Ideal op-amp assumptions:
   - Infinite open-loop gain
   - Infinite input impedance
   - Zero output impedance
   - Zero input offset voltage

6. Common uses:
   - Amplification
   - Filtering
   - Comparison
   - Mathematical operations (addition, subtraction, integration, differentiation)

7. Feedback:
   - Often used with negative feedback to control gain and improve stability

### EMG Amplifier Explanation
An EMG amplifier is like a super-sensitive listener for your muscles.

1. **Picking up tiny signals**: 
   - Your muscles produce very small electrical signals when they move or tense up.
   - These signals are too small for normal equipment to detect clearly.

2. **Amplification**:
   - The EMG amplifier uses op-amps to make these tiny signals much bigger.
   - like turning up the volume on very quiet muscle whispers.

3. **Filtering out noise**:
   - The amplifier also includes filters to remove unwanted signals (noise).
   - This is like cleaning up a fuzzy radio signal to hear the music clearly.

4. **Differential amplification**:
   - EMG amplifiers usually use a special setup called differential amplification.
   - This compares the signal from the muscle to a nearby "quiet" area.
   - It helps cancel out common background noise, leaving mostly the muscle signal.

5. **High input impedance**:
   - The amplifier is designed not to disturb the tiny muscle signals.
   - It's like listening without putting your ear right up against the skin.

6. **Output**:
   - The amplified and cleaned-up signal can then be displayed on a screen, recorded, or used to control devices.

--------------------------------------------------------------------
#### band pass filter
allows signals within a specific frequency range to pass through while attenuating signals outside this range. It combines a high-pass and low-pass filter, defined by:
1. Center frequency (f₀): The midpoint of the passband
2. Bandwidth (W): The width of the passband
3. Quality factor (Q): Selectivity of the filter (f₀/W)


#### Active Low-Pass Filter:

An active low-pass filter allows signals below a cutoff frequency to pass while attenuating higher frequencies. Key features:

1. Cutoff frequency (fc): The frequency at which signal attenuation begins
2. Roll-off rate: typically -20 dB/decade for first-order filters
3. Uses op-amps for amplification and improved performance

#### Active High-Pass Filter:

An active high-pass filter allows signals above a cutoff frequency to pass while attenuating lower frequencies. Key features:

1. Cutoff frequency (fc): The frequency above which signals pass through
2. Roll-off rate: typically -20 dB/decade for first-order filters
3. Uses op-amps for amplification and improved performance

### RC Low Pass Filter Explained
simple circuit that passes low-frequency signals and attenuates high-frequency signals.
1. Operation:
   - At low frequencies: Capacitor acts as an open circuit
   - At high frequencies: Capacitor acts as a short circuit

2. Cutoff Frequency (fc):
   - fc = 1 / (2π * R * C)
   - Signals above fc are attenuated

3. Characteristics:
   - Roll-off: -20 dB/decade (-6 dB/octave)
   - Phase shift: 0° at DC to -90° at high frequencies

4. Advantages:
   - Simple design
   - Passive (no power required)
   - Inexpensive

5. Limitations:
   - Fixed attenuation rate
   - Loading effects on the signal source

[video of full walkthrough](https://www.youtube.com/watch?v=ucwtJ28Yia0)
