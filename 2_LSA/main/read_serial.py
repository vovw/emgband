import csv

def save_to_csv(data):
    with open("data.csv", "a",newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(data)


def read_txt(filename):
    reading = []
    with open(filename,"r") as file:
        for line in file:
            line = line.strip()
            data = line.split(",") 
            data = [float(num) for num in data ]
            data.append(1.0)
            save_data = 0
            if any(num > 100 for num in data):
                reading.append(data)
    print(reading)
    save_to_csv(reading)


read_txt("readings.txt")

# Script written to test with values simillar to ours
# import random

# def generate_readings(num_lines, num_bins, spike_probability, min_spike_value):
#   """Generates a text file with specified number of lines of random readings with potential spikes.

#   Args:
#     num_lines: The number of lines (examples) to generate.
#     num_bins: The number of frequency bins per line.
#     spike_probability: Probability of a spike occurring in a bin.
#     min_spike_value: Minimum value of a spike.
#   """

#   with open("readings.txt", "w") as f:
#     for _ in range(num_lines):
#       line = []
#       for _ in range(num_bins):
#         value = random.uniform(0, 1)
#         if random.random() < spike_probability:
#           value = random.uniform(0, min_spike_value + 100)  # Adjust range as needed
#         line.append(str(value))
#       f.write(", ".join(line) + "\n")

# # Generate a text file with 10 lines of 128 bins each, with 10% spike probability and minimum spike value 200
# generate_readings(500, 128, 0.1, 200)
