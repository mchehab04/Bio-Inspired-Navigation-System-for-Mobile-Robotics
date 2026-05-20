# LIDAR Test

## Overview
This folder contains scripts and source modifications used to integrate and test the LDS-02 LIDAR sensor with Promethe2.

---

## Features Tested

- LIDAR communication
- Distance extraction
- Laser scan processing
- Promethe2 laser group integration

---

## Important Files

| File | Description |
|---|---|
| `test.script` | Main LIDAR test script |
| `laser.hwl` | LIDAR hardware configuration |
| `f_laser.c` | Modified Promethe2 laser group |
| `laser_init.c` | LIDAR initialization |
| `laser_get_values.c` | Laser data extraction |
| `laser_get_intensity_by_degres.c` | Degree-based intensity extraction |

---

## files_to_update_in_promethe2_for_LDS02

Contains modifications required for supporting the LDS-02 sensor inside Promethe2.

---

## Notes

These modifications were created to extend Promethe2 compatibility with the LDS-02 LIDAR sensor used on the TurtleBot3 platform.