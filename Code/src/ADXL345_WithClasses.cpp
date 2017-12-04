#include <onion-i2c.h>
#include <iostream>

class Accelerometer {
	public:
		int get_x_accel();
		int get_y_accel();
		int get_z_accel();
		Accelerometer();
		~Accelerometer();
	private:
		int status, x_accel, y_accel, z_accel;
		uint8_t *buffer0, *buffer1; // buffer0 contains least significant bit, buffer1 contains most significant bit
};

// Setup the accelerometer
Accelerometer::Accelerometer() {
	// Initialize some variables
	status = 0;
	buffer0 = new uint8_t[2];
	buffer1 = new uint8_t[2];

	// i2c_write(devNum, devAddr, addr, val)
	// The ADXL345 has an address of 0x53 (83); writing 0x0A (10) to 0x2C (44, bandwidth rate register) sets the output data rate to 100Hz.
	status = i2c_write(0, 0x53, 0x2C, 0x0A);

	// Writing 0x08 (08) to 0x2D (45, power control register) disables auto-sleep.
	status = i2c_write(0, 0x53, 0x2D, 0x08);

	// Writing 0x08 (08) to 0x31 (49) sets the sensitivity to +/-2g, with self-test disabled and 4-wire interface.
	status = i2c_write(0, 0x53, 0x31, 0x08);

	std::cout << "Status after setup is " << status << std::endl;
}

// Clean up after ourselves!
Accelerometer::~Accelerometer() {
	delete[] buffer0;
	delete[] buffer1;
}

// Get x-values
int Accelerometer::get_x_accel() {
	status = i2c_read(0, 0x53, 0x32, buffer0, 1); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
	status = i2c_read(0, 0x53, 0x33, buffer1, 1); // 0x32 to 0x37 contain acceleration data

	// Now that we have the data, convert it to 10-bits
	int x_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
	if (x_accel > 511) {
		x_accel -= 1024;
	}

	return x_accel;
}

// Get y-values
int Accelerometer::get_y_accel() {
	status = i2c_read(0, 0x53, 0x34, buffer0, 1);
	status = i2c_read(0, 0x53, 0x35, buffer1, 1);

	int y_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
	if (y_accel > 511) {
		y_accel -= 1024;
	}

	return y_accel;
}

// Get z-values
int Accelerometer::get_z_accel() {
	status = i2c_read(0, 0x53, 0x36, buffer0, 1);
	status = i2c_read(0, 0x53, 0x37, buffer1, 1);

	int z_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
	if (z_accel > 511) {
		z_accel -= 1024;
	}

	return z_accel;
}

int main(const int argc, const char* const argv[]) {

	Accelerometer accelerometer; // Instantiate Accelerometer object (and set up the accelerometer)

	usleep(500); // Sleep for 0.5 seconds to make sure that changes have propagated

	// Allocate memory for buffer and read from accelerometer into the buffer
	// 0x00 contains all the data from the accelerometer.
	for (int i = 0; i > -1; i += 0) {

		std::cout << "Looping..." << std::endl;

		// Print out the data
		std::cout << "Acceleration in the X-Axis: " << accelerometer.get_x_accel() << std::endl;
		std::cout << "Acceleration in the Y-Axis: " << accelerometer.get_y_accel() << std::endl;
		std::cout << "Acceleration in the Z-Axis: " << accelerometer.get_z_accel() << std::endl;

		usleep(500); // Sleep for 100 milliseconds to make it read nicer

	}

	return 0;
}