# ClimateGuard-360-ESP32

This repository hosts the ESP32 firmware for the ClimateGuard-360 project. It includes separate modules for sensors and actuators, facilitating a comprehensive environmental monitoring and control system.

## Overview

The ESP32 firmware is split into two main modules:
- **Sensor Module**: Collects environmental data using various sensors.
- **Actuator Module**: Manages output devices based on sensor data and user inputs.

## Hardware Components

### Sensor Module
- **Rain Sensor**: Detects rain intensity.
- **MQ-135**: Measures air quality, particularly ammonia, nitrogen oxides, smoke, and other harmful gases.
- **BH1750**: A digital ambient light sensor providing lux measurements.
- **DHT11**: Measures temperature and humidity.
- **BMP280**: A sensor for measuring barometric pressure and temperature.

### Actuator Module
- **Display**: Shows system status and sensor readings.
- **Speaker**: Issues audio alerts based on sensor data.
- **Buttons**: Allow manual control over actuator actions.
- **LEDs**: Visual indicators for system status.
- **Fan**: Regulates air flow based on sensor data.
- **Servo**: Operates physical mechanisms as required.

## Communication

- **MQTT**: Primary method for syncing data between ESP32 modules and the server.
- **ESP-NOW**: Used as a backup communication protocol between ESP32 devices.
- **HTTP Requests**: ESP32 can directly interact with the SQL database via PHP scripts for data insertion when not handled by the Vaadin web application.

## Software and Libraries

- Developed using PlatformIO, which simplifies configuration and dependency management.
- Utilizes well-known libraries for sensor interfacing and network communication.

## Setup and Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/THR4GG/ClimateGuard-360-ESP32.git
   ```
2. **Navigate to the appropriate directory** for the sensor or actuator module.
3. **Open the project in PlatformIO**.

## Configuration

- Configure MQTT broker details in the code to match your network setup.
- Set up HTTP and ESP-NOW settings according to your system requirements.

## Deployment

- Compile and upload the firmware to the ESP32 devices using PlatformIO.
- Ensure all devices are correctly configured to connect to the same MQTT broker.

## Contributing

Contributions that improve the reliability, efficiency, or functionality of the sensors or actuators are welcome. Please fork the repository and submit your pull requests.

## License

This project is licensed under the Unlicense, allowing free use, modification, and distribution.