import random
import pyautogui
import time
time.sleep(3) 
try:
    print("Press Ctrl+C to stop the program.")
    
    while True:  # Infinite loop
        # Generate a random number between 1 and 10
        random_number = random.randint(1, 3)

        # Print the number to the console
        print(f"Generated number: {random_number}")

        # Simulate pressing the corresponding number key on the keyboard
        pyautogui.write(str(random_number))

        # Press Enter after typing the number
        pyautogui.press('enter')

        # Wait before the next iteration (adjust delay as needed)
        time.sleep(2)  # 3 seconds delay

except KeyboardInterrupt:
    print("\nProgram stopped by user.")3

