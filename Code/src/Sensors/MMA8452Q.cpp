#include <onion-i2c.h>
#include <iostream>

int main(const int argc, const char* const argv[]) {

	// Read from the accelerometer (MMA8452Q)
	int status;

	// i2c_write(devNum, devAddr, addr, val, numBytes)
	// The MMA8452Q has an address of 0x1C (28); writing 0x01 to 0x2A activates the accelerometer.
	status = i2c_write(0, 0x1C, 0x2A, 0x01);

	// Writing 0x00 to 0x0E sets the sensitivity to +/-2g. This could be changed to 01 or 10 for 4 and 8 respectively.
	status = i2c_write(0, 0x1C, 0x0E, 0x00);

	sleep(1); // Sleep for 1 second to make sure that changes have propagated

	while (true) {
		// Allocate memory for buffer and read from accelerometer into the buffer
		// 0x00 contains all the data from the accelerometer.
		uint8_t *buffer = new uint8_t[7];
		status = i2c_read(0, 0x1C, 0x00, buffer, 7); // i2c_read(devNum, devAddr, addr, buffer, numbytes)

		// Now that we have the data, convert it (see datasheet AN4076 for data manipulation details)
		int x_accel = (buffer[1] * 256 + buffer[2]) / 16;
		if (x_accel > 2047) {
			x_accel -= 4096;
		}

		int y_accel = (buffer[3] * 256 + buffer[4]) / 16;
		if (y_accel > 2047) {
			y_accel -= 4096;
		}

		int z_accel = (buffer[5] * 256 + buffer[6]) / 16;
		if (z_accel > 2047) {
			z_accel -= 4096;
		}

		cout << "Acceleration in the X-Axis: " << x_accel << endl;
		cout << "Acceleration in the Y-Axis: " << y_accel << endl;
		cout << "Acceleration in the Z-Axis: " << z_accel << endl;

		delete[] buffer;
	}
}