#include <iostream>
#include <pthread.h>
#include <sstream>
#include <vector>

using namespace std;

bool goodRun(long hold, long time, long dist) {
  return (time - hold) * hold > dist;
}

long minGoodRun(long time, long dist) {
  long left = 0, right = time;
  long halfL, halfR;

  while (left <= right) {
    halfL = left + (right - left) / 2;
    halfR = halfL + 1;
    bool goodL = goodRun(halfL, time, dist);
    bool goodR = goodRun(halfR, time, dist);

    if (!goodL && goodR) {
      return halfR;
    }
    if (goodL && goodR) {
      right = halfL - 1;
    } else {
      left = halfR;
    }
  }

  return -1;
}

long numOfGoodRuns(long minGood, long time) {
  if (minGood < 0 || minGood > time) {
    return 0;
  }

  return (time + 1) - 2 * minGood;
}

void caclAll(const vector<int> &time, const vector<int> &dist) {
  int cur = 0;
  long result = 1;

  for (int i = 0; i < time.size(); ++i) {
    cur = numOfGoodRuns(minGoodRun(time[i], dist[i]), time[i]);
    cout << "Race " << i + 1 << ": " << cur << endl;
    result *= cur;
  }

  cout << "Total: " << result << endl;
}

string vectorToString(const vector<int> &v) {
  stringstream ss;
  for (auto i : v) {
    ss << i;
  }
  return ss.str();
}

void calsAll2(const vector<int> &time, const vector<int> &dist) {
  cout << "Part2:" << endl;
  cout << "Input: " << vectorToString(time) << " " << vectorToString(dist)
       << endl;
  long time2 = stol(vectorToString(time));
  long dist2 = stol(vectorToString(dist));

  long result = numOfGoodRuns(minGoodRun(time2, dist2), time2);
  cout << "Total: " << result << endl;
}

int main() {

  vector<int> time;
  vector<int> dist;

  string line;
  int n;

  getline(cin, line);
  std::istringstream is{line.substr(5)};
  while (is >> n)
    time.push_back(n);

  getline(cin, line);
  is = istringstream{line.substr(10)};
  while (is >> n)
    dist.push_back(n);

  caclAll(time, dist);

  calsAll2(time, dist);

  return 0;
}
