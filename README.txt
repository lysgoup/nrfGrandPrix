Project Name : [NRF GrandPrix]
Description : This is a project making Car Racing Gaming device which also can detect the information of circumstances using nrf52840 board and Zephyr RTOS.

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
Button 1: Launch Game
Button 2: Detect CO2
Button 3: Sound detection
Button 4: Check board status

During use the device, program errors may occur for various reasons (e.g., a pin may fall out). In this case, press the reset button to restart from the main screen.