
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


# Spectral Methods

The continuous Fourier Transform of a function \( f(t) \) is given by:

$$
F(\omega) = \int_{-\omega}^{\omega} f(t) e^{-i \omega t} \, dt
$$

$$
f(\omega) = 1/2\pi \int_{-\omega}^{\omega}e^{ikx}F(k)\,dk
$$

$$
\hat F^{(n)} = (ik)^{n} \hat f
$$

# Cooley- Tukey Algorithm

This algorithm is the most widely-used fft algorithm.
It breaks down DFT into smaller DFTs which are combined together.

This brings to complexity of the algorithm from $$O(n^2)\  \ to \  \ O(log(n))$$
DFT converts N complex numbers x0,x1,x2....xN-1 to another sequence X0,X1,X2...XN-1.
$$
X_k = \sum_{n=0}^{N-1} x_n e^{-i2\pi kn /N}
$$
for k = 0,1,2,...N-1

The Cooley-Tukey Algorithm takes a divide and conquer approach to this DFT.


1. **Divide Step**
 $$x_{even} = (x_0,x_2,x_4,...,x_{N-2})$$
$$ x_{odd} = (x_1,x_3,x_5,...x_{N-1})$$
2. **Conquer Step**
	 Compute the DFT of x_even and x_odd recursively.

3. **Combine Step**
	1. Combine the results of the smaller DFTs to get the DFT of the original sequence.
	2. For each k from 0 to N/2 - 1:
		1. $$ X_k = X_{even,k} + e^{-i2\pi k /N}X_(odd,k)$$
		2. $$X_{k+N/2} = X_{even,k} - e^{-i2\pi k/N}X_{odd,k}$$


**The code basically does this operations by breaking the samples array down into many such smaller subsets and combining them after computing the equations above.**