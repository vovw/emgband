import numpy as np
import csv

# Define the input and output file paths.
INPUT_FILE = 'claw.txt'
OUTPUT_FILE = 'emg_magnitudes.csv'

# Function to read data from the text file.
def read_emg_data(file_path):
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    data = []
    
    for line in lines:
        line = line.strip()
        if line:
            # Process line only if it's not empty
            magnitudes = list(map(float, line.split(',')))
            if len(magnitudes) == 128 * 3:  # Check if there are exactly 128 examples with 3 values each
                reshaped_example = np.array(magnitudes).reshape(128, 3).tolist()
                data.append(reshaped_example)
    
    return data

# Function to write data to a CSV file.
def write_to_csv(file_path, data):
    with open(file_path, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        for example in data:
            for row in example:
                # Append 1 to the end of each row
                csv_writer.writerow(row + [1])
            csv_writer.writerow([])  # Add a blank line to separate training examples

def main():
    # Read EMG data from the text file.
    emg_data = read_emg_data(INPUT_FILE)
    
    # Write the tensor data to the CSV file.
    write_to_csv(OUTPUT_FILE, emg_data)

if __name__ == "__main__":
    main()
