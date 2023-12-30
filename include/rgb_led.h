#ifndef RGB_LED
#define RGB_LED

#include <Arduino.h>
#include <rules.h>

struct RGBLed {
  int redPin, greenPin, bluePin;
  RGBLed() {}
  RGBLed(int redPin, int greenPin, int bluePin)
      : redPin(redPin), greenPin(greenPin), bluePin(bluePin) {
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
  }
  void setColor(Colors color) {
    int red = color == Red || color == Yellow || color == Magenta;
    int green = color == Green || color == Yellow;
    int blue = color == Blue || color == Magenta;
    digitalWrite(redPin, red);
    digitalWrite(greenPin, green);
    digitalWrite(bluePin, blue);
  }
  void off() {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }
};

#endif // RGB_LED