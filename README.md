# Lit Brolly

*Nōn omne quod nitet aurum est.*

A project by Aaron Tan, Lily Yang, and [Hengcheng Yu](https://github.com/handchan) to make your brollies lit. The LEDs change colour based on the movement detected by the accelerometer!

![alt tag](https://github.com/cheeseisdisgusting/lit-brolly/blob/master/Docs/Resources/lit-brolly-trimmed.gif)

---

### Contents
1. [Setup](#setup)
	- [Requirements](#requirements)
	- [Compilation](#compilation)
	- [Hardware](#hardware)
	- [Execution](#execution)
2. [Usage](#usage)
	- [Synergy](#synergy)
	- [Arduino](#arduino)
	- [ADXL345](#adxl345)
3. [Miscellaneous](#miscellaneous)
	- [Acknowledgements](#acknowledgements)
	- [License](#license)

---

## Setup
### Requirements
- Onion Omega2 (should work with the 2+ but this has not been tested)
- Arduino Dock R2
- Neopixels
- ADXL345 accelerometer
- Time. Lots of it.

### Compilation

**Note: Pre-compiled executables are available in the folder ```Code/executable```. Only follow the instructions under the Compilation heading if you really want to compile from source.**

- Set up the virtual machine as described in the PDF labelled "SetupCrossCompilingEnvironmentV1.pdf" in ```Docs/Compile```. In the ```make menuconfig``` step, include the onioni2c library. This will take upwards of an hour of your time.
- Modify the target of the makefile under the compile folder to the name of the file that you wish to compile. By default, it will try to compile ```Synergy.cpp```.
- Move the file you want to compile into the same folder as your ```makefile``` and ```xCompile.sh```.
- Once the makefile, the target, and xCompile.sh are in the same folder, run xCompile.sh as described in the PDF labelled "CompilingCppCodeOmega.pdf" in ```Docs/Compile```. An executable file will be generated in the same folder.

### Hardware
- Solder the Neopixels and hook them up to the Arduino Dock R2 headers as described in the schematic under ```Docs/Schematics```. This project supports hooking up to five strands of Neopixels; connect the power wires in series and data pins to 6, 7, 8, 9, and 10 on the Arduino Dock R2 headers.
- Solder header pins on the ADXL345 as necessary and hook them up to the Onion Expansion headers as described in the schematic under ```Docs/Schematics```.
- A breadboard may prove handy for cable management and organization. See ```Example.png``` under ```Docs/Schematics``` for an real-life example of this schematic put together.

### Execution
- Move your executable to a USB stick, then mount it on your Omega:
	- Connect to your Omega's Wi-Fi network, then using PuTTY on Windows or your terminal on Mac/Linux: ```ssh root@192.168.3.1```. You will need to type in the password for the Omega2: default is onioneer.
	- In the terminal: ```mkdir /mnt/usb``` (first time only, making a directory to be the mount point for your USB stick) then ```mount /dev/sda1 /mnt/usb``` (actually mounting the USB)
- Connect to the Arduino Dock (I prefer wirelessly, but it shouldn't matter), and upload the ```ardu.ino``` sketch under ```Code/src```.
- Run Synergy from the command line: ```/mnt/usb/Synergy``` (assuming you followed the previous directions)
- If you wire up the Neopixels correctly (i.e. the way it's described under the "Final Project Report" appendix), then you should see your Neopixels light up when you move around the accelerometer!

## Usage
### Synergy
Main program; puts everything together (synergy... hahaha get it?). Running the file requires no command line arguments.

- Program will continually pull accelerometer readings every second and output to terminal.
- Lights will change colour every second.
- Generates two log files directly on the Omega2; can be accessed by ```cat```:
	- ```log.txt``` logs, with timestamp, the last readings of the accelerometer and the status of the I2C write command to the Neopixels.
	- ```histogram.txt``` logs, with timestamp of generation time, a histogram of the x, y, and z accelerations as well as total acceleration over the period that the program runs. It is updated every ten seconds with new accelerometer values.
- **Must have ardu.ino sketch running on the Arduino dock for this to work.**

### Arduino
Program to test Neopixels. Starting program requires four integer command line arguments: ```[brightness] [red value] [green value] [blue value]```

- Program sets all Neopixels to the RGB values specified by arguments. Brightness value is vestigial and doesn't actually do anything.
- **Must have ardu.ino running on the Arduino dock for this to work.**

### ADXL345
Program to test the accelerometer. No command line arguments required.

- Pulls accelerometer values every second and outputs to command line.

## Miscellaneous
### Acknowledgements
Big thanks to Austin Jiang for his vehicle.

Big thanks to (Aaaaaaaaaaaaaaaaaaaaaaaa)Kash Pirani for his soldering and head patting skills.

Big thanks to Edward Yang and Shun Yu Rao for moral support and their USB sticks.

Big thanks to Google Assistant, Cortana, and Siri for turning on and off the lights.

Big thanks to Ferrero Rocher for being the Official Snack Food™ of this project.

### License
See LICENSE.md for more details.