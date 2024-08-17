import numpy as np
import tensorflow as tf
from sklearn.preprocessing import LabelEncoder
from collections import deque
import numpy as np
from collections import deque

def load_and_preprocess_data(file_path, lines_per_example=300, delimiter=',', dtype=float):
    """Loads and preprocesses data from a single text file for inference.

    Args:
        file_path: Path to the text file.
        lines_per_example: The number of lines per example.
        delimiter: The delimiter used in the data file.
        dtype: The data type of the values.

    Returns:
        A NumPy array of processed data suitable for CNN input.
    """

    data = []
    queue = deque(maxlen=lines_per_example)

    with open(file_path, 'r') as file:
        lines = file.readlines()

    i = 0
    while i < len(lines) - lines_per_example + 1:
        queue.clear()  # Clear the queue for the next window

        # Fill the queue with the next `lines_per_example` values
        for j in range(lines_per_example):
            line = lines[i + j].strip()
            if line:
                values = [dtype(val) for val in line.split(delimiter)]
                queue.append(values)

        # If the queue is fully populated, add it to the data list
        if len(queue) == lines_per_example:
            data.append(list(queue))

        # Move the window one value forward
        i += 1

    if not data:
        raise ValueError("No data loaded")

    # Convert list to NumPy array and add channel dimension
    data = np.array(data)
    data = np.expand_dims(data, axis=-1)  # Add channel dimension for CNN input

    return data

def load_model(model_path):
    return tf.keras.models.load_model(model_path)

def predict(model, data, label_encoder):
    predictions = model.predict(data)
    predicted_labels = np.argmax(predictions, axis=1)
    print(f'Predicted classes: {predicted_labels}')
    return label_encoder.inverse_transform(predicted_labels)

if __name__ == "__main__":
    # Load the trained model
    model_path = 'emg_moment_prediction_model.h5' 
    model = load_model(model_path)

    # Load and preprocess new data
    new_data_path = 'data/test.txt'  # Replace with the path to your new data
    new_data = load_and_preprocess_data(new_data_path)

    # Initialize LabelEncoder with the same labels used during training
    label_encoder = LabelEncoder()
    label_encoder.fit(['claw', 'index', 'middle'])

    # Make predictions
    predictions = predict(model, new_data, label_encoder)

    # Print results
    for i, prediction in enumerate(predictions):
        print(f"Example {i+1}: Predicted moment - {prediction}")
