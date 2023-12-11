#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>
#include <tuple>
#include <unistd.h>
#include <vector>

using namespace std;

/* Example of loop
.S-7.
.|.|.
.L-J.
*/

vector<string> m;
vector<vector<bool>> loopMap;
vector<vector<bool>> currMap;

void clearMap() {
  for (int i = 0; i < currMap.size(); i++) {
    for (int j = 0; j < currMap[0].size(); j++) {
      currMap[i][j] = false;
    }
  }
}

pair<int, int> findS(const vector<string> &m) {
  for (int i = 0; i < m.size(); i++) {
    for (int j = 0; j < m[0].size(); j++) {
      if (m[i][j] == 'S')
        return {i, j};
    }
  }

  return {-1, -1};
}

enum class Dir { Up, Left, Right, Down, None };

Dir oppositeDir(Dir d) {
  switch (d) {
  case Dir::Up:
    return Dir::Down;
  case Dir::Left:
    return Dir::Right;
  case Dir::Right:
    return Dir::Left;
  case Dir::Down:
    return Dir::Up;
  default:
    return Dir::None;
  }
}

enum class Conn {
  None,
  Vertical,
  Horizontal,
  NorthEast,
  NorthWest,
  SouthEast,
  SouthWest,
  All
};

Conn charToDir(char c) {
  switch (c) {
  case 'S':
    return Conn::All;
  case '|':
    return Conn::Vertical;
  case '-':
    return Conn::Horizontal;
  case 'L':
    return Conn::NorthEast;
  case 'J':
    return Conn::NorthWest;
  case '7':
    return Conn::SouthWest;
  case 'F':
    return Conn::SouthEast;
  default:
    return Conn::None;
  }
}

struct Cell {
  int i;
  int j;
  Dir from;
  Conn conn;
};

vector<Dir> connDirs(Conn conn) {
  switch (conn) {
  case Conn::All:
    return {Dir::Up, Dir::Down, Dir::Left, Dir::Right};
  case Conn::Vertical:
    return {Dir::Up, Dir::Down};
  case Conn::Horizontal:
    return {Dir::Left, Dir::Right};
  case Conn::NorthEast:
    return {Dir::Up, Dir::Right};
  case Conn::NorthWest:
    return {Dir::Up, Dir::Left};
  case Conn::SouthEast:
    return {Dir::Down, Dir::Right};
  case Conn::SouthWest:
    return {Dir::Down, Dir::Left};
  case Conn::None:
    return {};
  }
  return {};
}

bool connCompatible(Conn current, Conn next, Dir d) {
  const auto currDirs = connDirs(current);
  const auto nextDirs = connDirs(next);

  for (auto i : currDirs) {
    for (auto j : nextDirs)
      if (i == d && i == oppositeDir(j))
        return true;
  }

  return false;
}

array<optional<Cell>, 4> getDirections(const vector<string> &map,
                                       const Cell &current) {

  array<optional<Cell>, 4> res = {nullopt};

  for (const auto &dir : {Dir::Up, Dir::Left, Dir::Right, Dir::Down}) {
    if (dir == oppositeDir(current.from))
      continue;

    int i = current.i + (dir == Dir::Up ? -1 : dir == Dir::Down ? 1 : 0);
    if (i >= map.size())
      continue;

    int j = current.j + (dir == Dir::Left ? -1 : dir == Dir::Right ? 1 : 0);
    if (j >= map[0].size())
      continue;

    Conn conn = charToDir(map[i][j]);
    if (conn == Conn::None)
      continue;

    if (connCompatible(current.conn, conn, dir))
      res[static_cast<int>(dir)] = Cell{i, j, dir, conn};
  }

  return res;
}

ulong travel(const vector<string> &map, ulong step, const Cell &next) {
  const auto dirs = getDirections(map, next);

  for (const auto &dir : dirs) {
    if (dir == nullopt)
      continue;

    currMap[next.i][next.j] = true;

    if (dir->conn == Conn::All) {
      return step;
    }

    return travel(map, step + 1, *dir);
  }

  return step;
}

void travelFromS(const vector<string> &map) {
  auto [i, j] = findS(map);
  ulong max = 0;

  const auto s = Cell{i, j, Dir::None, Conn::All};
  const auto dirs = getDirections(map, s);
  for (const auto &dir : dirs) {
    if (dir == nullopt)
      continue;
    clearMap();

    auto res = travel(map, 1, *dir);
    if (res > max) {
      max = res;
      currMap[i][j] = true;
      loopMap = currMap;
      clearMap();
    }
  }

  cout << "max: " << round(max / 2.) << endl;
}

uint coutInner() {
  uint sum = 0;

  cout << endl;
  for (int i = 0; i < loopMap.size(); i++) {
    bool isInside = false;
    for (int j = 0; j < loopMap[0].size(); j++) {
      if (loopMap[i][j]) {
        cout << loopMap[i][j];
        auto conn = charToDir(m[i][j]);
        if (conn == Conn::Vertical || conn == Conn::SouthEast ||
            conn == Conn::SouthWest) {
          isInside = !isInside;
        }
      } else {

        if (isInside) {
          sum++;
          cout << 'T';
        } else {
          cout << '.';
        }
      }
    }
    cout << endl;
  }

  return sum;
}

int main() {
  ifstream in("../../2023/inputs/d10i1");
  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  string line;
  while (getline(cin, line)) {
    m.push_back(line);
    currMap.push_back(vector<bool>(line.size(), false));
  }

  travelFromS(m);

  cout << "Sum inner: " << coutInner() << endl;

  return 0;
}
