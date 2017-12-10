# Lit Brolly

*Nōn omne quod nitet aurum est.*

A project by Aaron Tan, Lily Yang, and Hengcheng Yu to make your brollies lit. The LEDs change colour based on the movement detected by the accelerometer!

# Requirements
- Onion Omega2 (should work with the 2+ but this has not been tested)
- Arduino Dock R2
- Time. Lots of it.

# Compiling
- Set up the virtual machine as described in the PDF labelled "SetupCrossCompilingEnvironmentV1.pdf". In the ```make menuconfig``` step, include the onioni2c library. This will take upwards of an hour of your time.
- Modify the target of the makefile under the compile folder to the name of the file that you wish to compile. By default, it will try to compile Synergy.cpp.
- Move the file you want to compile into the same folder as your makefile and xCompile.sh.
- Once the makefile, the target, and xCompile.sh are in the same folder, run xCompile.sh as described in the PDF labelled "CompilingCppCodeOmega.pdf". An executable file will be generated in the same folder.

# Running
- Move your executable to a USB stick, then mount it on your Omega:
	- Connect to your Omega's Wi-Fi network, then using PuTTY on Windows or your terminal on Mac/Linux: ```ssh root@192.168.3.1```. You will need to type in the password for the Omega2: default is onioneer.
	- In the terminal: ```mkdir /mnt/usb``` (first time only, making a directory to be the mount point for your USB stick) then ```mount /dev/sda1 /mnt/usb``` (actually mounting the USB)
- Connect to the Arduino Dock (I prefer wirelessly, but it shouldn't matter), and upload the ```ardu.ino``` sketch under Code/src/.
- Run Synergy from the command line: ```/mnt/usb/Synergy``` (assuming you followed the previous directions)
- If you wire up the Neopixels correctly (i.e. the way it's described under the "Final Project Report" appendix), then you should see your Neopixels light up when you move around the accelerometer!