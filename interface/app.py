from flask import Flask, render_template, request, url_for
from flask_socketio import SocketIO
from threading import Lock
import time
from cleanespread import getchanneldata
import csv

from cleanespread import num_sensors
import cleanespread

name = 'rough'
# Background Thread
thread = None
thread1 = None
thread_lock = Lock()
thread_lock1 = Lock()
app = Flask(__name__)
app.config['SECRET_KEY'] = 'UDL!'
socketio = SocketIO(app, cors_allowed_origins='*')
allData = [[] for _ in range(num_sensors)]


# Serve root index file
@app.route('/')
def index():
    return render_template('home.html')


@app.route('/next')
def next():
    gifs = [
        {'src': url_for('static', filename='images/gif1.gif'), 'heading': 'Move thumb'},
        {'src': url_for('static', filename='images/gif2.gif'), 'heading': 'Move finger1'},
        {'src': url_for('static', filename='images/gif3.gif'), 'heading': 'Move finger2'},
        {'src': url_for('static', filename='images/gif4.gif'), 'heading': 'Move finger3'},
        {'src': url_for('static', filename='images/gif5.gif'), 'heading': 'Move finger4'}]
    return render_template('index.html', gifs=gifs, name=name)


@socketio.on('user_data')
def handle_user_data(data):
    global name
    name = data['name']
    print('Received user data:', name)


# Decorator for connect
@socketio.on('connect')
def connect():
    global thread
    print('Client connected')
    with thread_lock:
        if thread is None:
            thread = socketio.start_background_task(getchanneldata, socketio, allData)


@socketio.on('stop_recording')
def stop_recording():
    global allData
    allData=list(map(list,zip(*allData)))
    # Specify the CSV file path
    csv_file_path = f'subjects_recordings/{name}-{time.time()}.csv'
    # Write the list of lists to the CSV file
    with open(csv_file_path, 'w', newline='') as file:
        writer = csv.writer(file)
        writer.writerows(allData)
    allData = []


@socketio.on('connect1')
def connect1():
    global thread1
    print('Client connected')
    with thread_lock1:
        if thread1 is None:
            thread1 = socketio.start_background_task(getchanneldata, socketio, name)


# Decorator for disconnect
@socketio.on('disconnect')
def disconnect():
    print('Client disconnected', request.sid)


if __name__ == '__main__':
    socketio.run(app, allow_unsafe_werkzeug=True)
