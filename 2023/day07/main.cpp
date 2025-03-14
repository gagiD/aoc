#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <type_traits>
#include <unistd.h>
#include <vector>

using namespace std;

enum class Card { c2 = 2, c3, c4, c5, c6, c7, c8, c9, T, J, Q, K, A };

enum class Card2 { J = 1, c2, c3, c4, c5, c6, c7, c8, c9, T, Q, K, A };

template <typename T = Card> struct Hand {
  array<T, 5> cards;
  int bid;
};

template <typename T> T charToCard(char c) {
  if (c >= '2' && c <= '9')
    return static_cast<T>(c - '0');
  if (c == 'T')
    return T::T;
  if (c == 'J')
    return T::J;
  if (c == 'Q')
    return T::Q;
  if (c == 'K')
    return T::K;
  if (c == 'A')
    return T::A;

  return T::c2; // shpould never happen
}

template <typename T> array<T, 5> strToCards(const string &s) {
  array<T, 5> cards;

  for (int i = 0; i < 5; ++i) {
    char c = s[i];
    cards[i] = charToCard<T>(c);
  }

  return cards;
}

template <typename T> unsigned long hashCards(const array<T, 5> &cards) {
  unsigned long hash = 0;

  for (int i = 0; i < 5; ++i) {
    hash += static_cast<unsigned long>(cards[i]) * (unsigned long)pow(100, i);
  }

  return hash;
}

template <typename T> int handType(const array<T, 5> &cards) {
  array<int, 15> histogram = {0};
  short jokers = 0;
  for (auto c : cards) {
    if constexpr (is_same<T, Card2>::value) {
      if (c == Card2::J)
        jokers++;
      else
        histogram[static_cast<int>(c)]++;
    } else
      histogram[static_cast<int>(c)]++;
  }

  sort(histogram.begin(), histogram.end(), greater<int>());

  if (is_same<T, Card2>::value && jokers > 0) {
    if (jokers == 5)
      return 1;
    histogram[0] += jokers;
  }

  short triples = 0, doubles = 0;
  for (int i = 0; i < 5; ++i) {
    if (histogram[i] == 5)
      return 1;
    if (histogram[i] == 4)
      return 2;
    if (histogram[i] == 3)
      triples++;
    if (histogram[i] == 2)
      doubles++;
  }

  if (triples == 1 && doubles == 1)
    return 3;
  if (triples == 1)
    return 4;
  if (doubles == 2)
    return 5;
  if (doubles == 1)
    return 6;

  return 7;
}

template <typename T> unsigned long cardsDigitsValue(const array<T, 5> &cards) {
  unsigned long value = 0;

  for (int i = 4; i >= 0; --i) {
    value += (14 - static_cast<unsigned long>(cards[i])) *
             (unsigned long)pow(100, 4 - i);
  }

  return value;
}

template <typename T> unsigned long handSortValue(const array<T, 5> &cards) {
  static map<unsigned long, unsigned long> memo1;
  static map<unsigned long, unsigned long> memo2;

  auto &map = is_same<T, Card>::value ? memo1 : memo2;

  auto hash = hashCards(cards);
  auto memoIt = map.find(hash);
  if (memoIt != map.end())
    return memoIt->second;

  unsigned long value = handType(cards) * 10000000000 + cardsDigitsValue(cards);
  map[hash] = value;

  return value;
}

template <typename T> void sortHands(vector<T> &hands) {
  sort(hands.begin(), hands.end(), [](const T &h1, const T &h2) {
    if (handSortValue(h1.cards) < handSortValue(h2.cards))
      return true;
    return false;
  });
}

// sorted hands
template <typename T> unsigned long handsTotal(const vector<T> &hands) {
  unsigned long total = 0;
  for (int i = 0; i < hands.size(); ++i) {
    total += (hands.size() - i) * hands[i].bid;
  }

  return total;
}

int main() {
  ifstream in("../../2023/examples/d7e1");
  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  string hand;
  int bid;

  vector<Hand<Card>> hands;
  vector<Hand<Card2>> hands2;

  while (cin >> hand >> bid) {
    hands.push_back({strToCards<Card>(hand), bid});
    hands2.push_back({strToCards<Card2>(hand), bid});
  }

  sortHands(hands);
  sortHands(hands2);

  cout << "\nTotal: " << handsTotal(hands) << endl;
  cout << endl;
  cout << "\nTotal2: " << handsTotal(hands2) << endl;
}
