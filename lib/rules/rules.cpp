#include <random>
#include <rules.h>

std::mt19937 rng;

const int RULES = 7;
const int PRESS_AND_HOLD_CASES = 3;
const float doubleConditionChance = 0.8;

std::string getWordString(Words word) {
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

int randRange(int min, int max) { return rng() % (max - min + 1) + min; }

Condition createRandomCondition() {
  Condition condition;
  condition.type = (ConditionTypes)randRange(0, 3);
  switch (condition.type) {
  case ButtonColor:
    condition.color = (Colors)randRange(0, COLORS - 1);
    break;
  case ButtonWord:
    condition.word = (Words)randRange(0, WORDS - 1);
    break;
  case PuzzleModulesSolved:
    condition.puzzleModulesSolvedEven = randRange(0, 1);
    break;
  case PuzzleModulesPending:
    condition.puzzleModulesPendingEven = randRange(0, 1);
    break;
  }
  return condition;
}

bool isRedundant(Condition a, Condition b) { return a.type == b.type; }

Rule createRandomRule(bool lastRule = false) {
  Rule rule;
  if (!lastRule) {
    rule.conditions.push_back(createRandomCondition());
    if (rng() % 100 < doubleConditionChance * 100) {
      Condition condition;
      do {
        condition = createRandomCondition();
      } while (isRedundant(rule.conditions[0], condition));
      rule.conditions.push_back(condition);
    }
  }
  return rule;
}

Rules generateRules(int code) {
  rng = std::mt19937(code);
  Rules rules;
  for (int i = 0; i < RULES - 1; i++)
    rules.rules.push_back(createRandomRule());
  rules.rules.push_back(createRandomRule(true));
  int firstAction = randRange(0, 1);
  for (int i = 0; i < RULES; i++) {
    rules.rules[i].action = (ActionTypes)((firstAction + i) % 2);
  }
  rules.defaultPressAndHoldDigit = randRange(0, 9);
  while (rules.pressAndHoldDigits.size() != PRESS_AND_HOLD_CASES) {
    PressAndHoldDigit pressAndHoldDigit;
    pressAndHoldDigit.color = (Colors)randRange(0, COLORS - 1);
    pressAndHoldDigit.digit = randRange(0, 9);
    bool ok = true;
    for (int i = 0; i < rules.pressAndHoldDigits.size(); i++)
      if (rules.pressAndHoldDigits[i].color == pressAndHoldDigit.color)
        ok = false;
    if (ok)
      rules.pressAndHoldDigits.push_back(pressAndHoldDigit);
  }
  return rules;
}