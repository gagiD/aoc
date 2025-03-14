#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

using namespace std;

unsigned int walk(const string &steps,
                  unordered_map<uint, pair<uint, uint>> &map) {
  unsigned int s = 1;
  uint index = 656565;
  while (true) {
    char step = steps[(s - 1) % steps.size()];
    if (step == 'L')
      index = map[index].first;
    else if (step == 'R')
      index = map[index].second;

    if (index == 909090)
      break;

    ++s;
  }

  return s;
}

unsigned long walk2Node(uint index, const string &steps,
                        unordered_map<uint, pair<uint, uint>> &map) {
  unsigned long s = 1;

  while (true) {
    char step = steps[(s - 1) % steps.size()];

    index = step == 'L' ? map[index].first : map[index].second;

    if (index % 100 == 'Z')
      break;

    ++s;
  }

  return s;
}

unsigned long walk2(const string &steps,
                    unordered_map<uint, pair<uint, uint>> &map) {

  vector<uint> indexes;
  for (const auto entry : map) {
    const uint key = entry.first;
    if (key % 100 == 'A') {
      indexes.push_back(key);
    }
  }

  vector<unsigned long> indexLens;
  indexLens.reserve(indexes.size());

  for (const auto index : indexes) {
    indexLens.push_back(walk2Node(index, steps, map));
  }

  unsigned long s = indexLens[0];
  for (int i = 1; i < indexLens.size(); ++i) {
    s = lcm(s, indexLens[i]);
  }

  return s;
}

inline unsigned int strToNum(const string &str) {
  uint num = 0;
  uint p = 1;

  for (int i = 2; i >= 0; --i) {
    num += str[i] * p;
    p *= 100;
  }

  return num;
}

int main() {
  ifstream in("../../2023/inputs/d8i1");
  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  string steps;
  cin >> steps;

  unordered_map<uint, pair<uint, uint>> map;
  string line;
  getline(cin, line);
  getline(cin, line);
  while (getline(cin, line)) {
    char c;
    string index, left, right;

    istringstream ss{line};
    ss >> index >> c >> c >> left >> right;

    left.pop_back();
    right.pop_back();

    map[strToNum(index)] = {strToNum(left), strToNum(right)};
  }

  cout << "Steps: " << walk(steps, map) << endl;

  cout << "Steps part 2: " << walk2(steps, map) << endl;

  return 0;
}
