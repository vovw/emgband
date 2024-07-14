# Overview

  

We will be making a One hidden layer nerual network

Last week we made a neuron which could do the z =wtx +b and a = sig(z) and L(x,y)

How we will be making a network with layers of neurons

![[Screenshot_2024-07-11_at_11.27.29_PM 1.png|Screenshot_2024-07-11_at_11.27.29_PM 1.png]]

This is what we will be making where the layer will be corresponding to a z and a calculation

  

![[Screenshot_2024-07-11_at_11.30.03_PM 1.png|Screenshot_2024-07-11_at_11.30.03_PM 1.png]]

This is the notation we will use for multiple layered neural network. []superscript refers to the number of the layer

  

# Neural Network Representation

  

![[Screenshot_2024-07-11_at_11.38.37_PM 2.png|Screenshot_2024-07-11_at_11.38.37_PM 2.png]]

This is how a neural networks is represented.

  

- Parameters like w and b are associated with each layer
    - w→ (4,3) for a[1] because 4 nodes in hidden layer and 3 in input layer
    - b —>(4,1)

Every line represents the work we did in the last week.

And every node in the neural network has a value which is just a z of that line added together.

  

# Computing a Neural Network’s Output

  

A neural network is just the logistic regression we did but multiple times

  

![[Screenshot_2024-07-11_at_11.45.14_PM 1.png|Screenshot_2024-07-11_at_11.45.14_PM 1.png]]

This is what every node in a neural network does.

![[Screenshot_2024-07-11_at_11.46.50_PM 1.png|Screenshot_2024-07-11_at_11.46.50_PM 1.png]]

this is the notation used

  

![[Screenshot_2024-07-11_at_11.38.37_PM 1 1.png|Screenshot_2024-07-11_at_11.38.37_PM 1 1.png]]

The neural network show in a 2 layerd neural network - input layer is not counted as a neural network layer

  

For every a^[1]:

- w is a 4,3 vector
- b is a 4,1 vector
- z is a 4,1 vector

  

z1[1] = w1[1]*x + b1[1]

z2[1] = w2[1]*x + b2[1]

z3[1] = w3[1]*x + b3[1]

z4[1] = w4[1]*x + b4[1]

  

This is how computation is done for one layer of the neural network

a1[1]…a4[1] = sigmoid(z[1])

z[1] is a 4,1 vector of all the z values for layer 1

  

![[Screenshot_2024-07-12_at_1.24.05_PM 1.png|Screenshot_2024-07-12_at_1.24.05_PM 1.png]]

  

# Vectorizing across multiple examples

  

Stacking up the training examples across columns of a matrix we will be able to apply the equations from previous videos. And with very little modifications change them for the neural network to compute on every example.

  

![[Screenshot_2024-07-12_at_1.39.46_PM 1.png|Screenshot_2024-07-12_at_1.39.46_PM 1.png]]

These are the equations from the previous video

- x1 —> a[2](1) —> y
- x2 —> a[2](2) —> y
- .
- .
- .
- xm —> a[2](m) —> y

The network will go through the m training examples and compute the a[2] i.e the a of the second layer on each of them

for i in range(m):

z[1]i = W[1]X1 + b[1]

a[1]i = sigmoid(z[1]i)

z[2]i = W[2]a[1]i + b[2]

a[2]i = sigmoid(z[2]i)

  

We have to get rid of the for loop and vectorize it.

X is (nx,m) so each training example is stacked in columns.

Z[1] = W[1]X + b[1]

A[1] = sigmoid(Z[1])

Z[2] = W[2]A[1] + b[2]

A[2] = sigmoid(Z[2])

  

![[Screenshot_2024-07-12_at_1.51.21_PM 1.png|Screenshot_2024-07-12_at_1.51.21_PM 1.png]]

  

Horizontally we index through training examples

Vertically we index through the number of nodes.

  

We can think of making it a tensor and the depth dimension could be associated to each layer of the neural network.

  

# Activation Functions

  

When building neural networks one of the coices you get to make are choosing the activation function in the hidden layers and output of the neural networks.

  

a = sigmoid(z)

but we can have: a = g(z)

A activation function that almost always works better than sigmoid is the tanh:

## tanh (z)

  

![[Screenshot_2024-07-12_at_4.23.35_PM 1.png|Screenshot_2024-07-12_at_4.23.35_PM 1.png]]

This is how the tanh function looks.

It is mathematically just a shifted sigmoid.

tanh has a tendancy to centre your data. Because the mean of the tanh function is 0. This makes learning better for the next layer.

  

tanh is usually superior than sigmoid. One exception is in the output layer you would want the output to be a number between 0 and 1 rather than between -1 and 1.Especially for binary classification.

Activation functions can be different for different functions. So tanh for hidden layer and sigmoid for the output layer works.

  

