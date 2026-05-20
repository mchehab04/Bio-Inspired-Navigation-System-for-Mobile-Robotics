# Learning Place Cells and Actions

## Overview
This folder contains the primary biologically inspired navigation system developed in Promethe2. The system models several concepts inspired by mammalian navigation, including:

- Place Cells (PCs)
- Head Direction (HD) Cells
- Landmark-based navigation
- Visual attention and foveation
- Action association and recall

The network learns visual landmarks from camera input and associates them with spatial orientation and robot actions.

---

## Main Components

### Visual Processing
The visual pipeline extracts salient regions from camera images and processes them using biologically inspired methods such as:
- Focus point extraction
- Log-polar visual representation
- Landmark categorization

### Place Cell Learning
The system creates place representations based on:
- Visual landmark configurations
- Relative landmark orientation
- Head direction information

### Action Learning
Robot actions are associated with learned place cells, allowing the robot to recall navigation behaviors when revisiting known locations.

---

## Important Files

| File | Description |
|---|---|
| `place_cells.script` | Main Promethe2 navigation script |
| `place_cells.config` | Simulation configuration |
| `place_cells.draw` | Network visualization |
| `place_cells.gcd` | Group configuration data |
| `compass_with_joystick_controls.ino` | Arduino/OpenCR compass and joystick control |
| `learn_file` | Stored learning parameters |
| `capture.hwc` | Camera hardware configuration |
| `laser.hwl` | LIDAR configuration |
| `joystick.hwj` | Joystick configuration |

---

## Input Folder

The `Input/` folder contains:
- Vision masks
- Gaussian filters
- Attention masks
- Learning masks
- Visual preprocessing files
- Experimental scripts

---

## Test Images

The `test_images/` folder contains sample landmark images used during testing and validation.

---

## Hardware Used

- TurtleBot3 Waffle Pi
- Raspberry Pi
- OpenCR
- USB camera
- LDS LIDAR
- Compass sensor
- Joystick controller

---

## Notes

This folder represents the main implementation of the senior design project and contains the complete navigation architecture used during experimentation and testing.