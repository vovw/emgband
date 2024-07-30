import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Dropout
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
import numpy as np


def load_data(file_paths, file_labels):
    data = []
    labels = []

    for file_path, label in zip(file_paths, file_labels):
        with open(file_path, 'r') as file:
            lines = file.readlines()
            for line in lines:
                # Skip the START keyword
                if "START" in line:
                    continue
                # Split the line into values and convert to float
                values = line.strip().split(',')
                if values:
                    data.append([float(value) for value in values if value])
                    labels.append(label)
    
    if not data:
        raise ValueError("No data loaded. Please check your data files.")
    
    data = np.array(data)
    labels = np.array(labels)
    
    return data, labels

file_paths = [
    'data/claw.txt',
    'data/index.txt',
    'data/idle.txt'
]

file_labels = [
    'claw',
    'index',
    'idle'
]

data, labels = load_data(file_paths, file_labels)

print("Data Shape:", data.shape)
print("Labels Shape:", labels.shape)
print("Labels:", labels)

# Encode labels
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

# Split data into training and testing sets
X_train, X_test, Y_train, Y_test = train_test_split(data, labels, test_size=0.2, random_state=42)

# Build the model
model = Sequential([
    LSTM(64, input_shape=(128, 3), return_sequences=True),
    Dropout(0.2),
    LSTM(64),
    Dropout(0.2),
    Dense(64, activation='relu'),
    Dense(Y_train.shape[1], activation='softmax')  # Number of classes
])

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.summary()

# Train the model
history = model.fit(X_train, Y_train, epochs=50, batch_size=32, validation_split=0.2)

# Evaluate the model
test_loss, test_acc = model.evaluate(X_test, Y_test)
print(f'Test accuracy: {test_acc}')

# Save the model
model.save('emg_moment_prediction_model.h5')
