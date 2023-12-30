#ifndef RULES_H
#define RULES_H

#include <string>
#include <vector>

const int COLORS = 5;
enum Colors { Red, Green, Blue, Yellow, Magenta };
const int WORDS = 5;
enum Words { Abort, Detonate, Hold, Press, Wait };

enum ConditionTypes {
  ButtonColor,
  ButtonWord,
  PuzzleModulesSolved,
  PuzzleModulesPending,
};

struct Condition {
  ConditionTypes type;
  Colors color;
  Words word;
  bool puzzleModulesSolvedEven;
  bool puzzleModulesPendingEven;
};

enum ActionTypes {
  PressAndRelease,
  PressAndHold,
};

struct PressAndHoldDigit {
  Colors color;
  int digit;
};

struct Rule {
  std::vector<Condition> conditions;
  ActionTypes action;
};

struct Rules {
  std::vector<Rule> rules;
  std::vector<PressAndHoldDigit> pressAndHoldDigits;
  int defaultPressAndHoldDigit;
};

std::string getWordString(Words word);
Rules generateRules(int code);

#endif // RULES_H