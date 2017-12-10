/****************************** Arduino ******************************/
/* A command line utility to set all the pixels to a specific colour
 * Useful for testing the Neopixels
 */

#include <onion-i2c.h>
#include <iostream>

#define NEOPIXEL_I2C_DEVICE_NUM (0)
#define DEV_ADDRESS 0x08

typedef struct Pixel {
	int brightness;
	int r;
	int g;
	int b;
} Pixel;

int main (const int argc, const char* const argv[]) {
	
	// Get arguments from the command line
	Pixel pixel;
	pixel.brightness = atoi(argv[1]);
	pixel.r = atoi(argv[2]);
	pixel.g = atoi(argv[3]);
	pixel.b = atoi(argv[4]);

	i2c_writeBytes(0, 0x08, pixel.brightness, 0x01, 0);
	i2c_writeBytes(0, 0x08, pixel.r, 0x01, 0);
	i2c_writeBytes(0, 0x08, pixel.g, 0x01, 0);
	i2c_writeBytes(0, 0x08, pixel.b, 0x01, 0);
}