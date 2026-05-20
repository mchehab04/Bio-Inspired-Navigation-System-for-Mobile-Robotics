# Arduino Sensor Test

## Overview
This folder contains Promethe2 scripts and hardware configurations used to test Arduino/OpenCR sensor communication.

The goal was to validate:
- Serial communication
- Sensor reading
- Data transfer into Promethe2

---

## Important Files

| File | Description |
|---|---|
| `arduino_sensor.script` | Main test script |
| `arduino_sensor.config` | Configuration file |
| `arduino_port.hws` | Serial port configuration |
| `arduino_inputs.hwc` | Input hardware configuration |
| `arduino_outputs.hwc` | Output hardware configuration |
| `obstacle_avoidance_with_compass.ino` | Arduino/OpenCR firmware |

---

## Purpose

Used to validate communication between:
- Raspberry Pi
- OpenCR/Arduino
- Promethe2 neural groups