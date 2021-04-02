# TUTagSat2021

Taylor University's TagSat2021 code in partnership with NSL. Based off TUSatellite7. Developed in March, 2021.

## Primary Contributors

- Jonathon Fritsch (NSL Engineering Technician)
- Jeff Jewett (Lead Software Developer)
- Steven Dalcher (Taylor)
- Stefan Brandle (Taylor)
- Matt Orvis (NSL)

## Hardware

- NearSpace Education ThinSat Sensor Board v4 
   - LSM9DS1TR 9-axis IMU (acceleration, gyroscope, magnetic field)
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
- EEPROM (i2c)
   - 64KB = 512Kb
- UVA (analog)
   - units: Amps per Watts/cm2
- UVC (analog)
   - units: Amps per Watts/cm2
- Temperature (analog)
   - 10 mV/°C scale factor
- Radiation (pulse count)
   - 5 cpm/µSv/h
- Light to Frequency (pulse count)

## Utilities

- .linux-side.sh is a bash script to update the project file (TUTagSat2021.side)
- .powershell-side.ps1 is a powershell script to update the project file (TUTagSat2021.side)
- .side-compile.txt contains information about the SimpleIDE compiler options (leave as default)

- Vector3.h is a 3-vector of floats
- Packet.h contains header info + 32 bytes of data for a packet
- PacketQueue.h is a queue data structure which holds packets
- DataConversion.h contains functions to switch between 32-bit and 16-bit floats (at cost of precision)

## Packet List

- IMU (float32 values):
   - accx [4x8]: 0x40
   - accy [4x8]: 0x41
   - accz [4x8]: 0x42
   - gyrox [4x8]: 0x43
   - gyroy [4x8]: 0x44
   - gyroz [4x8]: 0x45
   - magx [4x8]: 0x46
   - magy [4x8]: 0x47
   - magz [4x8]: 0x48
- Analog (uint16_t values):
   - uva [4x8]: 0x11
   - uvc [4x8]: 0x12
   - temp1 [4x8]: 0x13
   - temp2 [4x8]: 0x14
   - temp3 [4x8]: 0x15
- PulseCount (uint16_t):
   - ltf [2x16]: 0x16
   - rad [1x32]: 0x17