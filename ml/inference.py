import numpy as np
import tensorflow as tf
from sklearn.preprocessing import LabelEncoder
from collections import deque
import numpy as np
from collections import deque

# def load_and_preprocess_data(file_path, lines_per_example=300, delimiter=',', dtype=float):
#     """Loads and preprocesses data from a single text file for inference.

#     Args:
#         file_path: Path to the text file.
#         lines_per_example: The number of lines per example.
#         delimiter: The delimiter used in the data file.
#         dtype: The data type of the values.

#     Returns:
#         A NumPy array of processed data suitable for CNN input.
#     """

#     data = []
#     queue = deque(maxlen=lines_per_example)

#     with open(file_path, 'r') as file:
#         lines = file.readlines()

#     i = 0
#     while i < len(lines) - lines_per_example + 1:
#         queue.clear()  # Clear the queue for the next window

#         # Fill the queue with the next `lines_per_example` values
#         for j in range(lines_per_example):
#             line = lines[i + j].strip()
#             if line:
#                 values = [dtype(val) for val in line.split(delimiter)]
#                 queue.append(values)

#         # If the queue is fully populated, add it to the data list
#         if len(queue) == lines_per_example:
#             data.append(list(queue))

#         # Move the window one value forward
#         i += 1

#     if not data:
#         raise ValueError("No data loaded")

#     # Convert list to NumPy array and add channel dimension
#     data = np.array(data)
#     data = np.expand_dims(data, axis=-1)  # Add channel dimension for CNN input

#     return data

# def load_model(model_path):
#     return tf.keras.models.load_model(model_path)

# def predict(model, data, label_encoder):
#     predictions = model.predict(data)
#     predicted_labels = np.argmax(predictions, axis=1)
#     print(f'Predicted classes: {predicted_labels}')
#     return label_encoder.inverse_transform(predicted_labels)

# if __name__ == "__main__":
#     # Load the trained model
#     model_path = 'emg_moment_prediction_model_cnnlstm_further_trained.h5' 
#     model = load_model(model_path)

#     # Load and preprocess new data
#     new_data_path = 'data/final_data/thumb_final.txt'  # Replace with the path to your new data
#     new_data = load_and_preprocess_data(new_data_path)

#     # Initialize LabelEncoder with the same labels used during training
#     label_encoder = LabelEncoder()
#     label_encoder.fit(['claw', 'index', 'middle','thumb'])

#     # Make predictions
#     predictions = predict(model, new_data, label_encoder)

#     # Print results
#     for i, prediction in enumerate(predictions):
#         print(f"Example {i+1}: Predicted moment - {prediction}")



import numpy as np
import tensorflow as tf
from sklearn.preprocessing import LabelEncoder
from collections import deque

def load_and_preprocess_data(file_path, lines_per_example=300, delimiter=',', dtype=np.float32):
    """Loads and preprocesses data from a single text file for inference."""
    data = []
    queue = deque(maxlen=lines_per_example)

    with open(file_path, 'r') as file:
        lines = file.readlines()

    i = 0
    while i < len(lines) - lines_per_example + 1:
        queue.clear()  # Clear the queue for the next window

        for j in range(lines_per_example):
            line = lines[i + j].strip()
            if line:
                values = [dtype(val) for val in line.split(delimiter)]
                queue.append(values)

        if len(queue) == lines_per_example:
            data.append(list(queue))

        i += 1

    if not data:
        raise ValueError("No data loaded")

    data = np.array(data, dtype=np.float32)  # Ensure data is of type FLOAT32

    # Check the shape and adjust if necessary
    print("Data shape before adding channel dimension:", data.shape)
    
    if len(data.shape) == 3:
        data = np.expand_dims(data, axis=-1)
    
    print("Data shape after adding channel dimension:", data.shape)

    return data

def load_tflite_model(model_path):
    """Load a TensorFlow Lite model from a file."""
    interpreter = tf.lite.Interpreter(model_path=model_path)
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()
    print("Input details:", input_details)
    return interpreter
def predict_with_tflite(interpreter, input_data):
    """Run inference on input data using the TensorFlow Lite interpreter."""
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()

    predictions = []

    # Loop through each sample and perform inference
    for sample in input_data:
        # Ensure each sample has the shape [1, 300, 4]
        sample = np.squeeze(sample, axis=-1)  # Remove the last dimension
        sample = np.expand_dims(sample, axis=0)  # Add batch dimension
        
        input_data = np.array(sample, dtype=np.float32)
        expected_shape = input_details[0]['shape']
        print("Expected input shape:", expected_shape)
        print("Input data shape:", input_data.shape)

        if input_data.shape != tuple(expected_shape):
            raise ValueError(f"Shape mismatch: Expected {expected_shape}, but got {input_data.shape}")

        interpreter.set_tensor(input_details[0]['index'], input_data)
        interpreter.invoke()

        output_data = interpreter.get_tensor(output_details[0]['index'])
        predictions.append(output_data)

    return np.array(predictions)

if __name__ == "__main__":
    tflite_model_path = 'emg_moment_prediction_model_cnnlstm.tflite'
    interpreter = load_tflite_model(tflite_model_path)

    new_data_path = 'data/final_data/index_final1.txt'
    new_data = load_and_preprocess_data(new_data_path)

    predictions = predict_with_tflite(interpreter, new_data)

    # Assuming you have labels in the same order as used for encoding
    label_encoder = LabelEncoder()
    label_encoder.fit(['claw', 'index', 'middle', 'thumb'])
    
    # Flatten predictions if needed and get the labels
    flattened_predictions = np.vstack(predictions)
    predicted_labels = np.argmax(flattened_predictions, axis=1)
    decoded_predictions = label_encoder.inverse_transform(predicted_labels)

    for i, prediction in enumerate(decoded_predictions):
        print(f"Example {i+1}: Predicted moment - {prediction}")
