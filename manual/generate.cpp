#include <algorithm>
#include <manual.h>
#include <rules.h>

std::string colorToString(Colors color) {
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

std::string conditionString(Condition c) {
  std::string w = getWordString(c.word);
  switch (c.type) {
  case ButtonColor:
    return "the button is " + colorToString(c.color);
  case ButtonWord:
    w[0] = toupper(w[0]);
    std::transform(w.begin() + 1, w.end(), w.begin() + 1, tolower);
    return "the button says \"" + w + "\"";
  case PuzzleModulesSolved:
    return "the number of solved modules is " +
           std::string(c.puzzleModulesSolvedEven ? "even" : "odd");
  case PuzzleModulesPending:
    return "the number of pending modules is " +
           std::string(c.puzzleModulesPendingEven ? "even" : "odd");
  }
  return "";
}

std::string actionString(ActionTypes action) {
  switch (action) {
  case PressAndRelease:
    return "press and immediately release the button";
  case PressAndHold:
    return "hold the button and refer to \"Releasing a Held Button\"";
  }
  return "";
}

int main(int argc, char **argv) {
  int code = atoi(argv[1]);
  Rules rules = generateRules(code);

  manual::json data = manual::init(
      "The Button", "The Button",
      "You might think that a button telling you to press it is pretty "
      "straightforward. That's the kind of thinking that gets people exploded.",
      APP_VERSION);

  data["rules"] = manual::json::array();
  for (Rule rule : rules.rules) {
    manual::json ruleData;
    ruleData["conditions"] = manual::json::array();
    ruleData["numberOfConditions"] = rule.conditions.size();
    ruleData["action"] = actionString(rule.action);
    for (int i = 0; i < rule.conditions.size(); i++) {
      ruleData["conditions"].push_back(conditionString(rule.conditions[i]));
    }
    data["rules"].push_back(ruleData);
  }

  data["pressAndHoldDigits"] = manual::json::array();
  for (auto pressAndHold : rules.pressAndHoldDigits) {
    manual::json pressAndHoldData;
    pressAndHoldData["color"] = colorToString(pressAndHold.color);
    pressAndHoldData["digit"] = pressAndHold.digit;
    data["pressAndHoldDigits"].push_back(pressAndHoldData);
  }
  data["defaultPressAndHoldDigit"] = rules.defaultPressAndHoldDigit;

  manual::save(data, code);
}
