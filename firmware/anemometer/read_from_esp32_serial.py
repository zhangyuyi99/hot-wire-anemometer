import serial
import time
import os
from datetime import datetime
from time import time

ser = serial.Serial('COM3', 115200)  # replace '/dev/ttyUSB0' with your port
ser.flushInput()  # Flush the serial buffer

# Get the start time
start_time = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

# Open the output file
output_file = open(f"{start_time}_temp.txt", "a")

buffer = []  # Create a buffer to hold the console output

try:
    while True:
        try:
            line = ser.readline().decode('utf-8')  # read a '\n' terminated line
        except UnicodeDecodeError:
            print("Received invalid character. Continuing...")
            continue
        current_time = time()
        output_file.write(f"{current_time:.3f}")
        output_file.write(" ")
        output_file.write(line)
        buffer.append((current_time, line))  # Add the data to the buffer instead of printing it
except KeyboardInterrupt:
    print("Exiting program")
finally:
    # Print out the entire buffer
    for timestamp, line in buffer:
        print(f"{timestamp}, {line}", end="")

    # Get the end time
    end_time = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")

    # Close the output file
    output_file.close()

    # Rename the file to include the end timestamp
    os.rename(f"{start_time}_temp.txt", f"{start_time}_{end_time}.txt")
    
    ser.close()
