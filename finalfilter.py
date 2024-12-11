import numpy as np
import matplotlib.pyplot as plt
import serial
from scipy.signal import butter, filtfilt
from scipy.fftpack import fft
import time

# Parameters
port = 'COM6'  
baudrate = 115200
sampling_rate = 800  # Hz
cutoff_frequency = 10  # Hz
duration = 10  # seconds

# Function to apply a low-pass filter
def low_pass_filter(data, cutoff, fs, order=5):
    nyquist = 0.5 * fs
    normal_cutoff = cutoff / nyquist
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

# Function to compute the FFT and frequency axis
def compute_fft(data, fs):
    N = len(data)
    fft_values = fft(data)
    fft_magnitude = 2.0 / N * np.abs(fft_values[:N // 2])
    freqs = np.fft.fftfreq(N, 1 / fs)[:N // 2]
    return freqs, fft_magnitude

# Initialize serial connection
ser = serial.Serial(port, baudrate)
print("Connected to Arduino on port", port)

# Collect data
xdata, ydata = [], []
start_time = time.time()

print("Collecting data...")

while time.time() - start_time < duration:
    if ser.in_waiting > 0:
        line_data = ser.readline().decode('utf-8', errors='ignore').strip()
        try:
            sig = float(line_data)
            xdata.append(len(xdata))
            ydata.append(sig)
        except ValueError:
            print(f"Skipping invalid data: {line_data}")

# Apply filter
print("Applying filter...")
filtered_data = low_pass_filter(ydata, cutoff_frequency, sampling_rate)

# Compute FFT for original and filtered data
print("Computing FFT...")
freqs_original, fft_original = compute_fft(ydata, sampling_rate)
freqs_filtered, fft_filtered = compute_fft(filtered_data, sampling_rate)

# Plot the data in time domain
plt.figure(figsize=(12, 8))

plt.subplot(3, 1, 1)
plt.plot(xdata, ydata, label='Original Data', marker='')
plt.xlabel('Sample')
plt.ylabel('Voltage')
plt.title('Original Data')
plt.grid(True)
plt.legend()

plt.subplot(3, 1, 2)
plt.plot(xdata, filtered_data, label='Filtered Data', color='orange', marker='')
plt.xlabel('Sample')
plt.ylabel('Voltage')
plt.title('Filtered Data')
plt.grid(True)
plt.legend()

# Plot the data in frequency domain
plt.subplot(3, 1, 3)
plt.plot(freqs_original, fft_original, label='Original Data (FFT)', color='blue')
plt.plot(freqs_filtered, fft_filtered, label='Filtered Data (FFT)', color='orange')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title('Frequency Spectrum')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

ser.close()