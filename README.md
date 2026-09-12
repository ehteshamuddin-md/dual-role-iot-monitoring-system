# Dual-Role IoT Monitoring System

An ESP32-based IoT monitoring system with separate dashboards for Medical Staff and Facility Management using MQTT and Node-RED.

## Overview

The system monitors both patient health parameters and hospital facility conditions.

### Medical Staff Dashboard
- Heart Rate
- SpO₂
- Body Temperature

### Facility Management Dashboard
- Room Temperature
- Oxygen Level
- Air Quality Index (AQI)

The data is transmitted using separate MQTT topics and displayed through role-specific dashboards.

## System Architecture

ESP32 (Wokwi)
↓
Wi-Fi
↓
HiveMQ MQTT Broker
↓
Node-RED
↓
Role-Based Dashboards

## Technologies Used

- ESP32
- Wokwi
- MQTT
- HiveMQ MQTT Broker
- Node-RED
- FlowFuse Dashboard
- C++
- DHT22 Sensor
- Potentiometers for sensor simulation

## MQTT Topics

### Patient Data

hospital/patient/heart_rate  
hospital/patient/spo2  
hospital/patient/body_temperature

### Facility Data

hospital/facility/room_temperature  
hospital/facility/oxygen_level  
hospital/facility/aqi

### Summary Data

hospital/summary/patient  
hospital/summary/facility

## Features

- Real-time IoT monitoring
- Separate medical and facility dashboards
- MQTT-based communication
- Gauge-based visualization
- Abnormal-condition alerts
- Persistent alert feeds
- 30-second periodic data aggregation
- ESP32 simulation using Wokwi

## Wokwi Simulation

The ESP32 prototype is simulated in Wokwi.

Potentiometers are used to simulate:
- Heart Rate
- SpO₂
- Body Temperature
- Oxygen Level
- AQI

A DHT22 sensor is used for Room Temperature.

## Project Files

- `sketch.ino` – ESP32 program
- `diagram.json` – Wokwi circuit configuration
- `libraries.txt` – Required libraries
- `Dual_Role_IoT_Monitoring_System_Project_Report.pdf` – Complete project report

## Note

The system is an engineering prototype for IoT monitoring demonstration. The simulated sensor values and alert thresholds are not intended for clinical diagnosis.

The HiveMQ public broker is used for demonstration and testing. Production deployment would require MQTT authentication, TLS encryption, and Access Control Lists (ACLs).

## Author

**Ehtesham**
