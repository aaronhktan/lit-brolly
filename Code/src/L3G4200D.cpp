#include <onion-i2c.h>
#include <iostream>

int main(const int argc, const char* const argv[]) {

	// Read from the gyro (L3G4200D)
	int status;

	// i2c_write(devNum, devAddr, addr, val, numBytes)
	// The L3G4200D has an address of 0x68 (104); writing 0x0F (15) to 0x20 (32) activates X, Y, and Z axis.
	status = i2c_write(0, 0x68, 0x20, 0x0F, 8);

	// Writing 0x30 (48) to 0x23 (35) sets mode to continuous update, data lsbat lower address, fsr 2000dps, self-test enabled, and 4-wire interface
	status = i2c_write(0, 0x168 0x23, 0x30, 8);

	usleep(500); // Sleep for 0.5 seconds to make sure that changes have propagated


	// Main loop
	while (true) {

		usleep(100); // Sleep for 0.1 seconds to delay

		// Allocate memory for buffer and read from gyro into the buffers
		// 0x28 (40) and 0x29 (41) contain all the x-axis data from the gyro, LSB first.
		uint8_t *buffer0 = new uint8_t[2];
		uint8_t *buffer1 = new uint8_t[2];
		status = i2c_read(0, 0x68, 0x28, buffer0, 2); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
		status = i2c_read(0, 0x68, 0x29, buffer1, 2);

		// Now that we have the data, convert it
		int x_gyro = (buffer1[0] * 256 + buffer0[0]);
		if (x_gyro > 32767) {
			x_gyro -= 65536;
		}

		// Now do for y-values from 0x2A and 0x2B
		status = i2c_read(0, 0x68, 0x2A, buffer0, 2);
		status = i2c_read(0, 0x68, 0x2B, buffer1, 2);

		int y_gyro = (buffer0[2] * 256 + buffer1[0]);
		if (y_gyro > 32767) {
			y_gyro -= 65536;
		}

		// Now do for z-values from 0x2C and 0x2D
		status = i2c_read(0, 0x68, 0x2C, buffer0, 2);
		status = i2c_read(0, 0x68, 0x2D, buffer1, 2);

		int z_gyro = (buffer0[1] * 256 + buffer1[0]);
		if (z_gyro > 32767) {
			z_gyro -= 65536;
		}

		cout << "Tilt in the X-Axis: " << x_gyro << endl;
		cout << "Tilt in the Y-Axis: " << y_gyro << endl;
		cout << "Tilt in the Z-Axis: " << z_gyro << endl;
	}
}