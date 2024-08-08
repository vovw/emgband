import websocket
import socket
import time
import math
import logging
import scipy.signal as signal
from digitalfilter import LiveLFilter
import pandas as pd

num_sensors = 4

def send_data(ws, live_bsfilter, live_bpfilter, blockSize, start_time, socketio, allData):
    data_size, sample_size, packet_size = 0, 0, 0
    previous_sample_number, previous_data = -1, []
    while True:
        data = ws.recv()
        data_size += len(data)
        current_time = time.time()
        elapsed_time = current_time - start_time

        if elapsed_time >= 1.0:
            refresh_rate = calculate_rate(packet_size, elapsed_time)
            samples_per_second = calculate_rate(sample_size, elapsed_time)
            bytes_per_second = calculate_rate(data_size, elapsed_time)
            logging.info(
                f"{math.ceil(refresh_rate)} FPS : {math.ceil(samples_per_second)} SPS : {math.ceil(bytes_per_second)} BPS")

            packet_size, sample_size, data_size = 0, 0, 0
            start_time = current_time

        if data:
            packet_size += 1
            logging.debug("Counters of 1st 3 samples in packet : %d %d %d", data[0], data[13], data[26])
            channel_data_buffer = [[] for _ in range(6)]
            for block_location in range(0, len(data), blockSize):
                sample_size += 1
                block = data[block_location:block_location + blockSize]
                sample_number = block[0]
                channel_data = []

                for channel in range(6):
                    channel_offset = 1 + (channel * 2)
                    sample = int.from_bytes(block[channel_offset:channel_offset + 2], byteorder='big', signed=True)
                    sample = live_bsfilter[channel](sample)
                    sample = live_bpfilter[channel](sample)
                    channel_data.append(sample)
                    channel_data_buffer[channel].append(sample)
                    allData[channel].append(sample)
                # print(channel_data)

                if previous_sample_number == -1:
                    previous_sample_number, previous_data = sample_number, channel_data
                else:
                    if sample_number - previous_sample_number > 1:
                        logging.error("Error: Sample Lost")
                        return
                    elif sample_number == previous_sample_number:
                        logging.error("Error: Duplicate sample")
                        return
                    elif sample_number - previous_sample_number < 1 and not (
                            sample_number == 0 and previous_sample_number == 255):
                        logging.error("Error: Sample order missed")
                        return
                    else:
                        previous_sample_number, previous_data = sample_number, channel_data
            data = {
                'channel1': channel_data_buffer[0],
                'channel2': channel_data_buffer[1],
                'channel3': channel_data_buffer[2],
                'channel4': channel_data_buffer[3],
                # 'channel5': channel_data_buffer[4],
                # 'channel6': channel_data_buffer[5]
            }
            socketio.emit('data', data)


def calculate_rate(data_size, elapsed_time):
    return data_size / elapsed_time


def getchanneldata(socketio, allData):
    logging.basicConfig(level=logging.INFO)
    sampling_rate = 1000
    blockSize = 13

    order = 1
    bs_coeffs = signal.iirfilter(order, Wn=[45, 55], btype='bandstop', ftype='butter', fs=sampling_rate)
    live_bsfilter = [LiveLFilter(*bs_coeffs) for _ in range(num_sensors)]

    order = 2
    bp_coeffs = signal.iirfilter(order, Wn=[1, 400], btype='bandpass', ftype='butter', fs=sampling_rate, output="ba")
    live_bpfilter = [LiveLFilter(*bp_coeffs) for _ in range(num_sensors)]

    ws = websocket.WebSocket()
    logging.info("Trying to connect")
    ws.connect("ws://" + socket.gethostbyname("multi-emg.local") + ":81")

    logging.info("Sampling rate is set to %d", sampling_rate)
    start_time = time.time()
    send_data(ws, live_bsfilter, live_bpfilter, blockSize, start_time, socketio, allData)
