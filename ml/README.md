This folder contains the machine learning components of the EMG Armband project. It includes scripts for training the model and running inferences.

## Setup

Before running the scripts, you need to set up a virtual environment and install the required dependencies.

1. Create a virtual environment:
   ```bash
   python3 -m venv venv
   ```

2. Activate the virtual environment:
   - On Unix or MacOS:
     ```bash
     source venv/bin/activate
     ```
   - On Windows:
     ```bash
     venv\Scripts\activate
     ```

3. Install the required packages:
   ```bash
   pip install -r requirements.txt
   ```

   Note: Make sure you have a `requirements.txt` file in your `ml` folder listing all necessary packages.

## Usage

### Training the Model

To train the model, run:

```bash
python3 model_name.py
```

This script will load the training data, build the model architecture, train the model, and save the trained model.

### Running Inference

To perform inference using the trained model, run:

```bash
python3 inference.py
```

This script will load the trained model and use it to make predictions on new EMG data.

## Files

- `model_name.py`: Script for building and training the machine learning model.
- `infernece.py`: Script for running inference using the trained model.
- `requirements.txt`: List of Python packages required for the project.

## Notes

- Ensure that your training data is properly formatted and located in the correct directory before running `model_train.py`.
- The `inference.py` script assumes that a trained model is available. Make sure to run `model_train.py` first or have a pre-trained model file in the appropriate location.
