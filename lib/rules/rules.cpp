#include <rules.h>

#include <random>

std::mt19937 rng;

const int RULES = 7;
const int PRESS_AND_HOLD_CASES = 3;
const float double_condition_chance = 0.8;

std::string get_word_string(Words word) {
  switch (word) {
    case Abort:
      return "ABORT";
    case Detonate:
      return "DETONATE";
    case Hold:
      return "HOLD";
    case Press:
      return "PRESS";
    case Wait:
      return "WAIT";
    default:
      return "";
  }
}

int rand_range(int min, int max) { return rng() % (max - min + 1) + min; }

Condition create_random_condition() {
  Condition condition;
  condition.type = (ConditionTypes)rand_range(0, 3);
  switch (condition.type) {
    case ButtonColor:
      condition.color = (Colors)rand_range(0, COLORS - 1);
      break;
    case ButtonWord:
      condition.word = (Words)rand_range(0, WORDS - 1);
      break;
    case PuzzleModulesSolved:
      condition.puzzle_modules_solved_even = rand_range(0, 1);
      break;
    case PuzzleModulesPending:
      condition.puzzle_modules_pending_even = rand_range(0, 1);
      break;
  }
  return condition;
}

bool is_redundant(Condition a, Condition b) { return a.type == b.type; }

Rule create_random_rule(bool last_rule = false) {
  Rule rule;
  if (!last_rule) {
    rule.conditions.push_back(create_random_condition());
    if (rng() % 100 < double_condition_chance * 100) {
      Condition condition;
      do {
        condition = create_random_condition();
      } while (is_redundant(rule.conditions[0], condition));
      rule.conditions.push_back(condition);
    }
  }
  return rule;
}

Rules generate_rules(int code) {
  rng = std::mt19937(code);
  Rules rules;
  for (int i = 0; i < RULES - 1; i++) rules.rules.push_back(create_random_rule());
  rules.rules.push_back(create_random_rule(true));
  int first_action = rand_range(0, 1);
  for (int i = 0; i < RULES; i++) {
    rules.rules[i].action = (ActionTypes)((first_action + i) % 2);
  }
  rules.default_press_and_hold_digit = rand_range(0, 9);
  while (rules.press_and_hold_digits.size() != PRESS_AND_HOLD_CASES) {
    PressAndHoldDigit press_and_hold_digit;
    press_and_hold_digit.color = (Colors)rand_range(0, COLORS - 1);
    press_and_hold_digit.digit = rand_range(0, 9);
    bool ok = true;
    for (int i = 0; i < rules.press_and_hold_digits.size(); i++)
      if (rules.press_and_hold_digits[i].color == press_and_hold_digit.color) ok = false;
    if (ok) rules.press_and_hold_digits.push_back(press_and_hold_digit);
  }
  return rules;
}