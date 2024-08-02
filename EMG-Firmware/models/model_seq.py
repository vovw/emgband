import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv1D, LSTM, Dense, Dropout, Input
from tensorflow.keras.optimizers import Adam
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
    '../claw_voltages.txt',
    # '../index_voltages.txt',
    '../idle.txt'
]

file_labels = [
    'claw',
    # 'index',
    'idle'
]

data, labels = load_data(file_paths, file_labels)

print("Data Shape:", data.shape)
print("Labels Shape:", labels.shape)
print("Labels:", labels)

# Encode labels
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

# Convert labels to one-hot encoding
num_classes = len(label_encoder.classes_)
labels = tf.keras.utils.to_categorical(labels, num_classes=num_classes)

# Reshape data for LSTM
data = np.expand_dims(data, axis=-1)  # Add a channel dimension

# Split data into training and testing sets
X_train, X_test, Y_train, Y_test = train_test_split(data, labels, test_size=0.2, random_state=42)
print("X_train shape:", X_train.shape)
print("X_test shape:", X_test.shape)
print("Y_train shape:", Y_train.shape)
print("Y_test shape:", Y_test.shape)

# Build the model
model = tf.keras.Sequential([
    Input(shape=(128,1)),  # Input shape for LSTM (time steps, features)
    # Conv1D(32, kernel_size=3, activation='relu'),  # Conv1D layer
    LSTM(64, return_sequences=True),
    Dropout(0.2),
    LSTM(64),
    Dropout(0.2),
    Dense(64, activation='relu'),
    Dense(num_classes, activation='softmax')  # Number of classes
])

# Compile the model with the fixed learning rate optimizer
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.summary()

# Train the model
history = model.fit(X_train, Y_train, epochs=50, batch_size=32, validation_split=0.2)

# Evaluate the model
test_loss, test_acc = model.evaluate(X_test, Y_test)
print(f'Test accuracy: {test_acc}')

# Save the model
model.save('emg_moment_prediction_model.h5')
