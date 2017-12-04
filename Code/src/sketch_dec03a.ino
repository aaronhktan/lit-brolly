#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

typedef struct Pixel {
  int r;
  int g;
  int b;
  int brightness;
} Pixel;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
int transmission_number = 0;
Pixel pixel;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Wire.begin(0x08);
  Wire.onReceive(i2c_on_receive_handler);

  strip.begin();
  strip.show();
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
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, pixel.r, pixel.g, pixel.b);
        strip.setBrightness(pixel.brightness);
        strip.show();
      }
      transmission_number = 0;
      break;
  }
}

//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}
