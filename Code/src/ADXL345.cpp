#include <onion-i2c.h>
#include <iostream>

int main(const int argc, const char* const argv[]) {

	// Read from the accelerometer (MMA8452Q)
	int status;

	// i2c_write(devNum, devAddr, addr, val)
	// The ADXL345 has an address of 0x53 (83); writing 0x0A (10) to 0x2C (44, bandwidth rate register) sets the output data rate to 100Hz.
	status = i2c_write(0, 0x53, 0x2C, 0x0A);

	// Writing 0x08 (08) to 0x2D (45, power control register) disables auto-sleep.
	status = i2c_write(0, 0x53, 0x2D, 0x08);

	// Writing 0x08 (08) to 0x31 (49) sets the sensitivity to +/-2g, with self-test disabled and 4-wire interface.
	status = i2c_write(0, 0x53, 0x31, 0x08);

	std::cout << "Status after setup is " << status << std::endl;

	usleep(500); // Sleep for 0.5 seconds to make sure that changes have propagated

	// Allocate memory for buffer and read from accelerometer into the buffer
	// 0x00 contains all the data from the accelerometer.
	uint8_t *buffer0 = new uint8_t[2]; // Least significant bit
	uint8_t *buffer1 = new uint8_t[2]; // Most significant bit
	for (int i = 0; i > -1; i += 0) {

		std::cout << "Looping..." << std::endl;

		// Read for x-values
		status = i2c_read(0, 0x53, 0x32, buffer0, 1); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
		status = i2c_read(0, 0x53, 0x33, buffer1, 1); // i2c_read(devNum, devAddr, addr, buffer, numbytes)

		// Now that we have the data, convert it to 10-bits
		int x_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
		if (x_accel > 511) {
			x_accel -= 1024;
		}

		// Y-values
		status = i2c_read(0, 0x53, 0x34, buffer0, 1);
		status = i2c_read(0, 0x53, 0x35, buffer1, 1);

		int y_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
		if (y_accel > 511) {
			y_accel -= 1024;
		}

		// Z-values
		status = i2c_read(0, 0x53, 0x36, buffer0, 1);
		status = i2c_read(0, 0x53, 0x37, buffer1, 1);

		int z_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
		if (z_accel > 511) {
			z_accel -= 1024;
		}

		std::cout << "Acceleration in the X-Axis: " << x_accel << std::endl;
		std::cout << "Acceleration in the Y-Axis: " << y_accel << std::endl;
		std::cout << "Acceleration in the Z-Axis: " << z_accel << std::endl;

		usleep(100); // Sleep for 100 milliseconds to make it read nicer

	}

}