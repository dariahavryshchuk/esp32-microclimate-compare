# ESP32 Microclimate Compare

I built this project to gain more experience working with hardware and connecting physical sensors to software. The system uses an ESP32 to measure environmental conditions and a web interface I developed to compare measurements from different locations.

## What It Does

The system measures four environmental conditions:

- Temperature (°F)
- Humidity (%)
- Atmospheric pressure (hPa)
- Light intensity (lux)

The ESP32 collects measurements from two sensors and displays them on a web page. The readings update approximately every second.

I also added a feature that allows users to save measurements from different locations. Each snapshot can be given a name, such as "Desk" or "Window." The user can then select two snapshots and compare the differences in temperature, humidity, pressure, and light.

The snapshots are saved in the browser's local storage.

## Hardware

- ESP32 development board
- BME280 sensor for temperature, humidity, and pressure
- BH1750 sensor for light intensity
- Jumper wires

Both sensors communicate with the ESP32 using I2C.

## Software and Libraries

I programmed the ESP32 using Arduino IDE and C++.

The project uses these libraries:

- Wire
- WiFi
- WebServer
- Adafruit Unified Sensor
- Adafruit BME280
- hp_BH1750

The web interface uses HTML, CSS, and JavaScript, which are generated and served by the ESP32.

## How to Run the Project

1. Install ESP32 board support and the required libraries in Arduino IDE.
2. Connect the BME280 and BH1750 sensors to the ESP32 using I2C.
3. Open `MicroclimateCompare_final.ino` in Arduino IDE.
4. Replace `YOUR_WIFI_NAME` and `YOUR_WIFI_PASSWORD` with your own Wi-Fi information in your local copy.
5. Select the correct ESP32 board and upload the code.
6. Open Serial Monitor at 9600 baud to find the ESP32's IP address.
7. Enter that IP address in a browser connected to the same Wi-Fi network.

The web page will display live measurements and allow you to record and compare snapshots.

## A Problem I Encountered

One problem I encountered was that the ESP32 connected to Wi-Fi, but the BME280 sensor wasn't detected.

After checking the sensor communication, I found that the I2C address was incorrect. I changed the initialization to use address `0x77`, and the sensor started working.

This was an important part of the project because it helped me better understand how the hardware and software communicate.

## Project Demonstration

My MIT Maker Portfolio video includes a demonstration of the physical setup, live environmental readings, and the web interface used to compare measurements.
