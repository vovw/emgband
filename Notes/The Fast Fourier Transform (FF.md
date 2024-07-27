---
title: The Fast Fourier Transform (FF

---

# FFT

https://youtu.be/spUNpyF58BY?si=sagmahwpWOHDw7dS
https://youtu.be/qz0MLVh7Gok?si=3Ny0icUNbavIlHg5
https://youtu.be/PRUn5Alm6Vk?si=5fMewBNEyoC2k-YJ
https://youtu.be/tgH-a-vaiq8?si=d1YMxc0TO2VAYZPQ

## How will we use
Basically Fourier Transforms helps to convert the y axis from voltage to frequency. So in the EMG example we have a wave form which spikes when muscle contracts. Converting this voltage to frequency domain would filter out the steady state and highlight the contractions.

### Steps:

- We need to get n number of samples over a sampling time
- sampling time: t2 - t1
- sampling frequency = 2 * v
- Send this n samples to the fast fourier transform which is a function defined for arduino
### Limitations of Microcontrollers:

- Sampling frequency must be twice of the frequency of what we need to measure
- Sampling time should not interfere with other functions: Wifi, Serial, etc
- FFT Calculations are CPU intensive
- Frequency binning needs to be done manually


## Actual Meaning
The Fast Fourier Transform (FFT) is an algorithm to compute the Discrete Fourier Transform (DFT) and its inverse. The DFT is a mathematical technique used in signal processing and many other fields to transform a sequence of values into components of different frequencies. The FFT significantly reduces the computational complexity compared to directly computing the DFT, making it more efficient for practical use. Here are some key points about FFT:

Key Concepts
1. Discrete Fourier Transform (DFT):

- Converts a finite sequence of equally spaced samples of a function into a same-length sequence of equally spaced samples of the discrete-time Fourier transform (DTFT).
- Given a sequence x[n] of length n, the DFT is defined as:
![image](https://hackmd.io/_uploads/ByDxynbYC.png)
-X[k] represents the amplitude and phase of the k-th frequency component of the sequence.

2. Inverse DFT (IDFT):
- Converts the frequency domain representation back to the time domain:
![image](https://hackmd.io/_uploads/H1II1n-KA.png)

3. Computational Complexity:

- Direct computation of the DFT requires 𝑂(𝑁2) operations.
- FFT reduces this to 𝑂(𝑁log𝑁), making it much faster,especially for large N.


## Applications
1. Signal Processing:

- Filtering, spectral analysis, and feature extraction.
- Used in audio, image, and video processing.

2. Communications:
-  Modulation and demodulation schemes, error detection, and correction.
- OFDM (Orthogonal Frequency Division Multiplexing) in wireless communications relies heavily on FFT.

3. Scientific Computing:
- Solving partial differential equations, numerical simulations, and data analysis.
4. Image Processing:
- Image compression (JPEG uses DCT, a variant of FFT), image reconstruction, and enhancement.