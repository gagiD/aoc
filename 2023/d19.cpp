#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <unistd.h>
#include <variant>
#include <vector>

using namespace std;

enum Variable { x, m, a, s };

enum Result { A, R };

using RuleName = string;

enum Operator { null, eq, gt, lt };

struct RuleOp {
  optional<Variable> var;
  Operator op;
  optional<int> value;
  variant<RuleName, Result> result;
};

struct Rule {
  RuleName name;
  vector<RuleOp> ops;
};

struct Variables {
  int x;
  int m;
  int a;
  int s;

  int getValue(Variable v) const {
    switch (v) {
    case Variable::x:
      return x;
    case Variable::m:
      return m;
    case Variable::a:
      return a;
    case Variable::s:
      return s;
    }
    return -1;
  }
};

Variable parseVariable(string s) {
  if (s == "x")
    return Variable::x;
  if (s == "m")
    return Variable::m;
  if (s == "a")
    return Variable::a;
  if (s == "s")
    return Variable::s;
  return Variable::x; // should never happen
}

variant<RuleName, Result> parseResult(string s) {
  if (s == "A") {
    return Result::A;
  } else if (s == "R") {
    return Result::R;
  } else {
    return RuleName(s);
  }
}

RuleOp parseRuleOp(string &ruleOpStr) {
  auto column = ruleOpStr.find(':');
  if (column != string::npos) {
    auto opLoc = find_if(ruleOpStr.begin(), ruleOpStr.end(),
                         [](char c) { return c == '>' || c == '<'; });
    auto opLocI = opLoc - ruleOpStr.begin();
    return {parseVariable(ruleOpStr.substr(0, opLocI)),
            *opLoc == '>' ? Operator::gt : Operator::lt,
            atoi(ruleOpStr.substr(opLocI + 1, column - opLocI).c_str()),
            parseResult(ruleOpStr.substr(column + 1))};
  } else {
    return {nullopt, null, nullopt, parseResult(ruleOpStr)};
  }
}

Rule parseRule(string line) {
  uint s = line.find('{');
  string name = line.substr(0, s);
  vector<RuleOp> ops;

  uint start = s + 1;
  for (int i = start; i < line.size(); i++) {
    if (line[i] == ',' || line[i] == '}') {
      string ruleOpStr = line.substr(start, i - start);
      ops.push_back(parseRuleOp(ruleOpStr));
      start = i + 1;
    }
  }

  return {name, ops};
}

Variables parseVariables(string line) {
  Variables vars;

  auto start = line.begin() + 1;
  for (auto it = start; it < line.end(); it++) {
    if (*it == ',' || *it == '}') {
      char name = *start;
      int value = atoi(string(start + 2, it).c_str());

      if (name == 'x')
        vars.x = value;
      if (name == 'm')
        vars.m = value;
      if (name == 'a')
        vars.a = value;
      if (name == 's')
        vars.s = value;

      start = it + 1;
    }
  }

  return vars;
}

struct Program {
  vector<Rule> rules;

  vector<Variables> vars;
};

size_t findRule(const vector<Rule> &rules, RuleName name) {
  for (size_t i = 0; i < rules.size(); i++) {
    if (rules[i].name == name) {
      return i;
    }
  }

  return rules.size();
}

bool evalOp(const RuleOp &op, const Variables &vars) {
  if (!op.var.has_value() || !op.value.has_value())
    return false;

  int value = op.value.value();
  int var = vars.getValue(op.var.value());

  if (op.op == gt) {
    return var > value;
  } else if (op.op == lt) {
    return var < value;
  }

  return false;
}

Result walkRules(const Variables &vars, const vector<Rule> &rules, size_t i) {
  if (i >= rules.size()) {
    return Result::R;
  }
  auto rule = rules[i];

  optional<variant<RuleName, Result>> curResult;
  for (const auto &op : rule.ops) {
    curResult = nullopt;
    if (op.op == null) {
      curResult = op.result;
    } else {
      if (evalOp(op, vars)) {
        curResult = op.result;
      }
    }

    if (!curResult.has_value()) {
      continue;
    }

    if (holds_alternative<Result>(curResult.value())) {
      return get<Result>(curResult.value());
    } else {
      return walkRules(vars, rules,
                       findRule(rules, get<RuleName>(curResult.value())));
    }
  }

  return Result::R;
}

ulong count(const Program &program) {
  ulong sum = 0;
  size_t startIndex = findRule(program.rules, "in");

  for (const auto &vars : program.vars) {
    if (walkRules(vars, program.rules, startIndex) == Result::A) {
      sum += vars.x + vars.m + vars.a + vars.s;
    }
  }

  return sum;
}

int main() {
  ifstream in("../../2023/examples/d19e1");
  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  Program program;

  string line;
  bool vars = false;
  while (getline(cin, line)) {
    if (line.empty()) {
      vars = true;
      continue;
    }
    if (vars) {
      program.vars.push_back(parseVariables(line));
    } else {
      program.rules.push_back(parseRule(line));
    }
  }

  cout << "Sum accepted: " << count(program) << endl;

  return 0;
}
