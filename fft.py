import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import butter, lfilter, freqz


def butter_bandpass(lowcut, highcut, fs, order=5):
    nyquist = 0.5 * fs
    low = lowcut / nyquist
    high = highcut / nyquist
    b, a = butter(order, [low, high], btype='band')
    return b, a

3
def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

# Parameters
fs = 7e9 
T = 1e-7
n_samples = int(T * fs)  
t = np.linspace(0, T, n_samples, endpoint=False)  


f_signal = 3e7
f_noise = 3e9    

# noise
signal = np.sin(2 * np.pi * f_signal * t) 
noise = 0.5 * np.sin(2 * np.pi * f_noise * t)
input_signal = signal + noise

# Filter design
lowcut = 2.35e9  
highcut = 2.65e9 


filtered_signal = butter_bandpass_filter(input_signal, lowcut, highcut, fs, order=6)


plt.figure(figsize=(12, 8))


plt.subplot(2, 1, 1)
plt.plot(t * 1e6, input_signal, label='Input Signal (with Noise)')
plt.title('Input EMI Signal with Noise')
plt.xlabel('Time [µs]')
plt.ylabel('Amplitude')
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(t * 1e6, filtered_signal, label='Filtered Signal', color='orange')
plt.title('Filtered EMI Signal (2.4 GHz Band)')
plt.xlabel('Time [µs]')
plt.ylabel('Amplitude')
plt.grid()

plt.tight_layout()
plt.show()