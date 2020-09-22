**TODO: Formatting**

TUSatellite7

Taylor University's Satellite 7
Fall 2020

Credits:
Steven Dalcher (Project Manager)
Jeff Jewett (Student Project Manager)
Isaac Wickham
Mitchell Toth



Purpose:
To test and integrate different sensors in low-earth orbit. On this mission, our focus was on taking, analyzing, and transmitting 
useful low-resolution infrared images with the GridEye sensor, plus attempting to detect any bit flips on our EEPROM, and correlate
such bit flips with uv/gamma ray levels. We are building on our previous code for Satellite 6 (https://github.com/Jeffjewett27/SatelliteTU2020)



Hardware:
NearSpace Education ThinSat Sensor Board v4 
-LSM9DS1TR 9-axis IMU (acceleration, gyroscope, magnetic field)
-AMG8833 GridEye IR Sensor (8x8 pixel infrared)
-Teviso BG51 Radiation (Cosmic Particle/Gamma Detector)
-Three TMP36 temperature sensors
-Two UV Light Sensors: GUVA-S12SD UVA (240nm ~ 370nm) and GUVC-S10GD UVC (220nm ~ 280nm)
-SFH3310 Phototransistor
-TSL235R-LF Light to Frequency Phototransistor
-M24512 EEPROM
-Eight  MCP3208 analog inputs on an A/D converter
RGB color detector


Sensor Readings:
IMU (i2c)
-acceleration: 3-axis g's <Vector3>
-gyroscope: 3-axis deg/s <Vector3>
-magnetometer: 3-axis Gauss <Vector3>
GridEye (i2c)
-8x8 array of pixels <int16_t[64]>
-pixel format: 12 bit (11 bits + sign bit); divide by 4 to get degrees C <int16_t>
EEPROM (i2c)
-64KB = 512Kb
UVA (analog)
-units: Amps per Watts/cm2
-equation: amps = a*UVA power + b (determine constants)
UVC (analog)
-units: Amps per Watts/cm2
-equation: amps = a*UVA power + b (determine constants)
Temperature (analog)
-10 mV/°C scale factor (determine constant offset?)
Radiation (pulse count)
-5 cpm/µSv/h
Phototransistor
-In scope?
Light to Frequency (pulse count)
-TODO: locate datasheet
RGB color detector
-Which model?

Utilities:
-.linux-side.sh is a bash script to update the project file (TUSatellite7.side)
-.powershell-side.ps1 is a powershell script to update the project file (TUSatellite7.side)
-.side-compile.txt contains information about the SimpleIDE compiler options (leave as default)

-Vector3.h is a 3-vector of floats
-Packet.h contains header info + 32 bytes of data for a packet
-PacketQueue.h is a queue data structure which holds packets
-DataConversion.h contains functions to switch between 32-bit and 16-bit floats (at cost of precision)
