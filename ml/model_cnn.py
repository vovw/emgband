import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder


def load_data(file_paths, file_labels, lines_per_example=300, delimiter=',', dtype=float):
    """Loads data from text files into a NumPy array.

    Args:
        file_paths: A list of file paths.
        file_labels: A list of corresponding labels.
        lines_per_example: The number of lines per example.
        delimiter: The delimiter used in the data files.
        dtype: The data type of the values.

    Returns:
        A tuple of NumPy arrays (data, labels).
    """

    data = []
    labels = []

    for file_path, label in zip(file_paths, file_labels):
        try:
            with open(file_path, 'r') as file:
                example = []
                for i, line in enumerate(file):
                    # Skip blank lines
                    if not line.strip():
                        continue
                    
                    # Process the line if it's not blank
                    values = [dtype(val) for val in line.strip().split(delimiter)]
                    example.append(values)

                    # When 300 lines are collected, store the example and reset
                    if len(example) == lines_per_example:
                        data.append(example)
                        labels.append(label)
                        example = []
                        
                # If there is any leftover data that didn't fill a full example, you can handle it here.
                # (Omitting this part if your data is always a perfect multiple of 300 lines)
                
        except FileNotFoundError:
            print(f"File not found: {file_path}")
        except ValueError as e:
            print(f"Error parsing file: {file_path}, {e}")

    if not data:
        raise ValueError("No valid data loaded")

    # Convert lists to NumPy arrays
    data = np.array(data)
    labels = np.array(labels)

    return data, labels

file_paths = [
    # 'data/claw_final1.txt',
    'data/index_final1.txt',
    'data/middle_finger_final1.txt'
]

file_labels = [
    # 'claw',
    'index',
    'middle'
]



data, labels = load_data(file_paths, file_labels)

# Encode labels
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

# Train-test split
X_train, X_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, random_state=42)

# Reshape data for Conv1D (needs a 3D input: (batch_size, steps, features))
X_train = np.expand_dims(X_train, axis=-1)
X_test = np.expand_dims(X_test, axis=-1)

# Define the model with Conv1D layers and Dropout
model = tf.keras.Sequential([
    # Input layer
    tf.keras.layers.Input(shape=(300, 4)),
    
    # First Convolutional Block
    tf.keras.layers.Conv1D(filters=256, kernel_size=15, activation='relu', padding='same'),
    tf.keras.layers.BatchNormalization(),
    tf.keras.layers.MaxPooling1D(pool_size=2),
    tf.keras.layers.Dropout(0.3),
    
    # Second Convolutional Block
    tf.keras.layers.Conv1D(filters=128, kernel_size=10, activation='relu', padding='same'),
    tf.keras.layers.BatchNormalization(),
    tf.keras.layers.MaxPooling1D(pool_size=2),
    tf.keras.layers.Dropout(0.3),
    
    # Third Convolutional Block
    tf.keras.layers.Conv1D(filters=64, kernel_size=5, activation='relu', padding='same'),
    tf.keras.layers.BatchNormalization(),
    tf.keras.layers.MaxPooling1D(pool_size=2),
    tf.keras.layers.Dropout(0.3),
    
    # Fourth Convolutional Block (for deeper feature extraction)
    tf.keras.layers.Conv1D(filters=32, kernel_size=3, activation='relu', padding='same'),
    tf.keras.layers.BatchNormalization(),
    tf.keras.layers.MaxPooling1D(pool_size=2),
    tf.keras.layers.Dropout(0.3),
    
    # Flatten and Dense Layers
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.BatchNormalization(),
    tf.keras.layers.Dropout(0.4),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dropout(0.4),
    
    # Output Layer
    tf.keras.layers.Dense(len(file_labels), activation='softmax')
])


# Model compilation
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

# Train, evaluate, and save the model
model.fit(X_train, y_train, epochs=100, batch_size=12, validation_split=0.2)

test_loss, test_acc = model.evaluate(X_test, y_test)
print(f'Test accuracy: {test_acc}')

model.save('emg_moment_prediction_model.h5')
