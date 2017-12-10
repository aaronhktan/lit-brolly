#include <onion-i2c.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#define NEOPIXEL_I2C_DEVICE_NUM 0
#define DEV_ADDRESS 0x08

// Abstract Sensor class
class Sensor {
public:
    // Pure virtual functions to get x, y, and z accelerations
    virtual int get_x_value() = 0;
    virtual int get_y_value() = 0;
    virtual int get_z_value() = 0;
};

/****************************** ADXL345 ******************************/
/* The ADXL345 is a small ultralow power 3-axis accelerometer.
 * There are many others like it, but this one is mine.
 */

// Derived class ADXL345 is a subclass of the Sensor
class ADXL345 : public Sensor {
public:
    // Concrete implementation of virtual functions
    int get_x_value();
    int get_y_value();
    int get_z_value();

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
int ADXL345::get_x_value() {
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
int ADXL345::get_y_value() {
    status = i2c_read(0, 0x53, 0x34, buffer0, 1);
    status = i2c_read(0, 0x53, 0x35, buffer1, 1);

    int y_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
    if (y_accel > 511) {
        y_accel -= 1024;
    }

    return y_accel;
}

// Get z-values
int ADXL345::get_z_value() {
    status = i2c_read(0, 0x53, 0x36, buffer0, 1);
    status = i2c_read(0, 0x53, 0x37, buffer1, 1);

    int z_accel = (buffer1[0] & 0x03 * 256) + buffer0[0];
    if (z_accel > 511) {
        z_accel -= 1024;
    }

    return z_accel;
}

/****************************** MMA8452Q ******************************/
/* The MMA8425Q is another 3-axis accelerometer.
 * Ours broke. Also, SPI doesn't seem to work on this thing. Very sad. :(
 */


// Derived class MMA8425Q is a subclass of the Sensor
class MMA8452Q : public Sensor {
public:
    // Concrete implementation of virtual functions
    int get_x_value();
    int get_y_value();
    int get_z_value();

    // Put accelerometer data into the buffer
    void fetch_new_accel_data();

    // Constructor and destructors
    MMA8452Q();
    ~MMA8452Q();
private:
    int status, x_accel, y_accel, z_accel;
    uint8_t *buffer; // buffer contains all accelerometer data
};

// You get an MMA8452Q! You get an MMA8452Q! Everybody gets an MMA8452Q!
MMA8452Q::MMA8452Q() {
    // Initialize some variables
    status = 0;
    buffer = new uint8_t[7];

    // i2c_write(devNum, devAddr, addr, val, numBytes)
    // The MMA8452Q has an address of 0x1C (28); writing 0x01 to 0x2A activates the accelerometer.
    status = i2c_write(0, 0x1C, 0x2A, 0x01);

    // Writing 0x00 to 0x0E sets the sensitivity to +/-2g. This could be changed to 01 or 10 for 4 and 8 respectively.
    status = i2c_write(0, 0x1C, 0x0E, 0x00);

    std::cout << "Status after setup is " << status << std::endl;
}

// Clean up, clean up, everybody clean up
MMA8452Q::~MMA8452Q() {
    delete[] buffer;
}

// Read accelerometer values over I2C and put them into the buffer
void MMA8452Q::fetch_new_accel_data() {
    status = i2c_read(0, 0x1C, 0x00, buffer, 7); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
}

// Get x-values
int MMA8452Q::get_x_value() {
    // Now that we have the data, convert it (see datasheet AN4076 for data manipulation details)
    int x_accel = (buffer[1] * 256 + buffer[2]) / 16;
    if (x_accel > 2047) {
        x_accel -= 4096;
    }

    return x_accel;
}

// Get y-values
int MMA8452Q::get_y_value() {
    int y_accel = (buffer[3] * 256 + buffer[4]) / 16;
    if (y_accel > 2047) {
        y_accel -= 4096;
    }

    return y_accel;
}

// Get z-values
int MMA8452Q::get_z_value() {
    int z_accel = (buffer[5] * 256 + buffer[6]) / 16;
    if (z_accel > 2047) {
        z_accel -= 4096;
    }

    return z_accel;
}

/****************************** L3G4200D ******************************/
/* The L3G4200D is a 3-axis gyroscope / angular rate sensor.
 * Ours got into a sad soldering accident and perished bravely on the front lines.
 */

// Derived class L3G4200D is a subclass of the Sensor
class L3G4200D : public Sensor {
public:
    // Concrete implementation of virtual functions
    int get_x_value();
    int get_y_value();
    int get_z_value();

    // Constructor and destructors
    L3G4200D();
    ~L3G4200D();
private:
    int status, x_accel, y_accel, z_accel;
    uint8_t *buffer0, *buffer1; // buffer0 contains LSB, buffer1 contains MSB
};

// Set up the gyroscope
L3G4200D::L3G4200D() {
    // Initialize all the variables
    status = 0;
    buffer0 = new uint8_t[2];
    buffer1 = new uint8_t[2];

    // i2c_write(devNum, devAddr, addr, val, numBytes)
    // The L3G4200D has an address of 0x68 (104); writing 0x0F (15) to 0x20 (32) activates X, Y, and Z axis.
    status = i2c_write(0, 0x68, 0x20, 0x0F);

    // Writing 0x30 (48) to 0x23 (35) sets mode to continuous update, data lsbat lower address, fsr 2000dps, self-test enabled, and 4-wire interface
    status = i2c_write(0, 0x168, 0x23, 0x30);
}

// Clean up after ourselves
L3G4200D::~L3G4200D() {
    delete[] buffer0;
    delete[] buffer1;
}

// 0x28 (40) and 0x29 (41) contain all the x-axis data from the gyro, LSB first.
int L3G4200D::get_x_value() {
    status = i2c_read(0, 0x68, 0x28, buffer0, 2); // i2c_read(devNum, devAddr, addr, buffer, numbytes)
    status = i2c_read(0, 0x68, 0x29, buffer1, 2);

    // Now that we have the data, convert it
    int x_gyro = (buffer1[0] * 256 + buffer0[0]);
    if (x_gyro > 32767) {
        x_gyro -= 65536;
    }

    return x_gyro;
}

// Now do for y-values from 0x2A and 0x2B
int L3G4200D::get_y_value() {
    status = i2c_read(0, 0x68, 0x2A, buffer0, 2);
    status = i2c_read(0, 0x68, 0x2B, buffer1, 2);

    int y_gyro = (buffer0[2] * 256 + buffer1[0]);
    if (y_gyro > 32767) {
        y_gyro -= 65536;
    }

    return y_gyro;
}

// Now do for z-values from 0x2C and 0x2D
int L3G4200D::get_z_value() {
    status = i2c_read(0, 0x68, 0x2C, buffer0, 2);
    status = i2c_read(0, 0x68, 0x2D, buffer1, 2);

    int z_gyro = (buffer0[1] * 256 + buffer1[0]);
    if (z_gyro > 32767) {
        z_gyro -= 65536;
    }

    return z_gyro;
}

/****************************** Pixel ******************************/
/*  Struct declaration to hold data about how to light the Neopixels
    Typedef for convenience when initializing */
typedef struct Pixel {
    int brightness;
    int r;
    int g;
    int b;
} Pixel;

/****************************** Logging ******************************/
// We are the lumberjacks

// Histogram container struct
typedef struct Histogram {
    int accel[5];
    int histx[5];
    int histy[5];
    int histz[5];
} Histogram;

// Function to log data in a file
void file_log(Pixel pixel, int status) {
    std::ofstream file; // Make the text file to log everything in
    file.open("log.txt"); // Open the file

    if (file.is_open()) {
        std::time_t time = std::time(NULL); // Current time and date using ctime

        char* dt = std::ctime(&time); // Convert to string form

        // Put it in the text file
        if (pixel.r > 255 || pixel.g > 255 || pixel.b > 255 || pixel.r < 0 || pixel.g < 0 || pixel.b < 0) {
            file << dt << '\t' << "[ERROR]: The accelerometer values are incorrect" << std::endl;
        } else {
            file << dt << '\t' << "[DEBUG]: Accelerometer values are acceptable" << std::endl;

        }

        if (status != 0) {
            file << dt << '\t' << "[ERROR]: Something went wrong while sending data over I2C" << std::endl;
        } else {
            file << dt << '\t' << "[DEBUG]: No errors while sending data over I2C" << std::endl;
        }

        file << dt << '\t' << "[INFO]: Intensity of the pixels: " << pixel.brightness << std::endl;
        file << dt << '\t' << "[INFO]: Acceleration in the X-Axis: " << pixel.r << std::endl;
        file << dt << '\t' << "[INFO]: Acceleration in the Y-Axis: " << pixel.g << std::endl;
        file << dt << '\t' << "[INFO]: Acceleration in the Z-Axis: " << pixel.b << std::endl;
    }

    file.close();
}

// Function to initialize histogram
void initialize_histogram(Histogram &histogram) {
    for(int x = 0; x < 5; x++) { // Initialize values to 0
        histogram.accel[x] = 0;
        histogram.histx[x] = 0;
        histogram.histy[x] = 0;
        histogram.histz[x] = 0;
    }
}

// Function to put values into their buckets
void input_histogram(Histogram &histogram, Pixel pixel) {
    double accel = sqrt(pow(pixel.r , 2) + pow(pixel.b , 2) + pow(pixel.g , 2)); // Calculate total acceleration

    if (accel >= 0 && accel < 50) { // Drop some accelerations into those bins
        histogram.accel[0]++;
    } else if (accel >= 50 && accel < 100) {
        histogram. accel[1]++;
    } else if (accel >= 100 && accel < 150) {
        histogram.accel[2]++;
    } else if (accel >= 150 && accel < 200) {
        histogram.accel[3]++;
    } else if (accel <= 200) {
        histogram.accel[4]++;
    }

    if (pixel.r >= 0 && pixel.r < 50) { // Drop some Reds into those bins
        histogram.histx[0]++;
    } else if (pixel.r >= 50 && pixel.r < 100) {
        histogram.histx[1]++;
    } else if (pixel.r >= 100 && pixel.r < 150) {
        histogram.histx[2]++;
    } else if (pixel.r >= 150 && pixel.r < 200) {
        histogram.histx[3]++;
    } else if (pixel.r >= 200 && pixel.r <= 255) {
        histogram.histx[4]++;
    }

    if (pixel.b >= 0 && pixel.b < 50) { // Drop some Blues into those bins
        histogram.histy[0]++;
    } else if (pixel.b >= 50 && pixel.b < 100) {
        histogram.histy[1]++;
    } else if (pixel.b >= 100 && pixel.b < 150) {
        histogram.histy[2]++;
    } else if (pixel.b >= 150 && pixel.b < 200) {
        histogram.histy[3]++;
    } else if (pixel.b >= 200 && pixel.b <= 255) {
        histogram.histy[4]++;
    }

    if (pixel.g >= 0 && pixel.g < 50) { // Drop some Greens into those bins
        histogram.histz[0]++;
    } else if (pixel.g >= 50 && pixel.g < 100) {
        histogram.histz[1]++;
    } else if (pixel.g >= 100 && pixel.g < 150) {
        histogram.histz[2]++;
    } else if (pixel.g >= 150 && pixel.g < 200) {
        histogram.histz[3]++;
    } else if (pixel.g >= 200 && pixel.g <= 255) {
        histogram.histz[4]++;
    }
}

// Function to write histogram to a file
void write_histogram(Histogram histogram) {
    std::ofstream file;
    file.open("histogram.txt");

    if(file.is_open()){
        std::time_t time = std::time(NULL); // Current time and date using ctime
        char* dt = std::ctime(&time); // Convert to string form
        file << "Generated " << dt << std::endl;

        file << "Value" << '\t' << "X-acc: " << '\t' << "Y-acc" << '\t' << "Z-acc" << '\t' << "Total Acceleration" << std::endl;
        file << "0-49" << '\t' << histogram.histx[0] << '\t' << histogram.histy[0] << '\t' << histogram.histz[0] << '\t' << histogram.accel[0] << std::endl;
        file << "50-99" << '\t' << histogram.histx[1] << '\t' << histogram.histy[1] << '\t' << histogram.histz[1] << '\t' << histogram.accel[1] << std::endl;
        file << "100-149" << '\t' << histogram.histx[2] << '\t' << histogram.histy[2] << '\t' << histogram.histz[2] << '\t' << histogram.accel[2] << std::endl;
        file << "150-199" << '\t' << histogram.histx[3] << '\t' << histogram.histy[3] << '\t' << histogram.histz[3] << '\t' << histogram.accel[3] << std::endl;
        file << "200-255" << '\t' << histogram.histx[4] << '\t' << histogram.histy[4] << '\t' << histogram.histz[4] << '\t' << histogram.accel[4] << std::endl;
    }

    file.close();
}

/****************************** Main Logic ******************************/
int main(const int argc, const char* const argv[]) {

    Histogram histogram; // Set up the histogram
    initialize_histogram(histogram);

    int status = 0; // Keeps track of whether things worked

    ADXL345 accelerometer; // Instantiate ADXL345 object (and set up the accelerometer)
    
    // Our MMA8425Q broke so we are no longer using this code
    // MMA8452Q accelerometer; // Instantiate MMA8425Q object (and set up the accelerometer)

    // Our L3G4200D got into an accident involving solder so we are no longer using this code
    // L3G4200D gyro; // Instantiate L3G4200D object (and set up the gyroscrope)

    sleep(1); // Sleep for a second to make sure that changes have propagated

    // Infinite loop to continuously grab data from the accelerometer and then push the changes to the LEDs
    for (int i = 0; i > -1; i += 0) {
        std::cout << "Looping..." << std::endl;

        // Initialize a pixel with default brightness values and RGB values based on the X, Y, and Z readings from accelerometer
        Pixel pixel;
        // pixel.brightness = gyro.get_x_value; // The gyro can no longer be used.
        pixel.brightness = 127;
        pixel.r = accelerometer.get_x_value();
        pixel.g = accelerometer.get_y_value();
        pixel.b = accelerometer.get_z_value();

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
        status = i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.brightness, 0x01, 0);
        status = i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.r, 0x01, 0);
        status = i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.g, 0x01, 0);
        status = i2c_writeBytes(NEOPIXEL_I2C_DEVICE_NUM, DEV_ADDRESS, pixel.b, 0x01, 0);

        // Print out the data for verification in terminal
        std::cout << "Acceleration in the X-Axis: " << pixel.r << std::endl;
        std::cout << "Acceleration in the Y-Axis: " << pixel.g << std::endl;
        std::cout << "Acceleration in the Z-Axis: " << pixel.b << std::endl;

        // Log ALL THE THINGS
        file_log(pixel, status);
        input_histogram(histogram, pixel); // Put new values into histogram
        if (!(i % 10)) { // Runs only once every ten seconds to minimize load on Omega
            write_histogram(histogram);
        }

        // Sleep for a second to make it look nicer
        sleep(1);
    }

    return 0;
}