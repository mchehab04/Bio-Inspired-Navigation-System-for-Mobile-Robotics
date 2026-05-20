# Obstacle Avoidance System

## Overview
This folder contains the obstacle avoidance subsystem developed for the robot platform using Promethe2 and LIDAR sensing.

The system processes distance measurements from the LIDAR sensor and generates obstacle avoidance behaviors for safe robot navigation.

---

## Main Features

- Real-time obstacle detection
- LIDAR-based environment sensing
- Navigation correction
- Robot motion control
- Arduino/OpenCR integration

---

## Important Files

| File | Description |
|---|---|
| `obstacle.script` | Main obstacle avoidance script |
| `obstacle.config` | Simulation configuration |
| `obstacle.draw` | Network visualization |
| `laser.hwl` | LIDAR hardware configuration |
| `joystick.hwj` | Joystick configuration |
| `obstacle_avoidance_with_compass.ino` | Arduino/OpenCR control code |

---

## Tests Folder

Contains helper shell scripts for:
- Compilation
- File conversion
- Testing workflows

---

## Hardware Used

- LDS LIDAR
- TurtleBot3 Waffle Pi
- Raspberry Pi
- OpenCR

---

## Notes

This subsystem was developed separately from the place-cell navigation architecture and can be integrated into larger navigation systems for real-world robotic operation.