#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;

bool isBetween(int value, int n1, int n2) {
  return value >= min(n1, n2) && value <= max(n1, n2);
}

uint countEmpty(const vector<int> empty, int i1, int i2) {
  uint count = 0;
  for (const int e : empty) {
    if (isBetween(e, i1, i2)) {
      ++count;
    }
  }

  return count;
}

ulong calcPaths(const vector<pair<int, int>> &gs, const vector<int> &emptyRows,
                const vector<int> &emptyCols) {

  ulong sum = 0;
  for (int i = 0; i < gs.size() - 1; ++i) {
    for (int j = i; j < gs.size(); ++j) {
      const auto &gi = gs[i];
      const auto &gj = gs[j];

      if (gi.first == gj.first && gi.second == gj.second) {
        continue;
      }

      ulong d = abs(gi.first - gj.first) + abs(gi.second - gj.second) +
                countEmpty(emptyRows, gi.first, gj.first) +
                countEmpty(emptyCols, gi.second, gj.second);

      sum += d;
    }
  }

  return sum;
}

const ulong GrowFactor = 1000000;
unsigned long long calcPaths2(const vector<pair<int, int>> &gs,
                              const vector<int> &emptyRows,
                              const vector<int> &emptyCols) {

  unsigned long long sum = 0;
  for (int i = 0; i < gs.size() - 1; ++i) {
    for (int j = i; j < gs.size(); ++j) {
      const auto &gi = gs[i];
      const auto &gj = gs[j];

      if (gi.first == gj.first && gi.second == gj.second) {
        continue;
      }

      uint growRow = countEmpty(emptyRows, gi.first, gj.first);
      uint growCol = countEmpty(emptyCols, gi.second, gj.second);
      ulong d = abs(gi.first - gj.first) + abs(gi.second - gj.second) +
                growRow * (GrowFactor - 1) + growCol * (GrowFactor - 1);

      sum += d;
    }
  }

  return sum;
}

int main() {
  ifstream in("../../2023/examples/d11e1");
  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  vector<string> map;

  string line;
  while (getline(cin, line)) {
    map.push_back(line);
  }

  vector<pair<int, int>> gs;
  vector<int> emptyRows;
  vector<int> emptyCols;
  vector<bool> eCols = vector<bool>(map[0].size(), true);
  for (int i = 0; i < map.size(); ++i) {
    bool wasEmpty = true;
    for (int j = 0; j < map[0].size(); ++j) {
      if (map[i][j] == '#') {
        gs.push_back(make_pair(i, j));
        wasEmpty = false;
        eCols[j] = false;
      }
    }

    if (wasEmpty) {
      emptyRows.push_back(i);
    }
  }

  for (int i = 0; i < eCols.size(); ++i) {
    if (eCols[i]) {
      emptyCols.push_back(i);
    }
  }

  auto sum = calcPaths(gs, emptyRows, emptyCols);
  cout << "Sum: " << sum << endl;

  auto sum2 = calcPaths2(gs, emptyRows, emptyCols);
  cout << "Sum2: " << sum2 << endl;

  return 0;
}
