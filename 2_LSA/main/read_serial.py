import numpy as np
import csv

# Define the input and output file paths.
INPUT_FILE = 'sample_emg_data.txt'
OUTPUT_FILE = 'emg_magnitudes.csv'

# Function to read data from the text file.
def read_emg_data(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    data = []
    current_example = []
    
    for line in lines:
        line = line.strip()
        if line:
            # Process line only if it's not empty
            magnitudes = list(map(float, line.split(',')))
            current_example.append(magnitudes)
        else:
            # End of current training example
            if len(current_example) == 128:
                data.append(current_example)
                current_example = []
    
    # Add the last example if there is no trailing empty line
    if len(current_example) == 128:
        data.append(current_example)
    
    return data

# Function to write data to a CSV file.
def write_to_csv(file_path, data):
    with open(file_path, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        for example in data:
            # Convert the example to numpy array and reshape to (128, 3)
            reshaped_example = np.array(example).reshape(128, 3)
            for row in reshaped_example:
                csv_writer.writerow(row)
            csv_writer.writerow([])  # Add a blank line to separate training examples

def main():
    # Read EMG data from the text file.
    emg_data = read_emg_data(INPUT_FILE)
    
    # Write the tensor data to the CSV file.
    write_to_csv(OUTPUT_FILE, emg_data)

if __name__ == "__main__":
    main()
