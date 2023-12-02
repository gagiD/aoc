#include <iostream>
int main() {
  std::string t = "test";

  std::string s;
  std::cin >> s;

  std::cout << (s == t) << '\n';
  std::cout << (t == std::string(s.begin(), s.begin() + t.length())) << '\n';
}
