# ESP32-Dual-Core-MQTT-Assignment

# Overview

This assignment demonstrates the use of ESP32's dual-core capabilities to handle Wi-Fi connectivity, MQTT communication, and UART serial communication simultaneously. The program utilizes Core 0 for network-related tasks and Core 1 for UART communication with a laptop.

# Features

Core 0
  Connects ESP32 to a Wi-Fi network and indicates connection status via an LED.

  Establishes an MQTT connection with a free broker and signals success with an LED.

  Sends a serial message: "MQTT connected" upon successful connection.

  Transfers received serial data to Core 1 for MQTT publishing.

  Blinks LED every 0.5 seconds if MQTT is connected.
