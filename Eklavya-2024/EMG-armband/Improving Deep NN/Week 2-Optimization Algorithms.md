

# Mini-Batch Gradient Descent

Breaking down all your training examples into smaller sets. Applying gradient descent on these smaller batches individually

![[Pasted image 20240728214756.png]]

To run mini-batch gradient descent we would:

for t = 1,....5000
	1 step of gradient descent using X^{t} and Y^{t} 
 Instead of writing a for loop for all 5,000,000 examples we use vectorization to make it faster
![[Pasted image 20240728215459.png]]

When you have a large training set mini-batch gradient descent runs much faster than regular gradient descent.


When our Mini-batch size is m which is equal to the number of training examples, that is just batch gradient descent
When mini-batch size equals one that is called stochastic gradient descent where a single example is looked at and gradient descent is calculated.
Anything in between is optimal for a more general sized dataset.


# Exponentially Weighted Averages

Looking at the graph of temperature in London through some days we can plot this graph:
![[Pasted image 20240728231016.png]]

V(t) = B* V(t-1) + (1-B)O(t)
This is the formula we use to calculate a moving average of the data *blue points*

V(t) = averaging over 1/1-B days temperature

When B = 0.98 V(t) averages over last 50 days temperature- this is the green plot

When B = 0.2 We see a graph with high variance and - yellow plot

This formula is how we implement exponentially weighted moving average:
 
V(t) = B* V(t-1) + (1-B)O(t)
B is also a hyperparameter.
![[Pasted image 20240728231522.png]]

![[Pasted image 20240728233440.png]]
![[Pasted image 20240728233738.png]]


When implementing the moving average we will not get the green curve for a high beta.
We will get the purple curve
![[Pasted image 20240728234641.png]]
For B = 0.98
V0 = 0
V1 = 0.98O(1)
V2 = 0.98V1 + 0.02O(2)
V2 = 0.0196O(1) + 0.02 O(2)
V2 << O(1) or O(2) so it is not a accurate estimate of the actual data.

correction V(t) / (1 - B^t)

when t = 2
1 - B^t  = 0.0396

V2/ 0.0396 =  0.0196O(1) + 0.02 O(2)
This becomes a weighted average and removes the bias. This is significant for when t is small. When t is large the 1 - B^t will be close to 1 as B < 1.


# Gradient Descent with Momentum


 We basically perform the moving average on the gradients we get from normal gradient descent. 
 V(dw) = BV(dw) + (1-B)dW
 and then W = W - a* V(dw)
![[Pasted image 20240729231432.png]]


# RMSprop

![[Pasted image 20240729232012.png]]

# Adam optimization Algorithm

ADAM: Adaptive moment Optimization

![[Pasted image 20240729232659.png]]

## Hyperparameters choice:

![[Pasted image 20240729232830.png]]

# Learning Rate Decay

![[Pasted image 20240729233628.png]]

![[Pasted image 20240729233645.png]]

