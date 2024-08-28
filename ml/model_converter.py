import numpy as np
import tensorflow as tf
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder

def convert_model_to_tflite(model_path, output_path):
    # Load the Keras model
    model = tf.keras.models.load_model(model_path)
    
    # Create the TFLite Converter
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    
    # Enable the Flex Delegate
    # converter.target_spec.supported_ops = [tf.lite.OpsSet.SELECT_TF_OPS]
    
    # Apply post-training quantization
    # converter.optimizations = [tf.lite.Optimize.DEFAULT]
    
    # Convert the model
    tflite_model = converter.convert()
    
    # Save the TFLite model
    with open(output_path, 'wb') as f:
        f.write(tflite_model)

convert_model_to_tflite('emg_moment_prediction_model_cnnlstm.h5', 'emg_moment_prediction_model_cnnlstm.tflite')
