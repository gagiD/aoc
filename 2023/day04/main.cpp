#include <algorithm>
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

using namespace std;

long points(int numberOfMatches) {
  if (numberOfMatches <= 0) {
    return 0;
  }
  return 1 << (numberOfMatches - 1);
}

int main() {

  vector<int> v;
  v.reserve(10);
  vector<int> n;
  v.reserve(10);

  string line;

  long result = 0;

  vector<unsigned long> cards = {};
  short game = 0;

  while (getline(cin, line)) {
    n.clear();
    v.clear();

    bool wasGameSep = false;
    bool wasNumSep = false;

    short ns = -1;
    for (int i = 0; i < line.size(); ++i) {
      if (line[i] == ':') {
        wasGameSep = true;
      } else if (line[i] == '|') {
        wasNumSep = true;
      } else if (isdigit(line[i])) {
        if (wasGameSep && ns < 0)
          ns = i;
      } else if (ns > 0) {
        if (wasNumSep) {
          n.push_back(stoi(string(line.begin() + ns, line.begin() + i)));

          ns = -1;
        } else if (wasGameSep) {
          v.push_back(stoi(string(line.begin() + ns, line.begin() + i)));

          ns = -1;
        }
      }
    }
    if (ns > 0) {
      n.push_back(stoi(string(line.begin() + ns, line.end())));
    }

    auto count = count_if(n.begin(), n.end(), [&v](int nv) {
      return find(v.begin(), v.end(), nv) != v.end();
    });

    if (count > 0) {
      if (cards.size() < game + count + 1) {
        cards.resize(game + count + 1, 0);
      }
      cards[game]++;
      auto copiesGameI = cards[game] == 0 ? 1 : cards[game];

      for (int c = 1; c <= copiesGameI; ++c) {
        for (int i = 1; i <= count; ++i) {
          cards[game + i]++;
        }
      }
    } else {
      if (cards.size() < game + 1) {
        cards.resize(game + 1, 0);
      }

      cards[game]++;
    }

    result += points(count);

    ++game;
  }

  cout << "Result: " << result << endl;

  cout << "cards:";
  for (auto i : cards) {
    cout << i << " ";
  }
  cout << endl
       << "Strach result: " << accumulate(cards.begin(), cards.end(), 0)
       << endl;

  return 0;
}
