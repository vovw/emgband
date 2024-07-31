import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

"""
data loading
- make sure that your files dont have "START" "DONE" tags at start and end
"""

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
        raise ValueError("no data loaded")
    
    data = np.array(data)
    labels = np.array(labels)
    
    return data, labels

file_paths = [
    'data/claw.txt',
    'data/index_finger.txt',
    'data/idel.txt'
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

"""
Encode labels
"""
label_encoder = LabelEncoder()
labels = label_encoder.fit_transform(labels)

"""
train test split
"""
X_train, X_test, y_train, y_test = train_test_split(data, labels, test_size=0.2, random_state=42)


"""
layering the model
"""
model = tf.keras.Sequential([
    tf.keras.layers.InputLayer(input_shape=(128,)),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(64, activation='relu'),
    tf.keras.layers.Dense(32, activation='relu'),
    tf.keras.layers.Dense(3, activation='softmax')  # 3 classes: claw, index, idle
])

"""
model compilation
- adam optmizer https://arxiv.org/pdf/1412.6980
- loss https://www.tensorflow.org/api_docs/python/tf/keras/losses/SparseCategoricalCrossentropy
"""
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])
"""
train
evalulate
save
"""
model.fit(X_train, y_train, epochs=1000, batch_size=32, validation_split=0.2)

test_loss, test_acc = model.evaluate(X_test, y_test)
print(f'Test accuracy: {test_acc}')

model.save('emg_moment_prediction_model.h5')
