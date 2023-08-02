# Hot wire anemometer

This project includes the design and implementation of a Wi-Fi enabled anemometer. The anemometer consists of a hardware portion which includes the PCB designs for the main board and logic board and a software portion which includes the firmware for operating the anemometer. The anemometer uses a hot wire method for wind speed measurement, and it includes a function to monitor battery voltage, disabling wind readings when the voltage drops below 7V until it recovers.

## Contents
1. [Hardware](#Hardware)
2. [Software](#Software)
3. [Usage](#Usage)
4. [File Structure](#File-Structure)

## Hardware
The hardware part consists of two boards: 

### Main Board
The main board uses the analog hot wire anemometer method for wind measurement. It is powered and sends signal to the logic board.

### Logic Board
The logic board contains an ESP32 processor which receives signals from the main board. It sends the data either via a serial port or Wi-Fi. It also monitors the battery voltage and provides visual feedback using LEDs.

## Software
The software part consists of firmware written for the ESP32 processor. The firmware contains routines to read data from the main board, manage Wi-Fi connectivity, monitor battery voltage, and control LED indicators.

### Python Scripts
There are two Python scripts provided for reading data:

1. `read_from_esp32.py`: Reads data over Wi-Fi. To use this, the anemometer and the computer receiving data must be on the same Wi-Fi network.

2. `read_from_esp32_serial.py`: Reads data over a serial port.

## Usage

To use this anemometer, you will need to:

1. Print the main board and logic board based on the provided PCB design files.
2. Perform necessary wirings to power up the main board and send signals to the logic board.
3. Set up Wi-Fi connectivity in the firmware.
4. Compile and upload the firmware to the logic board.
5. Calibrate the anemometer. To calibrate, place the anemometer in an environment with the same ambient temperature as the actual measurement environment. Adjust the Through Hole Potentiometer on each anemometer until the no-wind reading reaches 1000-1200. Each anemometer is paired with a green LED on the logic board. The LED lights up when the no-wind reading is in the desired range.
6. Run the provided Python scripts to receive and save the wind data. Each wind data packet is a string of numbers separated by commas. The first number is a timestamp, and the rest are the anemometer readings.

## Modes

The anemometer supports four running modes:
* READ_ANEMOMETER
* SEND_PIN_READ
* PRINT_IP
* CALIBRATE

The default mode is CALIBRATE.

## File Structure

The files in this project are organized as follows:

- plot.pptx
- logic_board/
    - Various KiCAD and SVG files for logic board design
- main_board/
    - Various KiCAD and SVG files for main board design
- firmware/
    - anemometer/
        - Python scripts to read data from the anemometer
        - ESP32 firmware files
        - Additional files for Visual Studio Code and PlatformIO

For a detailed view of the file structure, refer to the file list provided earlier.

## Note

The readings of different anemometers for the same wind strength might not be the same. However, the change in readings should accurately reflect the changes in wind conditions.

## Contribution 

The design of main board and logic board is done by Filip Ayazi. The firmware is written by Yuyi Zhang and ChatGPT, actually mostly by ChatGPT.
