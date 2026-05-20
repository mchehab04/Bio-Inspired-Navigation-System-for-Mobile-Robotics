# Bio-Inspired Navigation System for Mobile Robotics

## Overview
This project presents a biologically inspired navigation system for mobile robotics implemented using the Promethe2 neural simulator. The system draws inspiration from mammalian navigation mechanisms such as Head Direction (HD) cells, Place Cells (PCs), landmark-based navigation, and visual attention mechanisms.

The project was developed and tested on a TurtleBot3 Waffle Pi robotic platform using real-world sensory inputs including:
- Camera vision
- LIDAR
- Compass/orientation sensing
- Joystick interaction
- Arduino/OpenCR communication

The navigation architecture combines:
- Visual landmark extraction
- Log-polar visual processing
- Head direction encoding
- Place cell learning and categorization
- Action association and motor control
- Obstacle avoidance

---

## Project Structure

```text
senior_design_project/
│
├── learning_place_cells_actions/
│   Main biologically inspired navigation system
│
├── obstacle_avoidance/
│   Obstacle avoidance subsystem using LIDAR
│
└── test_scripts/
    Hardware and subsystem validation scripts
```

---

## Main Features

- Bio-inspired navigation architecture
- Place cell learning and recognition
- Head direction representation
- Landmark categorization
- Log-polar visual processing
- Real-world robot integration
- Obstacle avoidance
- Camera-based visual attention
- Sensor integration using Promethe2

---

## Technologies Used

- Promethe2 Neural Simulator
- C
- TurtleBot3 Waffle Pi
- Raspberry Pi
- OpenCR
- Arduino
- Python
- LIDAR
- Computer Vision

---

## Repository Contents

### learning_place_cells_actions
Contains the primary navigation system implementing:
- Place cell learning
- Landmark recognition
- HD cell representation
- Visual processing
- Action association

### obstacle_avoidance
Contains the obstacle avoidance subsystem integrated with:
- LIDAR sensing
- Navigation control
- Arduino/OpenCR communication

### test_scripts
Contains standalone hardware and software validation scripts for:
- Camera testing
- LIDAR testing
- Joystick testing
- Arduino communication
- Python utilities

---

## Research Inspiration

This project is inspired by biological navigation systems observed in mammals, particularly:
- Hippocampal Place Cells
- Head Direction Cells
- Visual attention and foveation
- Landmark-based spatial memory

---

## Author

Mohamad Chehab  
Computer Engineering Graduate  
American University of Sharjah

---

## Notes

This repository contains experimental scripts, Promethe2 configurations, hardware interface files, and testing utilities developed throughout the senior design project.