#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;

const vector<string> digitStrs = {"one", "two",   "three", "four", "five",
                                  "six", "seven", "eight", "nine"};

short strToDigit(const string &s) {
  if (s.length() == 1 && s[0] >= '1' && s[0] <= '9')
    return s[0] - '0';

  if (s == "one")
    return 1;
  if (s == "two")
    return 2;
  if (s == "three")
    return 3;
  if (s == "four")
    return 4;
  if (s == "five")
    return 5;
  if (s == "six")
    return 6;
  if (s == "seven")
    return 7;
  if (s == "eight")
    return 8;
  if (s == "nine")
    return 9;

  return 0;
}

short charToDigit(const char c) { return c - '0'; }

int main() {

  string line;
  line.reserve(100);
  long sum = 0;
  while (getline(cin, line)) {
    short leftDigit = 0, rightDigit = 0;

    for (auto it = line.begin(); it != line.end(); it++) {
      if (isdigit(*it)) {
        leftDigit = charToDigit(*it);
        break;
      }

      for (const auto &s : digitStrs) {
        if (it + s.length() <= line.end() && s == string(it, it + s.length())) {
          leftDigit = strToDigit(s);
          goto afterFirst;
        }
      }
    }
  afterFirst:

    for (auto it = line.rbegin(); it != line.rend(); it++) {

      if (isdigit(*it)) {
        rightDigit = charToDigit(*it);
        break;
      }

      for (const auto &s : digitStrs) {
        if (it + s.length() <= line.rend() && equal(s.rbegin(), s.rend(), it)) {
          rightDigit = strToDigit(s);
          goto afterSecond;
        }
      }
    }

  afterSecond:

    sum += leftDigit * 10 + rightDigit;
  }
  cout << sum << endl;

  return 0;
}
