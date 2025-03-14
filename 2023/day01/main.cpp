// go:build ignore
//
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

const vector<string> digitStrs = {"one", "two",   "three", "four", "five",
                                  "six", "seven", "eight", "nine"};

short charToDigit(const char c) { return c - '0'; }

short getFirstDigit(const string &s) {
  for (auto it = s.begin(); it != s.end(); it++) {
    if (isdigit(*it)) {
      return charToDigit(*it);
    }
  }

  return 0;
}

short getLastDigit(const string &s) {
  for (auto it = s.rbegin(); it != s.rend(); it++) {
    if (isdigit(*it)) {
      return charToDigit(*it);
    }
  }

  return 0;
}

short getFirstDigitWithParse(const string &s) {
  for (auto it = s.begin(); it != s.end(); it++) {
    if (isdigit(*it)) {
      return charToDigit(*it);
    }

    for (int di = 0; const auto &ds : digitStrs) {
      if (it + ds.length() <= s.end() && ds == string(it, it + ds.length())) {
        return di + 1;
      }
      di++;
    }
  }

  return 0;
}

short getLastDigitWithParse(const string &s) {
  for (auto it = s.rbegin(); it != s.rend(); it++) {
    if (isdigit(*it)) {
      return charToDigit(*it);
    }

    for (int di = 0; const auto &ds : digitStrs) {
      if (it + ds.length() <= s.rend() && equal(ds.rbegin(), ds.rend(), it)) {
        return di + 1;
      }
      di++;
    }
  }

  return 0;
}

int main() {

  string line;
  line.reserve(100);
  long sumPart1 = 0;
  long sumPart2 = 0;

  while (getline(cin, line)) {

    sumPart1 += getFirstDigit(line) * 10 + getLastDigit(line);
    sumPart2 += getFirstDigitWithParse(line) * 10 + getLastDigitWithParse(line);
  }
  cout << "Sum part 1: " << sumPart1 << endl;
  cout << "Sum part 2: " << sumPart2 << endl;

  return 0;
}
