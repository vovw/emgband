
# Setting up you Machine Learning Application

## Train/Dev/Test sets

Applied Machine Learnings is a Highly iterative:
- number of layers
- number of hidden units
- learning rates
- activation function
- etc.
Identifying the best value of all these hyper-parameters happens in a loop:
idea --> experiment --> code.


Traditionally we will divide our entire data into parts:
- Training set
- Hold-out cross validation/ development set
- Testing set


We test different models on training and dev sets. Then we take the model which works best on the dev set and test it out on the testing set.

Dev sets just needs to be big enough to quickly evaluate a algorithm and check which one does better.

**Training set >> test set > dev set**

98% train, 1%dev 1%test for  100000000 examples
For a smaller dataset: 60/20/20 is a good split.
Avoid mismatched train/test sets. Let the data come from the same source.
**Rule of thumb: Let the Dev and Test sets come from the same distribution**
Its sometimes okay not having a test set.

## Bias / Variance

![[Pasted image 20240726000224.png]]

This is what bias and variance means

Lets say we have:
- 1% Training set error
- 11% Dev set error
This is overfitting to training set.
High variance

If:
- 15% Training set
- 16% Dev set
This is under-fitting or high bias.

If the error is high in both Training and Dev set then it is high bias and high variance.
If the error is low then it is the just right condition.

Sometime there is a Base / Optimal error. If it is high then condition like condition 2 is classified as low bias and low variance.


## Basic Recipe for Machine Learning

After training the model initially we ask the question:
 **Does the model have high bias**
	 If yes:
	- Make a bigger model
	- Train longer
	- NN architecture search

**Do you have a Variance Problem**
We identify this by testing the model performance on the dev set
If we have a variance problem:
- More data
- Regularisation
- NN architecture search

There was something called as bias variance tradeoff in the early neural network stage. In which optimising one always hurt the other.

But now making a bigger model always improves the bias and getting more data always improves variance. Essentially eliminating the trade-off.

# Regularising your Neural Network


## Regularisation

$$
J(b) = 1/m \sum_{i=1}^{m} L(\hat y ^{i}, y^i)
$$
$$ w \in R^{n_x},b \in R$$
For regularisation we:
$$J(b) = 1/m \sum_{i=1}^{m}{ L(\hat y ^{i}, y^i)} + {\lambda/{2m}*||w|^2|_2} $$
norm of w: summation of wj squared from j=1 to N-1

We can omit the regularisation of b because it is only one number.
The regularisation we looked at right now is L2 regularisation.

L1: $$\lambda / 2m \sum_{i=1}^{n_x}|w_j|$$
lambda is the regularisation parameter. We tune it using our dev set.

*in programming exercises we will use lambd because lambda is a reserved keyword*

![[Pasted image 20240726003945.png]]

![[Pasted image 20240726004020.png]]
 this is the Frobenius Norm

### Computing gradient Descent:

dw = from backprop + $$\lambda/m *w^{[l]}$$
L2 regularisation is also called weight decay.

![[Pasted image 20240726004433.png]]


## Why does regularisation reduces overfitting

Lambda term penalises the weight matrices from being too large.
Because having a large lambda would incentivise having w[l] to be close to zero.
Thus this gives us a intuition to why we use a Forbenuis Norm.


Another intuition is based on us using tanh as the activation function

g(z) = tanh(z) 

If lambda the regularisation parameter is large then the weights w would be small.
And z = wa +b so z will be small and g(z) will be roughly linear.

And if every layer is linear then the whole network will only be able to compute a linear function which is not what we want. But this prevents overfitting(although some under-fitting might be introduced)

Remember to see J decrease with time then we have to use the new L2 Regularisation technique's J.


## Dropout Regularisation

Let's say you train a nn and see overfitting.

We go through the nn and set a certain probability of the elimination of each node. We eliminate certain nodes and their connections.
We then run backpropogation and gradient descent on this reduced nn.
Then for each example we repeat this elimination of nodes process. This is dropout regularisation.
Because this much smaller nn is being trained it is regularised.

### Implement Dropout "Inverted Dropout"

Illustrate with layer l = 3
```
d3 = np.random.rand(a3.shape[0],a3.shape[1]) < keep_prob \\ d3 is the dropout 
probability
\\ d3 is now either 0 or 1 so it tells if we want to keep a node or not
a3 = np.multiply(a3,d3)
```

a3 --> [50,1]
keep_prob = 0.8 
on an average we end up with 10 units = 0
z[4] = a[3]w[4] + b[4] and z[4] will be reduced by 20%
So we need to divide each value of a[3] by 0.8 so as to keep the values of a[3] the same.

a3 /= keep_prob keeps the testing time same .


## Understanding Dropout

**Intuition: Can't rely on any one feature, as it might be dropped out any time. So the model will be pushed to spreading out it's weights**

Unless the algorithm is overfitting I will not use dropout.
Mostly used in Computer vision.

Downside is that the cost function J is no more well defined as the number of nodes is changing.
So to have a good clarity firstly we turn-off dropout and plot J. Then turn it on and verify that dropout has not introduced any bug in the code.

## Other regularisation methods

1. Getting more data - can be expensive
2. Data augmentation - changing the data set and making extra fake dataset elements and this is relatively cheaper than just getting more data.
3. Early Stopping - As we run gradient descent we plot J on a graph and it should decrease monotonically. We now plot this J and our dev set error. What we should see is that the dev set error will go down and then increase later. Stopping it at the lowest time is something that can be done to avoid overfitting.


# Setting up the Optimisation Problem.

## Normalising Training Sets


Normalising your inputs is two steps:
- Subtract Mean
- Normalise the variance

If you are normalising the training data, use the same mu and sigma to normalise the testing set.
![[Pasted image 20240726185227.png]]

We have applied fft and are sending the same data to our model.

### Why Normalise data
![[Pasted image 20240726185424.png]]
Normalising is important if your features come from different sets.


## Vanishing or Exploding Gradients

Basically for a net where g(z) = z and for a large net any value of w above 1 will become huge and any below 1 will diminish when size increases.


To avoid this more the layers we have smaller value of w we want.

## Weight Initialisation 


```
w[l] = np.random.rand(shape) * np.sqrt(2/ n[l-1])
this is good for reLu activation function
```

this is how we initialise random weights. This term at the end avoid the vanishing or explosion of gradients.

![[Pasted image 20240726194529.png|500]]


## Numerical Approximation of Gradients

![[Pasted image 20240728124424.png]]

Instead of taking derivative from theta to theta + epsilon. We take the derivative from -epsilon to +epsilon

## Gradient Checking

Take all W and b vectors and combine them into one large vector called theta.
J(W1,b1,W2,b2....Wn,bn) = J(theta)
Take all dW and db vectors and combine them into one large vector called dTheta.

![[Pasted image 20240728125438.png]]


![[Pasted image 20240728131713.png]]
