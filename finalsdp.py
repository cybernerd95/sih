import numpy as np
import matplotlib.pyplot as plt
import serial
from scipy.signal import butter, filtfilt
import time


port = 'COM4'  # Update to the correct port where Arduino is connected
baudrate = 115200
sampling_rate = 1000  
cutoff_frequency = 150 
duration = 10  


def low_pass_filter(data, cutoff, fs, order=5):
    nyquist = 0.5 * fs
    normal_cutoff = cutoff / nyquist
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

ser = serial.Serial(port, baudrate)
print("Connected to Arduino on port", port)

xdata, ydata = [], []
start_time = time.time()

print("Collecting data...")

while time.time() - start_time < duration:
    if ser.in_waiting > 0:
        line_data = ser.readline().decode('utf-8').strip()
        try:
            sig = float(line_data)
            xdata.append(len(xdata))
            ydata.append(sig)
        except ValueError:
            print(f"Skipping invalid data: {line_data}")

print("Applying filter...")
filtered_data = low_pass_filter(ydata, cutoff_frequency, sampling_rate)

# Send filtered data to Arduino
for value in filtered_data:
    ser.write(f"{value}\n".encode('utf-8'))
    time.sleep(0.1)  # To avoid overloading the serial communication

print("Data sent to Arduino.")

ser.close()
