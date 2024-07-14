  

  

# Binary Classification

  

Binary Classification is basically saying if a given data is a or b.

Let’s say we have a 64x64 image and we have to predict if it is a image of a cat.

Then we define a feature vector X which has all the pixels’ values untill we get a very long feature vector. So we get X having 64*64*3(color channels) = 12288 values. nx = 12288 or n = 12288.

Every training example is remresented by (x,y) where x = n dimensional feuature vector and y is either 0 or 1.

  

![[Screenshot_2024-07-06_at_3.18.24_PM.png]]

  

Capital X = all the training examples as columns of the training examples

![[Screenshot_2024-07-06_at_3.20.21_PM.png]]

  

X.shape = n x m( n = shape of each training example, m = # training examples)

![[Screenshot_2024-07-06_at_3.21.29_PM.png]]

  

  

# Logistic Regression

  

**Logistic Regression**: Learning algorithm you use when the output labels in a problem are 0 or 1.

  

Given an feautres vector x we want y^ = P( y = 1 | x)

y(hat) is the probability of y = 1 given x.

  

Parametes:

- w(weight): R(nx)
- b(bias): R

What does not work is yhat = wT *x + b this is what we do in linear regression

but yhat is the probability so:

  

y hat = sigmoid(z) where z → wT *x + b

  

sigmoid(z) = 1/(1+e^-z)

  

![[Screenshot_2024-07-06_at_3.30.21_PM.png]]

![[Screenshot_2024-07-06_at_3.30.06_PM.png]]

  

In logistic regress our job is to update w and b so that yhat becomes a good estimate of y becoming equal to 1.

**When we program neural networks we will keep w and b separate, where b corresponds to an inter-spectrum**

  

# Logistic Regression Cost Function

  

## Loss or error Function

  

L(y,yhat) could be 1/2(y-yh)^2 but this makes the optimization problem have a lot of local minima.

  

This L is the function we define to ouput how close the yh is for a given y.

So we define a different loss function that plays a simillar role to half squared error but gives a comvex optimization problem

  

L(y,yh) = -( ylogyh +(1-y)log(1-yh))

**If y = 1, L = -logyh**

- We want L to be minimum
- log yh should be as big as possible
- yh should be as big as possible
- since yh is a sigmoid it’s max value is 1
- Hence for y = 1 we want the yh = 1 for the L to be minimum

  

**If y = 0, L = -log(1-yh)**

- We want L to be minimum
- log(1-yh) should be as big as possible
- 1-yh should be as big as possible
- yh is positive always as it is a sigmoid
- Hence for y = 0 we want the yh = 0 for the L to be minimum

  

  

The cost function is :

![[Screenshot_2024-07-06_at_3.52.51_PM.png]]

We optimize the w and b such that J is as small as possible.

  

# Gradient Descent

  

For a convex Cost Function: Gradient Descent is the Algorithm that takes the biggest step in the direction of the global minima.

  

![[Screenshot_2024-07-06_at_4.09.09_PM.png]]

  

Repeat: {

w = w - alpha dJ(w)/dw // for a J vs w graph

}

alpha is the learning rate and dctates how big a jump we want to make when implementing the gradient descent algorithm

  

When writing code the convention we follow is we represent dJ/dw as dw

  

so **w = w - a dw**

![[Screenshot_2024-07-06_at_4.17.32_PM.png]]

This is the gradient descent algorithm.

  

# Computation Graph

  

Computations of a neural network are:

- Forward Pass
- Backpropogation

  

J(a,b,c) = 3(a + bc)

  

- u = bc
- v = a + u
- J = 3v

![[Screenshot_2024-07-07_at_1.32.30_AM.png]]

With a left to right pass we can calulaate J

With a right to left pass we can get the computation of Derivatives

  

# Derivatives with a Computational Graph

  

![[Screenshot_2024-07-07_at_1.36.39_AM.png]]

In this computational graph:

dJ/dv = 3

  

This is one step of Backward propogation

We now understand how changing the values of a,b,c,u,v will affect J

To find dJ/da we will use the chain rule and get that:

- dv/da = 1
- dJ/dv = 3
- dJ/da = 3

  

This is the backbone of Backpropogation.

  

d(var) will represent the final output variable you care about e.g. J or the cost function in the code.

  

du = dJ dv = dJ.1 = 3

dc and db can now also be derived.

**The most efficient way to compute these derivatives is following a right to left approach**

  

# Logistic Regression Gradient Descent

  

**How to compute derivates for gradient descent for logistic regression**

  

- z = w^t * x + b
- y^ = a = sig(z)
- L(a,y) = -(ylog(a) + (1-y)log(1-a))

We have to update w and b so that L decreases

![[Screenshot_2024-07-07_at_11.48.47_AM.png]]

  

Now using this we can calculate:

- da
- dz
- dw1
- dw2
- db

  

w1 = w1 - alpha(dw1)

w2 = w2 - alpha(dw2)

b = b - alpha(db)

  

# Gradient Descent on m examples

  

J = 1/m*(average of loss function L(a,y))

a^i = y^i = sig(z) = sig(wi*x + b)

  

dJ/dw1 = 1/m sigma(dw1^i)

dw1^i = the derivative for a single training example (calculated in the last video)

  

## Algorithm for gradient descent on m examples:

J = 0, dw1 = 0, dw2 = 0, db = 0

For i = 1 to m

![[Screenshot_2024-07-07_at_12.08.42_PM.png]]

![[Screenshot_2024-07-07_at_12.11.04_PM.png]]

By doing the summation as shown in image1 we will get dw1 to be dj/dw1

and then we can compute the w1 w2 and b.

Everything in the two images is done for every training example.

For more features you have dw1 dw2 …. dwn.

  

There are certain techiniques such as **vectorization** techiniques. They help you to get rid of these explicit for loops.

  

  

# Vectorization

  

Vectorization is: the odds of getting rid of explicit for loops.

  

If you want to do z = w^T * x + b:

- We might write a for loop and say z += w[nx - i] * x[i]
- z += b
- Vectorization allows you to write z = np.dot(w,x) which essentically does w transpose * x.

  

![[Screenshot_2024-07-09_at_11.36.36_PM.png]]

  

The non-vectorized output takes almost 500 times more seconds than the vectorized output.

  

  

## More vectorization examples

**Whenever possible avoid explicit for loops**

  

We have many vectorization implementing functions in numpy like:

- np.exp()
- np.log()
- np.abs()
- np.maximum(v,0)
- v**2 —> v squared element-wise
- 1/v —> element-wise v inverse
  

# Vectorizing Logistic Regression

  

## Forward pass

  

- z[i] = w^T[i]* x[i] + b
- a[i] = sigmoid(z[i])

  

Doing this using vectorization:

X is a matrix of (nx x m) dimensional matrix

Z is a 1xm row matrix which helps you compute z1,z2,…zm

  

Z = wT X + b

wT —> row vector

  

z = [wTx1 + b wTx2+ b wTx3 + b…….]

  

and these elements are z1 z2 and so on….

Z = np.dot(wT,X) + b

  

Now Lets compute a1 a2 a3:

A —> [a1 a2 a3 a4….]

  

sigmoid is 1/1+e^-x

- multiply a element wise with -1
- apply the exp comand to it
- add 1 to A element wise
- inverse element wise

```Plain
u = -1*v
u = np.exp(u)
u = u +1
u = 1/u
```

This gives u as sigmoid matrix of v

which is

```Plain
u = 1/(1 + np.exp(-1*v))
```

  

# Vectorizing Logistic Regression Gradient Output

  

dz1 = a1 - y1 dz2 = a2 - y2……

  

dz —> m dimensional row vector which stores dz1 dz2 and so on..

dz = A - Y

  

![[Screenshot_2024-07-10_at_2.11.01_AM.png]]

  

We can vectorize the first part of the image by using A, dz and Y vectors

  

The second part can be vectorized as:

- db = np.sum(dz) / m
- dw = np.sum(np.dot(dz,X)) / m

  

w = w - alpha(dw)

b = b - alpha(db)

  

# Broadcasting in Python

  

![[Screenshot_2024-07-10_at_2.34.01_AM.png]]

The act of copying the elements of the current matrix to that of the operand matrix is called broadcasting

  

(m,n) +-* / (1,n) then this 1 row is copied m times and then the operation is performed