# Arduino-gyro-robot
Navigating an Arduino robot using MPU6050 for gyro-based navigation


## Demo
[▶️ Watch Robot Demo](https://youtu.be/vVrv0_XRITA)


## Overview

This project is an autonomous Arduino robot that uses an MPU6050 gyroscope for real-time direction correction and precise turning. By using yaw feedback instead of timed delays, the robot achieves more accurate and reliable navigation.


## Features

* Gyro-based straight-line correction
* Accurate 90° and 180° turns
* Modular movement functions
* Autonomous navigation sequence

## Hardware

* Arduino Uno
* MPU6050 Gyroscope
* Motor Driver
* DC Motors

## Libraries Used
- Wire.h
- MPU6050 (I2Cdevlib)

## Code

Main logic is in `Arduino-gryo-robot.ino`


## Key Concept

This project uses closed-loop control based on gyroscope (yaw) feedback. The robot continuously compares its current heading to a target direction and adjusts motor output to correct any error. This enables accurate navigation and turning without relying on unreliable time-based movement.

## Challenges & Improvements
- Gyroscope drift required calibration and angle normalization
- Motor speed differences caused heading errors that required correction logic
- Future improvement: implement PID control for smoother correction
