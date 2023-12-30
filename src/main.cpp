#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <SPI.h>
#include <Wire.h>
#include <puzzle_module.h>
#include <rgb_led.h>
#include <rules.h>
#include <utils/button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int RED_PIN = 15;
const int GREEN_PIN = 5;

const int BUTTON_RED_PIN = 13, BUTTON_GREEN_PIN = 12, BUTTON_BLUE_PIN = 14;
RGBLed buttonLED(BUTTON_RED_PIN, BUTTON_GREEN_PIN, BUTTON_BLUE_PIN);
const int BUTTON_PIN = 4;
Button button(BUTTON_PIN);

const int STRIP_RED_PIN = 25, STRIP_GREEN_PIN = 33, STRIP_BLUE_PIN = 32;
RGBLed stripLED(STRIP_RED_PIN, STRIP_GREEN_PIN, STRIP_BLUE_PIN);

Words buttonWord;
Colors buttonColor, stripColor;
Rules rules;

ActionTypes correctAction(BombInfo bombInfo) {
  for (auto rule : rules.rules) {
    bool matches = true;
    for (auto condition : rule.conditions) {
      switch (condition.type) {
      case ButtonColor:
        matches &= condition.color == buttonColor;
        break;
      case ButtonWord:
        matches &= condition.word == buttonWord;
        break;
      }
    }
    if (!matches)
      continue;
    return rule.action;
  }
  return rules.rules.back().action;
}

int correctDigit() {
  int digit = rules.defaultPressAndHoldDigit;
  for (auto pressAndHold : rules.pressAndHoldDigits) {
    if (pressAndHold.color != stripColor)
      continue;
    digit = pressAndHold.digit;
    break;
  }
  return digit;
}

void trySolve(BombInfo bombInfo, ActionTypes action) {
  ActionTypes correctActionType = correctAction(bombInfo);
  if (correctActionType != action) {
    PuzzleModule::strike();
    return;
  }
  if (action == PressAndRelease) {
    PuzzleModule::solve();
    return;
  }
  int digit = correctDigit();
  for (int i = 0; i < 6; i++) {
    if (digit == bombInfo.time[i] - '0') {
      PuzzleModule::solve();
      return;
    }
  }
  PuzzleModule::strike();
}

void drawWord(const char *word) {
  int16_t x1, y1;
  uint16_t w, h;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setFont(&FreeMonoBold12pt7b);
  display.getTextBounds(word, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2 - y1);
  display.print(word);
  display.display();
}

void clear() {
  display.clearDisplay();
  display.display();
  buttonLED.off();
}

void onStateChange(ButtonState newState, ButtonState oldState) {
  if (newState == ButtonState::Held)
    stripLED.setColor(stripColor);
  else if (newState == ButtonState::Released) {
    stripLED.off();
    PuzzleModule::withBombInfo([oldState](BombInfo info) {
      trySolve(info,
               oldState == ButtonState::Held ? PressAndHold : PressAndRelease);
    });
  }
}

void start() {
  buttonWord = Words(esp_random() % WORDS);
  buttonColor = Colors(esp_random() % COLORS);
  stripColor = Colors(esp_random() % COLORS);
  drawWord(getWordString(buttonWord).c_str());
  buttonLED.setColor(buttonColor);
}

void restart() { clear(); }

void onManualCode(int code) { rules = generateRules(code); }

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    ESP.restart();

  PuzzleModule::onStart = start;
  PuzzleModule::onRestart = restart;
  PuzzleModule::onManualCode = onManualCode;

  if (!PuzzleModule::setup(
          PuzzleModule::StatusLight(RED_PIN, GREEN_PIN, false)))
    ESP.restart();

  button.onStateChange = onStateChange;

  clear();
}

void loop() {
  PuzzleModule::update();
  button.update();
}