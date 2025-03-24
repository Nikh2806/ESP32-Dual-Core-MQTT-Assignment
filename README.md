# ESP32-Dual-Core-MQTT-Assignment


## Overview
This assignment demonstrates the use of ESP32's dual-core capabilities to handle Wi-Fi connectivity, MQTT communication, and UART serial communication simultaneously. The program utilizes Core 0 for network-related tasks and Core 1 for UART communication with a laptop.


## Features

###	Core 0

 o	Connects ESP32 to a Wi-Fi network and indicates connection status via an LED.
 
 o	Establishes an MQTT connection with a free broker and signals success with an LED.
 
 o	Sends a serial message: "MQTT connected" upon successful connection.
 
 o	Transfers received serial data to Core 1 for MQTT publishing.
 
 o	Blinks LED every 0.5 seconds if MQTT is connected.  

###	Core 1

 o	Initializes UART communication at 9600 baud.
 
 o	Sends "UART Initialized" over the serial port.
 
 o	Reads input messages from the serial port (using Termite or Android Studio).
 
 o	Sends received messages to ESP32, which then transfers them via MQTT.


## Requirements

 •	ESP32 development board

 •	Arduino IDE (or ESP-IDF if preferred)

 •	MQTT broker (e.g., Mosquitto, HiveMQ)

 •	Termite or Android Studio for serial communication

 •	Wi-Fi connectivity


## Setup Instructions

1.	Install Dependencies

    o	Install the required libraries in Arduino IDE:

     ![image](https://github.com/user-attachments/assets/b27ab222-1c54-4026-a166-808cb3c063ef)


    o	Ensure drivers for ESP32 are installed.

3.	Flash the Code

    o	Upload the program to ESP32 via Arduino IDE.

    o	Open the Serial Monitor (9600 baud) to check logs.

4.	Test the Functionality

    o	Observe LED behaviour for Wi-Fi and MQTT connection status.

    o	Use Termite/Android Studio to send messages via UART.

    o	Monitor messages published via MQTT broker.
  	

## Expected Behaviour

 •	ESP32 connects to Wi-Fi and blinks an LED every 2 seconds.

 •	Upon successful MQTT connection, LED remains on and "MQTT connected" is printed on   Serial Monitor.

 •	UART initializes and displays "UART Initialized".

 •	Messages from the serial terminal are published via MQTT.

 •	If MQTT is connected, LED blinks every 0.5 seconds.
 

## License

  This project is open-source. Feel free to modify and improve it.
