#include <manual.h>
#include <rules.h>

#include <algorithm>

std::string color_to_string(Colors color) {
  switch (color) {
    case Red:
      return "Red";
    case Green:
      return "Green";
    case Blue:
      return "Blue";
    case Yellow:
      return "Yellow";
    case Magenta:
      return "Pink";
  }
  return "";
}

std::string condition_string(Condition c) {
  std::string w = get_word_string(c.word);
  switch (c.type) {
    case ButtonColor:
      return "the button is " + color_to_string(c.color);
    case ButtonWord:
      w[0] = toupper(w[0]);
      std::transform(w.begin() + 1, w.end(), w.begin() + 1, tolower);
      return "the button says \"" + w + "\"";
    case PuzzleModulesSolved:
      return "the number of solved modules is " + std::string(c.puzzle_modules_solved_even ? "even" : "odd");
    case PuzzleModulesPending:
      return "the number of pending modules is " + std::string(c.puzzle_modules_pending_even ? "even" : "odd");
  }
  return "";
}

std::string action_string(ActionTypes action) {
  switch (action) {
    case PressAndRelease:
      return "press and immediately release the button";
    case PressAndHold:
      return "hold the button and refer to \"Releasing a Held Button\"";
  }
  return "";
}

manual::json generate_json_for_code(uint16_t code) {
  Rules rules = generate_rules(code);

  manual::json data = manual::init(MODULE_NAME, MODULE_NAME,
                                   "You might think that a button telling you to press it is pretty "
                                   "straightforward. That's the kind of thinking that gets people exploded.",
                                   APP_VERSION);

  data["rules"] = manual::json::array();
  for (Rule rule : rules.rules) {
    manual::json rule_data;
    rule_data["conditions"] = manual::json::array();
    rule_data["numberOfConditions"] = rule.conditions.size();
    rule_data["action"] = action_string(rule.action);
    for (int i = 0; i < rule.conditions.size(); i++) {
      rule_data["conditions"].push_back(condition_string(rule.conditions[i]));
    }
    data["rules"].push_back(rule_data);
  }

  data["press_and_hold_digits"] = manual::json::array();
  for (auto press_and_hold : rules.press_and_hold_digits) {
    manual::json press_and_hold_data;
    press_and_hold_data["color"] = color_to_string(press_and_hold.color);
    press_and_hold_data["digit"] = press_and_hold.digit;
    data["press_and_hold_digits"].push_back(press_and_hold_data);
  }
  data["default_press_and_hold_digit"] = rules.default_press_and_hold_digit;

  return data;
}

int main(int argc, char** argv) { manual::run(argc, argv, generate_json_for_code); }