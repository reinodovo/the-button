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
  bool puzzle_modules_solved_even;
  bool puzzle_modules_pending_even;
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
  std::vector<PressAndHoldDigit> press_and_hold_digits;
  int default_press_and_hold_digit;
};

std::string get_word_string(Words word);
Rules generate_rules(int code);

#endif  // RULES_H