#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN0 6
#define PIN1 7
#define PIN2 8
#define PIN3 9
#define PIN4 10
#define PIN5 11
#define PIN6 12
#define PIN7 13

#define NUMBER_OF_STRANDS 8

typedef struct Pixel {
  int r;
  int g;
  int b;
  int brightness;
} Pixel;

Adafruit_NeoPixel strip[NUMBER_OF_STRANDS] = {
  Adafruit_NeoPixel(60, PIN0, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN4, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN5, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN6, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(60, PIN7, NEO_GRB + NEO_KHZ800)
};

int transmission_number = 0;
Pixel pixel;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Wire.begin(0x08);
  Wire.onReceive(i2c_on_receive_handler);

  for (int i = 0; i < NUMBER_OF_STRANDS; i++) {
    strip[i].begin();
    strip[i].show();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void i2c_on_receive_handler(int bytes_num) {
//  digitalWrite(LED_BUILTIN, Wire.read());

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
      transmission_number++;
      for (uint16_t i = 0; i < strip[0].numPixels(); i++) {
        for (int j = 0; j < NUMBER_OF_STRANDS; j++) {
          strip[j].setPixelColor(i, pixel.r, pixel.g, pixel.b);
          strip[j].setBrightness(pixel.brightness);
          strip[j].show();
        }
      }
      transmission_number = 0;
      break;
  }
}
