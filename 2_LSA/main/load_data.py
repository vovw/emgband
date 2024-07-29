import numpy as np
import pandas as pd

# Define the input file path
INPUT_FILE = 'emg_magnitudes.csv'

def load_emg_data(file_path):
    df = pd.read_csv(file_path, header=None, skip_blank_lines=False)
    data = []
    current_example = []

    for index, row in df.iterrows():
        if row.isnull().all():

            if len(current_example) == 128:
                data.append(current_example)

            current_example = []
        else:
            current_example.append(row.values.tolist())


    if len(current_example) == 128:
        data.append(current_example)

    tensor = np.array(data)
    return tensor

def main():

    emg_tensor = load_emg_data(INPUT_FILE)
    print("Tensor shape:", emg_tensor.shape)

if __name__ == "__main__":
    main()
