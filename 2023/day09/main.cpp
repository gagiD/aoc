#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;

long extrapolateForward(vector<int> &v) {
  bool allZero;
  ushort level = 1;
  do {
    allZero = true;
    for (int i = 0; i < v.size() - level; ++i) {
      int diff = v[i + 1] - v[i];
      v[i] = diff;
      if (diff != 0)
        allZero = false;
    }

    level++;
    if (level == v.size())
      break;
  } while (!allZero);

  return accumulate(v.begin(), v.end(), 0);
}

long extrapolateBackward(vector<int> &v) {
  bool allZero;
  ushort level = 1;
  do {
    allZero = true;
    for (int i = v.size() - 1; i >= 0 + level; --i) {
      int diff = v[i] - v[i - 1];
      v[i] = diff;
      if (diff != 0)
        allZero = false;
    }

    level++;
    if (level == v.size())
      break;
  } while (!allZero);

  long ext = v[0];
  long sign = -1;
  for (int i = 1; i <= level; ++i) {
    ext += sign * v[i];
    sign *= -1;
  }

  return ext;
}

int main() {
  vector<vector<int>> seq;

  string line;
  int n;
  ushort i = 0;
  while (getline(cin, line)) {
    stringstream ss(line);

    seq.push_back(vector<int>());

    while (ss >> n)
      (seq[i]).push_back(n);

    ++i;
  }

  vector<vector<int>> seqB = seq;
  long sum = 0;
  for (auto &v : seq) {
    sum += extrapolateForward(v);
  }
  long sumB = 0;
  for (auto &v : seqB) {
    sumB += extrapolateBackward(v);
  }

  cout << "Sum forward: " << sum << endl;
  cout << "Sum backward: " << sumB << endl;

  return 0;
}