A disadvantage of the tanh or sigmoid is that if the z is very large then the slope of the function becomes 0 which slows donw the learning process.An alternative for this is:

## ReLU- Rectified Linear Unit

![[Screenshot_2024-07-12_at_4.29.59_PM 1.png|Screenshot_2024-07-12_at_4.29.59_PM 1.png]]

  

  

Rules of thumd for choosing activation function:

- If output is 0 or 1 then sigmoid is a obvious choice
- For all other processes the rectified linear unit is a good choice
- Sometimes you can use tanh

One disadvantage of the ReLU is that the derivative is negative when z is negative.An alternate is:

  

## Leaky ReLU

![[Screenshot_2024-07-12_at_4.32.18_PM 1.png|Screenshot_2024-07-12_at_4.32.18_PM 1.png]]

This exists but relu is also a good choice. Most of the times for z>0 ReLU is the actvation function best for learning.

Majority times in our neural net z > 0 and for a positive z relu has a non-zero slope

  

## Pros and Cons

  

- Sigmoid - Only for binary classification
- tanh - Used for hidden units
- ReLU- Should be the default
- Leaky ReLU - Could be considered an alternative for ReLU

  

# Why do we need activation functions

  

Let’s say if a = g(z) = z

I.E the activation function is and identity function.

  

In this case the model is computing yhat as a linear function of the input features i.e x.

  

a1 = w1x + b1

a2 = w2a1 + b2

a2 = w2*w1*x + w2b1 + b2

this is jsut y = mx + c

  

If you were to use linear activattion function or identity function then the output yhat will be a linear relation of X.

  

The one place when you could use a linear activation function is in the output layer

  

# Derivative of activation functions

  

## Sigmoid

  

g(z) = 1 / (1+e^-z)

  

dg/dz = g(z)(1-g(z))

  

g’(z) = a(1-a)

  

## tanh

  

g(z) = (e^z - e^-z) / (e^z + e^-z)

g’(z) = 1 - tanh^2(z)

  

## ReLU

  

g(z) = max(0,a)

  

g’(z) = 0 if z < 0 | 1 if z > 0 | unefined if z = 0

  

## Leaky ReLU

  

g(z) = max(0.01z, z)

  

g’(z) = 0.01 if z <0 or 1 if z > 0

  

  

# Gradient Descent for Neural Network

  

Parameters: w[1],b[1[,w[2],b[2]

  

n[0] —> input features

n[1] —> hidden units

n[2] —> output labels

  

w[1] = (n[1],n[0])

b[1] = (n[1], 1)

w[2] = (n[2],n[1])

b[1] = (n[2],1)

  

Cost Function (assuming binary classification) = 1/m sum(L(yhat,y)) yhat = a[2]

  

Gradient Descent:

- Initialize the parameters to random values
- Repeat:
    - Compute Preds: yhat 1,2,3,…m
    - dw[1] = dJ/dw, db[1] = dJ/db
    - w[1] = w[1] - alpha*dw[1]
    - b[1] = b[1] - alpha*db[1]

  

  

## Equations

### Forward Propogation

- Z[1] = W[1]X + b[1]
- A[1] = sigmoid(Z[1])
- Z[2] = W[2]A[1] + b[2]
- A[2] = sigmoid(Z[2])

  

### Backward Prropogation

- **dZ[2] = A[2] - Y**
- **dW[2] = dZ[2]A[1].T / m**
- **db[2] = 1/m np.sum(dZ[2],axis = 1, keepdims = True)** keepdims ensures python output a n[2],1 matrix and not a (n[2],) array
- **dZ[1] = W[2]*dZ[2] * g[1]’Z[1]** where g[1]’ is the derivative of the activation function used. W[2]*dz[2] will give a n[1], m and same shape will be g[1]’Z[1] so the * in between is elementwise
- **db[1] = 1/m np.sum(dz[1],axis = 1, keepdims = True)**

  

**foo and d(foo) have the same dimensions always**

  

![[Screenshot_2024-07-13_at_12.22.33_AM.png]]

Backpropogation equation for one training example

  

![[Screenshot_2024-07-13_at_12.32.33_AM.png]]

Vectorized implementation of backpropogation

  

# Random Initialization

  

If we initialize w[1] and b[1] to 0.

Then a[1] would all be zero

so would a[2] be 0

and now all the parameters would be symmetric

  

Both the hidden units will compute exactly the same function. This makes the multiple hidden units redundent.

  

![[Screenshot_2024-07-13_at_12.53.54_AM.png]]

  

W[1] = np.random.randn((2,2)) * 0.01

b[1] = np.zeros((2,1))

**Why is 0.01, 0.01**

- We tend to initialize the weights to very small values
- When we z it will be very large and you are more likely to end up in the parts where slope is 0 for sigmoid or tanh activation functions
- If you do not have any sigmoid or tanh then it is not a issue but for binary classification it becomes important.