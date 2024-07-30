import numpy as np
import pandas as pd

# Define the input file path
INPUT_FILE = 'emg_magnitudes.csv'

def load_emg_data(file_path):
    # Read the CSV file into a pandas DataFrame, including blank lines
    df = pd.read_csv(file_path, header=None, skip_blank_lines=False)

    # Initialize lists to hold training examples
    data = []
    current_example = []

    # Process each row in the DataFrame
    for index, row in df.iterrows():
        if row.isnull().all():
            # If the row is empty (all values are NaN), it's the end of a training example
            if len(current_example) == 128:
                data.append(current_example)
            # Reset current example for the next batch
            current_example = []
        else:
            # Append the row to the current example
            current_example.append(row.values.tolist())

    # Add the last example if it's complete
    if len(current_example) == 128:
        data.append(current_example)

    # Convert the list of training examples to a numpy array
    tensor = np.array(data)
    return tensor

def main():
    # Load the EMG data from the CSV file
    emg_tensor = load_emg_data(INPUT_FILE)
    
    # Print the shape of the tensor to verify
    print("Tensor shape:", emg_tensor.shape)

if __name__ == "__main__":
    main()