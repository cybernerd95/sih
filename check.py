import pyautogui

import random
import time

def random_key_input(min_value=0, max_value=50, interval=0.0001):
    """
    Simulates random number inputs on the keyboard at regular intervals.

    :param min_value: Minimum value for the random number.
    :param max_value: Maximum value for the random number.
    :param interval: Time interval (in seconds) between each input.
    """
    try:
        while True:
            # Generate a random number
            random_number = random.randint(min_value, max_value)
            
            # Type the random number
            pyautogui.typewrite(str(random_number))
            
            # Press 'Enter' to simulate a keyboard press
            pyautogui.press('enter')
            
            # Wait for the next input
            time.sleep(interval)
    except KeyboardInterrupt:
        print("\nProgram terminated by user.")

# Run the script
time.sleep(3)
if __name__ == "__main__":
    print("Simulating random keyboard inputs. Press Ctrl+C to stop.")
    random_key_input(min_value=0, max_value=50, interval=0.01)
