
Basically Fourier Transforms helps to convert the y axis from voltage to frequency.
So in the EMG example we have a wave form which spikes when muscle contracts. Converting this voltage to frequency domain would filter out the steady state and highlight the contractions.

![[Pasted image 20240723121410.png]]
This will reduce the steady state noise.


Steps:
- We need to get n number of samples over a sampling time 
- sampling time: t2 - t1
- sampling frequency = 2 * v
- Send this n samples to the fast fourier transform which is a function defined for arduino

Limitations of Microcontrollers:
- Sampling frequency must be twice of the frequency of what we need to measure
- Sampling time should not interfere with other functions: Wifi, Serial, etc
- FFT Calculations are CPU intensive
- Frequency binning needs to be done manually


Binning of data means categorising the frequency into ranges.