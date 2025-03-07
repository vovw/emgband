import numpy as np
import tensorflow as tf

"""
hardcoded examples for easy testing, very hacky
TODO: find a better way to test
"""


claw = np.array([0.03,0.03,0.09,0.06,0.32,0.76,0.67,5.99,4.68,35.65,35.37,30.06,7.54,16.76,25.35,10.84,29.85,20.28,11.40,8.91,3.88,3.10,1.41,4.56,2.96,1.26,1.40,0.89,0.42,0.34,0.40,0.30,0.21,0.09,0.03,0.10,0.20,0.19,0.11,0.07,0.06,0.01,0.03,0.05,0.03,0.04,0.01,0.01,0.02,0.01,0.01,0.03,0.02,0.01,0.01,0.01,0.01,0.00,0.00,0.01,0.01,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00])
idle = np.array([0.02,0.03,0.01,0.18,1.35,2.25,4.61,7.80,20.54,40.61,37.54,45.95,54.15,18.44,30.88,15.16,18.73,40.38,53.61,42.02,19.31,3.64,5.48,4.07,3.49,1.56,0.11,0.08,0.86,0.76,0.17,0.20,0.27,0.22,0.07,0.04,0.05,0.07,0.09,0.13,0.11,0.05,0.02,0.01,0.02,0.02,0.02,0.02,0.03,0.02,0.01,0.02,0.02,0.01,0.01,0.01,0.01,0.00,0.00,0.00,0.01,0.00,0.01,0.01,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,])
index = np.array([0.06,0.07,0.15,0.22,1.55,0.84,3.00,24.63,34.59,42.02,86.81,37.72,35.97,29.28,60.30,70.69,66.57,43.73,19.62,11.15,13.65,2.08,2.67,2.87,2.08,1.85,1.40,0.90,0.81,0.42,0.33,0.22,0.13,0.12,0.19,0.06,0.04,0.00,0.02,0.03,0.01,0.04,0.03,0.03,0.03,0.02,0.00,0.01,0.01,0.01,0.01,0.00,0.00,0.01,0.01,0.01,0.01,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,])


example = claw

"""
if model = 1 run feed forward network
model = 2 run LSTM
model = 3 run SVM
"""
if which_model = 1:
    model = tf.keras.models.load_model('emg_moment_prediction_model.h5')
    # Function to preprocess new data
    def preprocess_new_data(new_data):
        # Ensure the new data is in the correct format (num_samples, 128)
        if new_data.ndim == 1:
            new_data = new_data.reshape(1, -1)  # Convert single sample to (1, 128)
        elif new_data.shape[1] != 128:
            raise ValueError("New data must have 128 features per sample.")
        return new_data

    # Function to perform inference
    def predict(new_data):
        preprocessed_data = preprocess_new_data(new_data)
        predictions = model.predict(preprocessed_data)
        return predictions

    # Example usage
    if __name__ == "__main__":
        # Example new data (replace with actual new data)
        # Perform inference
        predictions = predict(example)

        # Print the predictions
        print("Predictions:", predictions)
        print("Predicted class:", np.argmax(predictions, axis=1))
elif which_model = 2:
    model = load_model('gesture_recognition_model.h5')

    # Define the label encoder
    label_encoder = LabelEncoder()
    label_encoder.classes_ = np.array(['claw', 'index', 'idle'])

    def preprocess_input(data):
        # Ensure the data is in the correct shape
        data = np.array(data)
        data = data.reshape((data.shape[0], data.shape[1], 1))
        return data

    def predict(model, data):
        # Preprocess the input data
        data = preprocess_input(data)
        
        # Make predictions
        predictions = model.predict(data)
        
        # Convert predictions to class labels
        predicted_classes = np.argmax(predictions, axis=1)
        predicted_labels = label_encoder.inverse_transform(predicted_classes)
        
        return predicted_labels
        predicted_labels = predict(model, example)
        print("Predicted Labels:", predicted_labels)

