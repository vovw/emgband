  

# Welcome

## What you will learn:

- Neural networks and Deep Learning
- Improving Deep Neural Networks: Hyperparameter tuning, regularazation and optimization
- Structuring your machine learning projects
- Convolutional Neural Networks
- Sequence Models
    - RNN
    - LSTM
    - Transformers

  

  

# What is a Neural Network

  

Let’s say you have the data of 6 houses size of the house with its price. You have to write a function to predict the price of another house based on this data.

Linear regression will tell you to fit a straight line to this data. You know that price of a house will never be negative so

![[Screenshot_2024-07-05_at_5.37.38_PM.png]]

this is how the function graph would look.

This function that we created is a very simple neural network.

```Mermaid
flowchart LR

	A[Size]
	B((Neuron))
	C[Price]
	A --> B
	B --> C
```

The function we made looks like a relu function.

If this is a single neuron network, you get a bigger neural network by stacking such neurons in a layer.

  

If instead of only the size of the house affecting the price of the hosue, now even the number of bedrooms and the postal code and other parameters are afftecting the price then we will need bigger neural networks.

![[Screenshot_2024-07-05_at_5.45.46_PM.png]]

The magic of a neural network is that you only have to give it X and respective Y for all the data that you have, and it does everything else for you.

  

# Supervised Learning With Neural Networks

  

Most of the economical value created by neural networks is through supervised learning.

In supervised learning there is a input X and an output Y.

The most lucrative application of deep learning is online advertising.

X = Add / user info Y = Clicked or not

  

Computer vision has also made a lot of strides mainly due to deep learning.

Classifying an image as different objects.

Translation is also supervised learning

X = Image/ Radar Y= Position of other cars i.e. autonomous vehicles is also supervised learning.

  

- The housing application was standard feed forward neural network.
- Computer vision is mostly convolutional neural networks
- Speech or translation is time based so uses something like rnn or lstm

  

![[Screenshot_2024-07-05_at_8.09.38_PM.png]]

  

Superrvised learning can have two types of data:

- Structured Data- Data from a database or a table
- Unstructured Data- Data like images text or video or audio clips

  

# Why is Deep Learning taking off?

![[Screenshot_2024-07-05_at_8.17.19_PM.png]]

**Scale has been driving deep learning progress**

Scale means the size of neural network and the amount of data.

The amount of data refers to amount of labelled data.

m —> stands for denoting our training set

  

## Scale drives deep learning progress

  

- Data: More data more progress
- Computation: Bigger the neural network better the learning progress
- Algorithmic Change: One of the biggest breakthroughs of neural networks is the change from using sigmoid function to relu. Because for sigmoid when the slope at extremeties becomes zero then the gradient descent process becomes slow. But for relu the slope is not zero for positive values. Thus the gradient descent algorithm has faster computation with ReLU > Sigmoid

![[Screenshot_2024-07-05_at_8.25.46_PM.png]]

If the experiment process happens faster then more time is left for the ideation process