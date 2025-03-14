
#include <iostream>
#include <string>
#include <string_view>
using namespace std;

int main() {
  int maxRed = 12, maxGreen = 13, maxBlue = 14;

  string line;
  int result = 0;
  long power = 0;
  while (getline(cin, line)) {
    int gameId = 0;
    bool posible = true;

    int ns = 0;
    int r = 0, g = 0, b = 0;
    int num = 0;
    int mr = 1, mg = 1, mb = 1;
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == ' ') {
        if (ns != 0) {
          num = stoi(string(line.begin() + ns, line.begin() + i));
        }
        ns = i + 1;
      }

      if (line[i] == ':') {
        gameId = stoi(string(line.begin() + ns, line.begin() + i));
      }

      if (line[i] == ',' || line[i] == ';' || i == line.size() - 1) {
        if (line[ns] == 'r')
          r = num;
        if (line[ns] == 'g')
          g = num;
        if (line[ns] == 'b')
          b = num;

        if (line[i] == ';' || i == line.size() - 1) {
          if (r > maxRed || g > maxGreen || b > maxBlue) {
            posible = false;
          }

          if (r > mr)
            mr = r;
          if (g > mg)
            mg = g;
          if (b > mb)
            mb = b;

          r = g = b = 0;
          num = 0;
        }

        ns = 0;
      }
    }

    if (posible) {
      result += gameId;
    }

    power += mr * mg * mb;
  }

  cout << result << endl;
  cout << "power: " << power << endl;

  return 0;
}
