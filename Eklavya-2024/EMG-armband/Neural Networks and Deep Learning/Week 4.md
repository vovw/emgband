# Deep L-layer Neural Network

**What is a Deep Neural Network**
We say that logistic regression is a shallow model.So shallow and deep is the referring to the number of layer

## Notation

L = number of layers
N[l] = number of units in layer l
so n[0] is n_x
a[l] = activation in layer l
W[l] and b[l] are the parameters to determine z[l]
![[Pasted image 20240715020028.png]]

# Forward Propagation in Deep Neural Network
![[Pasted image 20240715021442.png]]

# Getting your Matrix Dimensions Right

Let a Neural Network be:
L = 5
z[1] = w[1] X + b[1]

![[Pasted image 20240715021601.png]]
n[1] = 3
n[2] = 5
n[3] = 4
n[5] = 1
n[0] = n_x = 2

z is the vector of activation for the first layer

z --> (3,1) or (n[1],1) dimensional matrix
x --> (n_x,1) (2,1)

w --> matrix we multiply in x to give us z. A (2,1) matrix times what gives a (3,1 matrix). A (3,2) matrix. w is (n[1],n[0]) dimensioned

w[1] --> (n[1],n[0]) so it is the dimension of the current layer, dim of previous layer

b vector will have the same dimension as the z vector of that layer
![[Pasted image 20240715024530.png|500]]

a[l] = g[l] (z[l])
For a vectorised implementation:
- W 
- b
- dW
- db
Will have same dimensions
Z,X A will change

Z[1] = W[1] * X  + b[1]
Z[1] --> Columns are individual training examples(n[1], m)
W --> (n[1] , n[0])
X --> (n[0], m )

so (n[1], n[0]) * (n[0], m) = (n[1], m) matrix
b ->(n[1], 1) but because of vectorisation it is taken care of

Z[l] , A[l] = (n[l], m)


# Why Deep Representation


![[Pasted image 20240718024041.png]]
This sort of hierarchical breakdown of data is what deep neural networks allow us to do

## Circuit Theory and Deep Learning

**There are function you can compute with a small L- layer deep neural network that a shallower network would require exponentially more hidden units**

Let's say you are trying to compute the parity of all your input features using X-OR Gates

![[Pasted image 20240718024649.png]]
Here the number of gates are not that large.
The depth is of the order O(log n)

Now if you are restricted to use these multiple layers. You are forced to compute the function using one hidden layer. Now the hidden layer will be exponentially large and will use more X-OR Gates

# Forward and Backward Propagation

![[Pasted image 20240718033536.png]]
The top function is forward prop
The lower function is backward prop
![[Pasted image 20240718034015.png]]

![[Pasted image 20240718034844.png]]


# Parameters vs Hyperparameters

## Hyperparameters
- learning rate
- number of iterations
- number of hidden layers
- number of hidden units
- choice of activation function
These are parameters which control our primary parameters which are W and b

Later: momentum, mini-batch size, regularisation 