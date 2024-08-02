import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout
from tensorflow.keras.utils import to_categorical
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
    '../index_voltages.txt',
    '../idle.txt'
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
labels = to_categorical(labels)  # One-hot encode labels

# Split data into training and testing sets
X_train, X_test, Y_train, Y_test = train_test_split(data, labels, test_size=0.2, random_state=42)
print("X_train shape:", X_train.shape)
print("X_test shape:", X_test.shape)
print("Y_train shape:", Y_train.shape)
print("Y_test shape:", Y_test.shape)

# Build the model
model = Sequential([
    Dense(128, activation='relu', input_shape=(X_train.shape[1],)),
    Dropout(0.3),
    Dense(64, activation='relu'),
    Dropout(0.3),
    Dense(32, activation='relu'),
    Dropout(0.3),
    Dense(Y_train.shape[1], activation='softmax')  # Number of classes
])

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.summary()

# Train the model
history = model.fit(X_train, Y_train, epochs=500, batch_size=32, validation_split=0.2)

# Evaluate the model
test_loss, test_acc = model.evaluate(X_test, Y_test)
print(f'Test accuracy: {test_acc}')

# Save the model
model.save('emg_moment_prediction_model.h5')
