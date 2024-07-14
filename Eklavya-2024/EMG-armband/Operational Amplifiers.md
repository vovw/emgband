[LM741 Op-Amp Datasheet](https://www.ti.com/lit/ds/symlink/lm741.pdf?ts=1719456827152&ref_url=https%253A%252F%252Fwww.google.com%252F)

[https://youtu.be/kbVqTMy8HMg?si=wYZcjq7WtePbBkJr](https://youtu.be/kbVqTMy8HMg?si=wYZcjq7WtePbBkJr)

[https://youtu.be/idJEMYhrIfs?si=IcTGbQddZbtF97se](https://youtu.be/idJEMYhrIfs?si=IcTGbQddZbtF97se)

# Introduction

  

- Opartional Amplifiers were created to do mathematical operations.
- Op-Amps could be turned into an integrator, addr,etc.
- It is a **differential amplifier**

  

# Design

![[Screenshot_2024-06-27_at_11.53.26_PM.png]]

An Op-Amp esssentially has **2 input signal pins**:

- Inverting and,
- Non-Inverting

(The names refer to the output singal produced when that input is used to do amplification)

It has an **output pin**

And it has DC input for modulating the gain

  

# Working

## Open Loop Configuration

- If there is even a millivolt of difference in the inverting and the non-inverting input, the op-amp will shoot the output to saturation.
- If the non-inverting terminal is applied with 1mV more volatage than the inverting terminal- the output will shoot and reach saturation with value equal to +Vsupply.
- In the inverse case tha output will shoot to saturation with a value eqaul to -Vsupply.
- The open loop configurations allows us to use Op-Amps as Comperators as well
- They are the building blocks for ADC using op-amps

  

## Closed Loop / Feedback Configuration

**In a feedback circuit: Internally the op-amp does whatever it can to keep the input voltage difference minimum**

### Inverting Feedback

![[Untitled.png]]

  

The image is the most basic negative feedback loop.

The rule about Op-Amps is that they try to mimimize the voltage difference between the inputs. So this circuit will just behave as a buffer.

Buffers a high impedence signal and gives a low impedence output which can drive huge currents in the circuit.

  

### Non-Inverting Feedback

  

![[Screenshot_2024-06-27_at_11.53.26_PM.png]]

Typical Application of Op-Amps(Non-Inverting Feedback)

  

This is the configuration that actually allows you to tame and modulate the gain the op-amp generates.

The gain in this configuration is : Av = (R2/R1) + 1