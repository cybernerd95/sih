import numpy as np
import matplotlib.pyplot as plt
import serial
from scipy.signal import butter, filtfilt
import time


port = 'COM5'  
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

with open('data_log.txt', 'w') as file:
    file.write('Sample,Original Voltage,Filtered Voltage\n')
    for i in range(len(xdata)):
        file.write(f'{xdata[i]},{ydata[i]},{filtered_data[i] if i < len(filtered_data) else "NaN"}\n')

print("Data saved to 'data_log.txt'")


plt.figure(figsize=(12, 6))

plt.subplot(2, 1, 1)
plt.plot(xdata, ydata, label='Original Data', marker='o')
plt.xlabel('Sample')
plt.ylabel('Voltage')
plt.title('Original Data')
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(xdata, filtered_data, label='Filtered Data', color='orange', marker='o')
plt.xlabel('Sample')
plt.ylabel('Voltage')
plt.title('Filtered Data')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()


ser.close()

