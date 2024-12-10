import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import serial
import io

def convert_to_rgb565(image):
    # Convert image to RGB565 format for TFT display
    rgb565_data = []
    image_rgb = image.convert('RGB')
    for pixel in image_rgb.getdata():
        r, g, b = pixel
        # Convert RGB to RGB565
        rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)
        rgb565_data.append(rgb565)
    return rgb565_data

def prepare_plot_for_tft():
    # Create plot
    plt.figure(figsize=(2.4, 2.0), dpi=80)  # Adjust size for typical TFT display
    x = np.linspace(0, 10, 100)
    y = np.sin(x)
    plt.plot(x, y)
    plt.title('Sine Wave')
    plt.xlabel('X axis')
    plt.ylabel('Y axis')

    # Save plot to buffer
    buf = io.BytesIO()
    plt.savefig(buf, format='png', bbox_inches='tight', pad_inches=0)
    buf.seek(0)

    # Open image and convert
    image = Image.open(buf)
    
    # Convert to RGB565
    rgb565_data = convert_to_rgb565(image)

    # Close plot to free memory
    plt.close()

    return rgb565_data

def send_to_arduino():
    # Open serial connection
    ser = serial.Serial('COM5', 115200)  # Higher baud rate for faster transfer
    
    # Get plot data
    rgb565_data = prepare_plot_for_tft()
    
    # Send data to Arduino
    for pixel in rgb565_data:
        ser.write(f"{pixel}\n".encode())
    
    ser.close()

# Run the transfer
send_to_arduino()