Source writer: Yunseo Lee, Wonbin Lee
Last Update: 2024/06/25

Project Name : [NRF GrandPrix]
Description : This is a project making Car Racing Gaming device which also can detect the information of circumstances using nrf52840 board and Zephyr RTOS.


# Project Structure and Details

- /nrf52840dk_nrf52840.overlay: Device Tree Configuration
This file configures the device tree for the nRF52840 development kit, setting up hardware peripherals such as GPIOs, ADCs, PWMs, and sensors.

- /prj.conf: Kernel Configuration
This file contains the kernel configuration for the Zephyr OS. It sets up various kernel features and system configurations required for the application, including Bluetooth, GPIO, and sensor support.

- /README.txt: Build and Usage Instructions
This document provides step-by-step instructions on how to build and use the program. It includes prerequisites, build commands, and operational guidelines for running the application on the nRF52840 development kit.

**/src**

- batteryDisplay.c: Arduino Rich Shield Board Battery Display Code
This file contains code to interface with the battery display on the Arduino Rich Shield Board. It includes functions to initialize the display, set brightness levels, and update the display to show the current battery status.

- ble.c: BLE Communication Code with Mobile Phone
This file handles Bluetooth Low Energy (BLE) communication between the nRF52840 and a mobile phone. It includes initialization of the BLE stack, setting up GATT services and characteristics, and managing connections and data transfers.

- buzzer.c: Code for Buzzer Connected to nRF52840
This file contains code to control a buzzer connected to the nRF52840. It includes initialization of PWM settings and functions to play different tones and sound patterns.

- co2.c: CO2 Sensor Code
This file interfaces with a CO2 sensor connected to the nRF52840. It includes sensor initialization, reading CO2 levels, and processing sensor data for further use in the application.

- gpios.c: GPIO Configuration Code
This file manages the configuration of General-Purpose Input/Output (GPIO) pins on the nRF52840. It includes setting up button interrupts, configuring pin modes, and handling GPIO callbacks for various input events.

- joy.c: Arduino Rich Shield Board Joystick Code
This file contains code to interface with the joystick on the Arduino Rich Shield Board. It includes ADC reading functions, joystick position tracking, and event handling for joystick movements.

- led.c: Arduino Rich Shield Board LED Matrix Code
This file manages the LED matrix on the Arduino Rich Shield Board. It includes functions to initialize the LED matrix, set LED states, and display patterns or animations on the matrix.

- main.c: Main Thread
This file contains the main entry point and primary execution loop for the application. It initializes the system, starts necessary threads, and manages the overall control flow of the program.

- map.s: Assembly Function for Mapping Sound Values
This assembly file includes functions to map sound values for the application. It provides low-level routines for sound data processing and conversion.

- rotary.c: Arduino Rich Shield Board Rotary Encoder Code
This file interfaces with the rotary encoder on the Arduino Rich Shield Board. It includes initialization of the encoder, handling rotation events, and updating related application states based on encoder input.

- sound.c: Arduino Rich Shield Board Sound Code
This file manages sound-related functionalities for the Arduino Rich Shield Board. It includes playing sound effects, controlling volume, and interfacing with sound hardware components.

- timer.c: Code for Timing Frames During Game Execution
This file handles the timing of frames during game execution. It includes setting up timers, managing frame rates, and ensuring that game frames are rendered at consistent intervals.


# About Hardware

To run this program successfully, you need to have the right hardware configuration for your conditions. First you need nrf52840 board and Open-Smart Rich Shield Two. Also you need passive buzzer and MH-Z14A CO2 sensor.
- Passive Buzzer: Connect (-)pin of buzzer to GND pin of the board and connect (+)pin of buzzer to P0.13 pin of the board.
- MH-Z14A: Connect pins 16, 17, 18, and 19 to the GND, 5V, p1.8, and p1.10 pins on the board, respectively.



# How to build?

1. Download source codes to your local space.
2. If there are no NRF Connection SDK in your VSCode, please download the extension.
3. Open the source code directory in VSCode.
4. Make build configuration. Don't forget to include prj.conf and overlay file to your build configuration.
5. Build and Flash the project to your board.


# How to run?

After the program is successfully built, the main screen will appear on the LED Matrix.

There are four buttons on the board that you can click to execute the desired function.

Button 1: Launch Game -> Move joystick up and down ! 
Button 2: Detect CO2
Button 3: Sound detection
Button 4: Check board status

Also, you can see board's various information on NRF connect via BLE.

1. Install NRF connect on your mobile.
2. Turn on the app
3. Find "My_device" from the list and connect.
4. Make it be able to accept the alarm from nrf device. You can see this in client section.
5. Change the data type as UTF-8.
6. Go to Log section and check the messages.

# Caution !

During use the device, program errors may occur for various reasons (e.g., a pin may fall out). In this case, press the reset button to restart from the main screen.