#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

map<pair<short, short>, vector<short>> gearMemo = {};

bool isAdjecent(const vector<string> &m, short number, short il, short lSize,
                short ns, short ne) {
  short start = max(il - 1, 0);
  short end = min(il + 1, short(m.size()) - 1);
  short lStart = max(short(ns - 1), short{0});
  short lEnd = min(ne, short(lSize - short(1)));

  for (short i = start; i <= end; ++i) {
    for (short j = lStart; j <= lEnd; ++j) {
      if (i == il && (j >= ns && j < ne)) {
        continue;
      }
      if (!isdigit(m[i][j]) && m[i][j] != '.') {
        if (m[i][j] == '*') {
          gearMemo[{i, j}].push_back(number);
        }
        return true;
      }
    }
  }

  return false;
}

long sumAdjecent(const vector<string> &m) {
  long result = {0};

  for (short i = 0; i < m.size(); ++i) {
    short ns = -1, ne = -1;

    for (short j = 0; j < m[i].size(); ++j) {
      if (isdigit(m[i][j])) {
        if (ns < 0)
          ns = j;
      } else {
        if (ns >= 0)
          ne = j;

        if (ne > 0) {
          short num = stoi(string(m[i].begin() + ns, m[i].begin() + ne));
          if (isAdjecent(m, num, i, m[i].size(), ns, ne)) {
            result += num;
          }

          ns = -1;
          ne = -1;
        } else {
          ns = -1;
          ne = -1;
        }
      }
    }

    if (ns > 0) {
      short num = stoi(string(m[i].begin() + ns, m[i].begin() + m[i].size()));
      if (isAdjecent(m, num, i, m[i].size(), ns, m[i].size())) {
        result += num;
      }
    }
  }

  return result;
}

long sumGearRatios() {
  long result = {0};
  for (const auto &kv : gearMemo) {
    if (kv.second.size() == 2) {
      result += kv.second[0] * kv.second[1];
    }
  }

  return result;
}

int main() {
  string line;
  line.reserve(144);
  vector<string> lines;
  lines.reserve(144);

  while (getline(cin, line)) {
    lines.push_back(line);
  }

  cout << "Result: " << sumAdjecent(lines) << endl;
  cout << "Gear ratios: " << sumGearRatios() << endl;

  return 0;
}
