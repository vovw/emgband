import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

# Load data from text files
def load_data(file_paths):
    data = []
    labels = []
    for label, file_path in file_paths.items():
        with open(file_path, 'r') as file:
            lines = file.readlines()
            # Find the index of "START"
            start_index = lines.index("START\n") + 1
            # Assume the data ends at the end of the file if "DONE" is not found
            done_index = None
            for i, line in enumerate(lines):
                if "DONE\n" in line:
                    done_index = i
                    break
            if done_index is None:
                done_index = len(lines)
            for line in lines[start_index:done_index]:
                try:
                    readings = list(map(float, line.strip().split(',')))
                    data.append(readings)
                    labels.append(label)
                except ValueError:
                    # Skip lines that cannot be converted to float readings
                    print(f"Skipping line: {line}")
                    continue
    if not data:
        raise ValueError("No data loaded. Please check your data files.")
    return np.array(data), np.array(labels)

file_paths = {
    'claw': 'data/claw.txt',
    'index': 'data/index.txt',
    'idle': 'idle.txt'
}

data, labels = load_data(file_paths)

# Encode labels
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

# Split data into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, random_state=42)

# Build the model
model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=(128,)),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(3, activation='softmax')  # 3 classes: claw, index, idle
])

# Compile the model
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

# Train the model
model.fit(X_train, y_train, epochs=50, batch_size=32, validation_split=0.2)

# Evaluate the model
test_loss, test_acc = model.evaluate(X_test, y_test)
print(f'Test accuracy: {test_acc}')

# Save the model
model.save('emg_moment_prediction_model.h5')

