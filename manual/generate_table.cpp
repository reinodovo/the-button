#include <algorithm>
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

void printCondition(Condition c) {
  std::string w = getWordString(c.word);
  switch (c.type) {
  case ButtonColor:
    printf("the button is %s", colorToString(c.color).c_str());
    break;
  case ButtonWord:
    w[0] = toupper(w[0]);
    std::transform(w.begin() + 1, w.end(), w.begin() + 1, tolower);
    printf("the button says \"%s\"", w.c_str());
    break;
  case PuzzleModulesSolved:
    printf("the number of solved modules is %s",
           c.puzzleModulesSolvedEven ? "even" : "odd");
    break;
  case PuzzleModulesPending:
    printf("the number of pending modules is %s",
           c.puzzleModulesPendingEven ? "even" : "odd");
    break;
  }
}

void printAction(ActionTypes action) {
  switch (action) {
  case PressAndRelease:
    printf("press and immediately release the button");
    break;
  case PressAndHold:
    printf("hold the button and refer to \"Releasing a Held Button\"");
    break;
  }
}

int main(int argc, char **argv) {
  freopen("./manual/rules.html", "w", stdout);
  int seed = atoi(argv[1]);
  printf("<p>Follow these rules in the order they are listed. Perform the "
         "first action that applies:</p>");
  Rules rules = generateRules(seed);
  printf("<ol>");
  for (Rule rule : rules.rules) {
    printf("<li>If ");
    for (int i = 0; i < rule.conditions.size(); i++) {
      printCondition(rule.conditions[i]);
      if (i + 1 != rule.conditions.size())
        printf(" and ");
    }
    if (rule.conditions.size() == 0)
      printf("none of the above apply");
    printf(", ");
    printAction(rule.action);
    printf(".</li>");
  }
  printf("</ol>");
  printf("<h3>Releasing a Held Button</h3>");
  printf("<p>If you start holding the button down, a colored strip will light "
         "up on the right side of the module. Based on its color, you must "
         "release the button at a specific point in time:</p>");
  printf("<ul>");
  for (auto pressAndHold : rules.pressAndHoldDigits) {
    printf("<li><em><span>%s strip:</span></em><span> release when the "
           "countdown timer has %s %d in any position</span></li>",
           colorToString(pressAndHold.color).c_str(),
           pressAndHold.digit == 8 ? "an" : "a", pressAndHold.digit);
  }
  printf(
      "<li><em><span>Any other color strip:</span></em><span> release when the "
      "countdown timer has %s %d in any position</span></li>",
      rules.defaultPressAndHoldDigit == 8 ? "an" : "a",
      rules.defaultPressAndHoldDigit);
  printf("</ul>");
}
