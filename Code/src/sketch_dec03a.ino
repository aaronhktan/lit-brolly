#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Data pins for the eight Neopixel strands
#define PIN_NEO_0 6
#define PIN_NEO_1 7
#define PIN_NEO_2 8
#define PIN_NEO_3 9
#define PIN_NEO_4 10
#define PIN_NEO_5 11
#define PIN_NEO_6 12
#define PIN_NEO_7 13

#define NUMBER_OF_STRANDS 8

// Struct declaration, where member variables change how a pixel is lit
typedef struct Pixel {
  int r;
  int g;
  int b;
  int brightness;
} Pixel;

// Initialize an array of strands with each strand's data pin set
Adafruit_NeoPixel strip[NUMBER_OF_STRANDS] = {
  Adafruit_NeoPixel(60, PIN_NEO_0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN_NEO_7, NEO_GRB + NEO_KHZ800)
};

int transmission_number = 0;
Pixel pixel, previous_pixel;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Turn off that blue LED on the Arduino dock (because it is ugly)
  digitalWrite(LED_BUILTIN, LOW);

  Wire.begin(0x08); // Listen for input on 0x08, the device address of the Arduino dock
  Wire.onReceive(i2c_on_receive_handler);

  // Loop through all the strands to set them up
  for (int i = 0; i < NUMBER_OF_STRANDS; i++) {
    strip[i].begin(); // Prepares data pin for Neopixel output
    strip[i].show(); // Turns off all Neopixels at the beginning
  }
}

void loop() {
  // Nothing to see here friendos! :)
}

// Fires when data is passed from the Omega to the Arduino dock
void i2c_on_receive_handler(int bytes_num) {
  
  /* Depending on which transmission over I2C this is, we set different member variables of the Pixel
   * First, brightness, then R, G, B, and finally, we show the pixels.
   */
   
  switch(transmission_number) {
    case 0:
      pixel.brightness = Wire.read();
      transmission_number++;
      break;
    case 1:
      pixel.r = Wire.read();
      transmission_number++;
      break;
    case 2:
      pixel.g = Wire.read();
      transmission_number++;
      break;
    case 3:
      pixel.b = Wire.read();
//      color_transition(pixel.r, pixel.g, pixel.b, pixel.brightness);
      for (int i = 0; i < NUMBER_OF_STRANDS; i++) { // Loop through every strand and every pixel to set colour
        for (uint16_t j = 0; j < strip[i].numPixels(); j++) {
          strip[i].setPixelColor(j, pixel.r, pixel.g, pixel.b);
          strip[i].show();
        }
      }
      transmission_number = 0;
      break;
  }
}

//void color_transition(int r, int g, int b, int brightness) {
//  for (int i = 0; i <= 255; i++) {
//    if (previous_pixel.r < r && i > previous_pixel.r && pixel.r < r) {
//      pixel.r++;
//    } else if (previous_pixel.r > r && i > r && pixel.r > r) {
//      pixel.r--;
//    }
//
//    if (previous_pixel.g < g && i > previous_pixel.g && pixel.g < g) {
//      pixel.g++;
//    } else if (previous_pixel.g > g && i > g && pixel.g > g) {
//      pixel.g--;
//    }
//
//    if (previous_pixel.b < b && i > previous_pixel.b && pixel.b < b) {
//      pixel.b++;
//    } else if (previous_pixel.b > b && i > b && pixel.b > b) {
//      pixel.b--;
//    }
//
//    for (int i = 0; i < NUMBER_OF_STRANDS; i++) { // Loop through every strand and every pixel to set colour
//      for (uint16_t j = 0; j < strip[i].numPixels(); j++) {
//        strip[i].setPixelColor(j, pixel.r, pixel.g, pixel.b);
//        strip[i].show();
//      }
//    }
//    
//    delay(3);
//  }
//  
//  previous_pixel = {
//    .r = r,
//    .g = g,
//    .b = b,
//    .brightness = brightness
//  };
//}

