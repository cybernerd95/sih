import numpy as np
import matplotlib.pyplot as plt
import serial
from scipy.fftpack import fft
import time

# Parameters
port = 'COM6'  
baudrate = 115200
sampling_rate = 1000  # Hz
duration = 10  # seconds

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

# Compute FFT for original data
print("Computing FFT...")
freqs_original, fft_original = compute_fft(ydata, sampling_rate)

# Plot the data in time domain
plt.figure(figsize=(12, 8))

plt.subplot(2, 1, 1)
plt.plot(xdata, ydata, label='Original Data', marker='o')
plt.xlabel('Sample')
plt.ylabel('Voltage')
plt.title('Original Data')
plt.grid(True)
plt.legend()

# Plot the data in frequency domain (FFT)
plt.subplot(2, 1, 2)
plt.plot(freqs_original, fft_original, label='Original Data (FFT)', color='blue')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Amplitude')
plt.title('Frequency Spectrum')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()

ser.close()
