# EMG Armband

<p align="center">
  <img src="https://github.com/user-attachments/assets/7166b846-3b5f-4400-821d-bd9e433ab1e2" alt="EMG Armband logo" width="300">
</p>

<p align="center">
  <strong>Developing an EMG system that can understand human hand motions.</strong>
</p>

## What?

Our project focuses on reading Electromyography (EMG) data using sensors. We follow these steps:
1. Amplify the EMG signals
2. Filter the data
3. Perform ADC conversion
4. Conduct frequency analysis
5. Feed the analyzed data into a ML model to recognize and classify hand motions

## Project Structure

Our codebase is organized as follows:
- `/ml`: Contains the machine learning model code
- `/muskel`: Houses the ESP32 package code for reading data, performing windowing, and conducting frequency analysis
- `/EMG-Amplifier`: Contains files for our custom EMG amplifier, built from scratch

## Setup Instructions

### Setting up the muskel
```bash
cd muskel
idf.py build
idf.py flash monitor >> dataset_file.csv
```

### Machine Learning
```bash
cd ml
python3 model_cnn_lstm.py  # Starts the training run
python3 inference.py       # Starts the inference
```

## Additional Resources

- [Project Report](https://docs.google.com/document/d/1yk_-5TVg101YQZIr_mQyxtKx-pYntiC_fhi9UAto6ts/edit#heading=h.k0lkhc9o50yk)
- [Blog Post: How Does EMG Armband Work?](https://blog.sravjti.in/2024/07/26/how-does-emg-armband-work.html)

<p align="center">
  <img src="https://blog.sravjti.in/assets/posts/emg-armband/upside_down.svg" alt="EMG Armband Diagram "width="250">
</p>

<p align="center">
  <em>Thanks to <a href="https://upsidedownlabs.tech">upsidedownlabs</a> for sponsoring and supporting our research!</em>
</p>

---

<p align="center">
  Made with ❤️ by the members of SRA
</p>
