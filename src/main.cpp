#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <SPI.h>
#include <Wire.h>
#include <modules/puzzle_module.h>
#include <rgb_led.h>
#include <rules.h>
#include <utils/button.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int RED_PIN = 15, GREEN_PIN = 5;
PuzzleModule module(StatusLight(RED_PIN, GREEN_PIN, StatusLightType::CommonCathode));

const int BUTTON_RED_PIN = 13, BUTTON_GREEN_PIN = 12, BUTTON_BLUE_PIN = 14;
RGBLed button_led(BUTTON_RED_PIN, BUTTON_GREEN_PIN, BUTTON_BLUE_PIN);
const int BUTTON_PIN = 4;
Button button(BUTTON_PIN);

const int STRIP_RED_PIN = 25, STRIP_GREEN_PIN = 33, STRIP_BLUE_PIN = 32;
RGBLed strip_led(STRIP_RED_PIN, STRIP_GREEN_PIN, STRIP_BLUE_PIN);

Words button_word;
Colors button_color, strip_color;
Rules rules;

ActionTypes correct_action(BombInfo bomb_info) {
  for (auto rule : rules.rules) {
    bool matches = true;
    for (auto condition : rule.conditions) {
      switch (condition.type) {
        case ButtonColor:
          matches &= condition.color == button_color;
          break;
        case ButtonWord:
          matches &= condition.word == button_word;
          break;
        case PuzzleModulesSolved:
          matches &= bomb_info.solved_puzzle_modules % 2 == !condition.puzzle_modules_solved_even;
          break;
        case PuzzleModulesPending:
          matches &= (bomb_info.total_puzzle_modules - bomb_info.solved_puzzle_modules) % 2 ==
                     !condition.puzzle_modules_pending_even;
          break;
      }
    }
    if (!matches) continue;
    return rule.action;
  }
  return rules.rules.back().action;
}

int correct_digit() {
  int digit = rules.default_press_and_hold_digit;
  for (auto press_and_hold : rules.press_and_hold_digits) {
    if (press_and_hold.color != strip_color) continue;
    digit = press_and_hold.digit;
    break;
  }
  return digit;
}

void try_solve(BombInfo bomb_info, ActionTypes action) {
  ActionTypes correct_action_type = correct_action(bomb_info);
  if (correct_action_type != action) {
    module.strike();
    return;
  }
  if (action == PressAndRelease) {
    module.solve();
    return;
  }
  int digit = correct_digit();
  for (int i = 0; i < 6; i++) {
    if (digit == bomb_info.time[i] - '0') {
      module.solve();
      return;
    }
  }
  module.strike();
}

void draw_word(const char* word) {
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
  button_led.off();
}

void on_state_change(ButtonState newState, ButtonState oldState) {
  if (newState == ButtonState::Held)
    strip_led.set_color(strip_color);
  else if (newState == ButtonState::Released) {
    strip_led.off();
    module.with_bomb_info([oldState](BombInfo info) {
      try_solve(info, oldState == ButtonState::Held ? PressAndHold : PressAndRelease);
    });
  }
}

void start() {
  button_word = Words(esp_random() % WORDS);
  button_color = Colors(esp_random() % COLORS);
  strip_color = Colors(esp_random() % COLORS);
  draw_word(get_word_string(button_word).c_str());
  button_led.set_color(button_color);
}

void restart() { clear(); }

void on_manual_code(int code) { rules = generate_rules(code); }

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) ESP.restart();

  module.on_start(start);
  module.on_reset(restart);
  module.on_manual_code(on_manual_code);

  if (!module.setup()) ESP.restart();

  button.on_state_change(on_state_change);

  clear();
}

void loop() {
  module.update();
  if (module.get_state() != PuzzleModuleState::Started) return;
  button.update();
}