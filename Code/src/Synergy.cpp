#include <onion-i2c.h>
#include <iostream>

#define NEOPIXEL_I2C_DEVICE_NUM 0
#define DEV_ADDRESS 0x08

// Abstract Accelerometer class
class Accelerometer {
public:
    // Pure virtual functions to get x, y, and z accelerations
    virtual int get_x_accel() = 0;
    virtual int get_y_accel() = 0;
    virtual int get_z_accel() = 0;
};

// Derived class ADXL345 is a subclass of the Accelerometer 
class ADXL345 : public Accelerometer {
public:
    // Concrete implementation of virtual functions
    int get_x_accel();
    int get_y_accel();
    int get_z_accel();

    // Constructor and destructors
    ADXL345();
    ~ADXL345();
private:
    int status, x_accel, y_accel, z_accel;
    uint8_t *buffer0, *buffer1; // buffer0 contains least significant bit, buffer1 contains most significant bit
};

// Setup the ADXL345
ADXL345::ADXL345() {
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
ADXL345::~ADXL345() {
    delete[] buffer0;
    delete[] buffer1;
}

// Get x-values
int ADXL345::get_x_accel() {
    status = i2c_read(0, 0x53, 0x32, buffer0, 1); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
    status = i2c_read(0, 0x53, 0x33, buffer1, 1); // 0x32 to 0x37 contain acceleration data

    // Now that we have the data, convert it to 10-bits with some magic numbers
    int x_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
    if (x_accel > 511) {
        x_accel -= 1024;
    }

    return x_accel;
}

// Get y-values
int ADXL345::get_y_accel() {
    status = i2c_read(0, 0x53, 0x34, buffer0, 1);
    status = i2c_read(0, 0x53, 0x35, buffer1, 1);

    int y_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
    if (y_accel > 511) {
        y_accel -= 1024;
    }

    return y_accel;
}

// Get z-values
int ADXL345::get_z_accel() {
    status = i2c_read(0, 0x53, 0x36, buffer0, 1);
    status = i2c_read(0, 0x53, 0x37, buffer1, 1);

    int z_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
    if (z_accel > 511) {
        z_accel -= 1024;
    }

    return z_accel;
}

/*  Struct declaration to hold data about how to light the Neopixels
    Typedef for convenience when initializing */
typedef struct Pixel {
    int brightness;
    int r;
    int g;
    int b;
} Pixel;

int main(const int argc, const char* const argv[]) {
    ADXL345 accelerometer; // Instantiate ADXL345 object (and set up the accelerometer)

    sleep(1); // Sleep for a second to make sure that changes have propagated

    // Infinite loop to continuously grab data from the accelerometer and then push the changes to the LEDs
    for (int i = 0; i > -1; i += 0) {
        std::cout << "Looping..." << std::endl;

        // Initialize a pixel with default brightness values and RGB values based on the X, Y, and Z readings from accelerometer
        Pixel pixel;
        pixel.brightness = 127;
        pixel.r = accelerometer.get_x_accel();
        pixel.g = accelerometer.get_y_accel();
        pixel.b = accelerometer.get_z_accel();

        // FOR TESTING PURPOSES DO NOT UNCOMMENT UNLESS YOU KNOW WHAT YOU'RE DOING
        // i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.brightness, 0x01, 0);
        // i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, 255, 0x01, 0);
        // i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, 0, 0x01, 0);
        // i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, 0, 0x01, 0);

        /*  Use I2C to communicate with the Arduino dock: change values with #defines at top of file
            int i2c_writeBytes (int devNum, int devAddr, int addr, int val, int numBytes)
            devNum is always 0 for Arduino dock
            devAddr for the ATMega is 0x08 by default unless you change it on the Omega itself
            addr to write to is the value that we want to transmit to the microcontroller, to be read with Wire library
            val to be written is a dummy value because we specify that we write 0 bytes of data */
        i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.brightness, 0x01, 0);
        i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.r, 0x01, 0);
        i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.g, 0x01, 0);
        i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.b, 0x01, 0);

        // Print out the data for verification
        std::cout << "Acceleration in the X-Axis: " << pixel.r << std::endl;
        std::cout << "Acceleration in the Y-Axis: " << pixel.g << std::endl;
        std::cout << "Acceleration in the Z-Axis: " << pixel.b << std::endl;

        // Sleep for a second to make it look nicer
        sleep(1);
    }

    return 0;
}