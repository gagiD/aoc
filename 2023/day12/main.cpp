
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Arangment {
  string schema;
  vector<uint> counts;
};

int main() {

  vector<Arangment> arangments;
  string line;
  while (getline(cin, line)) {
    stringstream ss{line};
    string schema;
    int n;
    vector<uint> counts;

    ss >> schema;
    char c;
    while (ss >> n >> c) {
      counts.push_back(n);
    }
    counts.push_back(n);

    arangments.push_back({schema, counts});
  }

  for (auto a : arangments) {
    cout << a.schema << " ";
    for (auto c : a.counts) {
      cout << c << " ";
    }
    cout << endl;
  }

  return 0;
}
