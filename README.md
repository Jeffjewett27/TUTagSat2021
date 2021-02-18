# TUSatellite7

Taylor University's Satellite 7 designed in Fall 2020

## Primary Contributors

- Steven Dalcher (Project Manager)
- Jeff Jewett (Student Project Manager)
- Isaac Wickham
- Mitchell Toth
- Joseph Vanderharst

## Purpose

To test and integrate different sensors in low-earth orbit. On this mission, our focus was on taking, analyzing, and transmitting 
useful low-resolution infrared images with the GridEye sensor, plus attempting to detect any bit flips on our EEPROM, and correlate
such bit flips with uv/gamma ray levels. We are building on our previous code for TU Satellite 6 (https://github.com/Jeffjewett27/SatelliteTU2020)



## Hardware

- NearSpace Education ThinSat Sensor Board v4 
   - LSM9DS1TR 9-axis IMU (acceleration, gyroscope, magnetic field)
   - AMG8833 GridEye IR Sensor (8x8 pixel infrared)
   - Teviso BG51 Radiation (Cosmic Particle/Gamma Detector)
   - Three TMP36 temperature sensors
   - Two UV Light Sensors: GUVA-S12SD UVA (240nm ~ 370nm) and GUVC-S10GD UVC (220nm ~ 280nm)
   - SFH3310 Phototransistor
   - TSL235R-LF Light to Frequency Phototransistor
   - M24512 EEPROM
   - Eight  MCP3208 analog inputs on an A/D converter
- RGB color detector


## Sensor Readings
- IMU (i2c)
   - acceleration: 3-axis g's `Vector3`
   - gyroscope: 3-axis deg/s `Vector3`
   - magnetometer: 3-axis Gauss `Vector3`
- GridEye (i2c)
- 8x8 array of pixels `int16_t[64]`
- pixel format: 12 bit (11 bits + sign bit); divide by 4 to get degrees C `int16_t`
- EEPROM (i2c)
   - 64KB = 512Kb
- UVA (analog)
   - units: Amps per Watts/cm2
   - equation: amps = `a*UVA power + b` (determine constants)
- UVC (analog)
   - units: Amps per Watts/cm2
   - equation: amps = `a*UVA power + b` (determine constants)
- Temperature (analog)
   - 10 mV/°C scale factor (determine constant offset?)
- Radiation (pulse count)
   - 5 cpm/µSv/h
- Light to Frequency (pulse count)

## Utilities

- .linux-side.sh is a bash script to update the project file (TUSatellite7.side)
- .powershell-side.ps1 is a powershell script to update the project file (TUSatellite7.side)
- .side-compile.txt contains information about the SimpleIDE compiler options (leave as default)

- Vector3.h is a 3-vector of floats
- Packet.h contains header info + 32 bytes of data for a packet
- PacketQueue.h is a queue data structure which holds packets
- DataConversion.h contains functions to switch between 32-bit and 16-bit floats (at cost of precision)

## Packet List

- GeneralSensor: **DESCOPED**
- IMU:
   - accxcomp [2x16]: 0x20
   - accycomp [2x16]: 0x21
   - acczcomp [2x16]: 0x22
   - gyroxcomp [2x16]: 0x23
   - gyroycomp [2x16]: 0x24
   - gyrozcomp [2x16]: 0x25
   - magxcomp [2x16]: 0x26
   - magycomp [2x16]: 0x27
   - magzcomp [2x16]: 0x28
   - accx [4x8]: 0x40
   - accy [4x8]: 0x41
   - accz [4x8]: 0x42
   - gyrox [4x8]: 0x43
   - gyroy [4x8]: 0x44
   - gyroz [4x8]: 0x45
   - magx [4x8]: 0x46
   - magy [4x8]: 0x47
   - magz [4x8]: 0x48
- GridEye:
   - imga: 0x30
   - imgb: 0x31
   - imgc: 0x32
- BitFlip:
   - detected: 0x33
- Analog:
   - uva [4x8]: 0x11
   - uvc [4x8]: 0x12
   - temp1 [4x8]: 0x13
   - temp2 [4x8]: 0x14
   - temp3 [4x8]: 0x15
- PulseCount:
   - 15 ltf, 1 radiation [ltf1,ltf1,ltf2,ltf2,...,ltf15,ltf15, rad1, 0x0]