# Test Scripts and Hardware Validation

## Overview
This folder contains standalone testing environments and utility scripts used to validate individual hardware components and subsystems during development.

The purpose of these scripts was to ensure proper communication between Promethe2 and the robot hardware before integration into the full navigation system.

---

## Folder Structure

| Folder | Description |
|---|---|
| `arduino_sensor/` | Arduino/OpenCR communication tests |
| `camera/` | Camera and image capture tests |
| `joystick_demo/` | Joystick communication tests |
| `python_scripts/` | Python helper and debugging utilities |
| `test_lidar/` | LIDAR testing and Promethe2 integration |

---

## Main Testing Areas

### Arduino Communication
Validation of:
- Serial communication
- Sensor data transfer
- Motor command transmission

### Camera Testing
Validation of:
- USB camera input
- Image capture
- Promethe2 camera interfaces

### Joystick Testing
Validation of:
- RC100/BT-410 joystick packets
- Manual robot control
- Serial decoding

### LIDAR Testing
Validation of:
- LDS sensor communication
- Laser scan extraction
- Integration with Promethe2 `f_laser` group

---

## Python Utilities

Python helper scripts include:
- Reading joystick packets
- Reading LIDAR data
- Distance debugging
- RC-to-joystick conversion

---

## Notes

These scripts were primarily used during hardware debugging and system integration stages of the project.