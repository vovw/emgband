  

  

# Revision of basics: EMG sensor structure

An EMG Sensor has 3 electrodes.

- Reference.
- Anode
- Cathode

We take readings based on these three eletrodes.

The sensor detects the electric potential produced by the muscles of the body. We place the reference electrode in a region close to the actual measuring muscle but not on it. So that it detects the constant change but not the change due to conscious muscle movement.

After the referecne electrode is placed the anode and cathode are placed near the muscle (usually ends of the biceps).

Reading of reference = S1

Reading of Anode Cathode = S2

  

Raw reading = S2 - S1

  

  

# EMG Circuit for Microcontroller

  

## 1. Power Circuit

  

We know how to make a positvie and a negative line using batteries. Now let’s understand how to make a negative line(-xV).

We have 2 batteries 9V each. We want 3 lines:

- +9V
- GND
- -9V

Connecting the batteries in series we get net 18V output.

**positive of one to the negative of another is how we make a series circuit**

To make any circuit we need a reference point(gnd). usually it is the negative terminal of the battery. Consider the postive to negative terminal joint as the reference. Now the difference of voltage between the postive and the new reference will gie us +9V and the other batteries negative will give us the -9V with the new reference

  

**Potential Difference is the keyword**

  

## 2. Signal acquisition

  

**Compoent used: INA106**

[https://pdf1.alldatasheet.com/datasheet-pdf/view/540147/TI1/INA106.html](https://pdf1.alldatasheet.com/datasheet-pdf/view/540147/TI1/INA106.html)

  

The op-amp is used in a open loop configuration with the differential of 9V

The componet is a basic OP AMP with a G = 110.

We make the connections :

- GND pin to GND terminal
- Pin 5 and pin 6 connected by a 1M ohm resistor
- Pin 1 connected to the ground using another 1M Ohm resistor

![[Screenshot_2024-07-01_at_12.11.53_AM.png]]

IMA106

  

## 3. Amplification

  

For amplification we use the TL072 IC which is again an op-amp ic.

[https://www.ti.com/lit/ds/symlink/tl072.pdf?ts=1719767301650&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTL072%3Fbm-verify%3DAAQAAAAJ_____7Jnb22C5ObK4z4pWQ5jXjW_B-EK47w5fpE4D9n_czPKAOLH9UPDZ-Ff2pn8hayr14_ItrnjxEtKnhJ05HIuFUJqPjw7mW4DVyeyjT5Xx-ALn8bhlqNWpwWoisXgG7jXctOgv_VOj2wYAMkjWlWpw0H0YWt4ZrQJ7_WOdEdzdk7Fls_g1RzjHjScfYde2-MEqfBPDeSHqQElJYEliXvYRMrQ3dj55h-cP8Zo2qugQbCmNPD90kjuYwNONfimICNqYf8g7Fuq5KOMEQP5Fn_K2pfYG9Ey8SL0CkiaZErWfWU](https://www.ti.com/lit/ds/symlink/tl072.pdf?ts=1719767301650&ref_url=https%3A%2F%2Fwww.ti.com%2Fproduct%2FTL072%3Fbm-verify%3DAAQAAAAJ_____7Jnb22C5ObK4z4pWQ5jXjW_B-EK47w5fpE4D9n_czPKAOLH9UPDZ-Ff2pn8hayr14_ItrnjxEtKnhJ05HIuFUJqPjw7mW4DVyeyjT5Xx-ALn8bhlqNWpwWoisXgG7jXctOgv_VOj2wYAMkjWlWpw0H0YWt4ZrQJ7_WOdEdzdk7Fls_g1RzjHjScfYde2-MEqfBPDeSHqQElJYEliXvYRMrQ3dj55h-cP8Zo2qugQbCmNPD90kjuYwNONfimICNqYf8g7Fuq5KOMEQP5Fn_K2pfYG9Ey8SL0CkiaZErWfWU)(datasheet)

  

- We connect the output of the IMA106 to the inverting input of the TL072 IC using a 10kOhm resistor.
- Connect the pin 6 and 7 of the IC using a 150kOhm resistor.
- Gain = -R2/R1 = -150 / 10 = -15
- We use this IC as an inverting amplifier

AC Coupling - We will add a capacitor to perform ac coupling on the signal.

  

**High Pass Filter**

- Connect the 2OUT pin of the IC to the 0.01uF capcatior and 150kOhm will connect the other end to the 2 PIN(1IN-) of the IC
- Connect the Pin 1 and 2( 1IN+ and 1IN-) together using the 150kOhm resistor.

  

Now we connect the pin 4 to -9V, pin 8 to +9V and 3 and 5 pin to gnd(Reason - inverting amplifier).

  

## 4. Signal Conditioning- Rectification