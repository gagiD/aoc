#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <unistd.h>
#include <vector>

using namespace std;

struct SeedRange {
  long start;
  long length;
};

struct SeedMap {
  long destination;
  long source;
  long length;
};

void inputMap(vector<SeedMap> &map, istream &in) {
  string line;
  long d, s, l;
  while (true) {
    getline(in, line);
    if (line.empty())
      break;
    auto ss = istringstream{line};
    ss >> d >> s >> l;
    map.push_back({d, s, l});
  }
}

long getNext(long s, const vector<SeedMap> &map) {
  for (const auto &m : map) {
    if (s >= m.source && s < m.source + m.length)
      return m.destination + s - m.source;
  }
  return s;
}

long traverse(const vector<long> &seeds, const vector<vector<SeedMap>> &maps) {
  long sn = -1;
  long min = numeric_limits<long long>::max();

  for (const auto &s : seeds) {
    sn = s;

    for (const auto &m : maps) {
      sn = getNext(sn, m);
    }

    if (sn < min)
      min = sn;
  }

  return min;
}

vector<SeedRange> getNextRange(vector<SeedRange> &ranges,
                               const vector<SeedMap> &map) {
  std::vector<SeedRange> updatedRanges;

  for (const auto &range : ranges) {
    long start = range.start;
    long length = range.length;

    for (const auto &mapping : map) {
      if (start >= mapping.source && start < mapping.source + mapping.length) {
        // Adjust the start position within the mapping
        long offset = start - mapping.source;
        start = mapping.destination + offset;

        // Calculate the remaining length within the mapping
        long remainingLength = mapping.length - offset;

        // Update the length considering the mapping length and remaining length
        length = (length < remainingLength) ? length : remainingLength;
      }
    }

    updatedRanges.push_back({start, length});
  }

  return updatedRanges;
}

long traverse2(const vector<long> &seeds, const vector<vector<SeedMap>> &maps) {
  vector<SeedRange> seedRanges;

  for (int i = 0; i < seeds.size(); i += 2) {
    seedRanges.push_back({seeds[i], seeds[i + 1]});
  }

  for (const auto &map : maps) {
    seedRanges = getNextRange(seedRanges, map);

    cout << "sRange\n";
    for (const auto &r : seedRanges) {
      cout << r.start << " " << r.length << endl;
    }
  }

  return -1;
}

int main() {
  ifstream in("../../2023/examples/d5e1");

  if (isatty(STDIN_FILENO)) {
    cin.rdbuf(in.rdbuf());
  }

  vector<long> seeds;
  vector<SeedMap> seedToSoil;
  vector<SeedMap> soilToFert;
  vector<SeedMap> fertToWater;
  vector<SeedMap> waterToLight;
  vector<SeedMap> lightToTemp;
  vector<SeedMap> tempToHumid;
  vector<SeedMap> humidToLoc;

  string line;
  long n;
  istringstream ss;
  long s, d, l;

  getline(cin, line);
  ss = istringstream{line.substr(7)};
  while (ss >> n) {
    seeds.push_back(n);
  }
  getline(cin, line);

  getline(cin, line);
  if (line == "seed-to-soil map:") {
    inputMap(seedToSoil, cin);
  }

  getline(cin, line);
  if (line == "soil-to-fertilizer map:") {
    inputMap(soilToFert, cin);
  }

  getline(cin, line);
  if (line == "fertilizer-to-water map:") {
    inputMap(fertToWater, cin);
  }

  getline(cin, line);
  if (line == "water-to-light map:") {
    inputMap(waterToLight, cin);
  }

  getline(cin, line);
  if (line == "light-to-temperature map:") {
    inputMap(lightToTemp, cin);
  }

  getline(cin, line);
  if (line == "temperature-to-humidity map:") {
    inputMap(tempToHumid, cin);
  }

  getline(cin, line);
  if (line == "humidity-to-location map:") {
    inputMap(humidToLoc, cin);
  }

  auto r = traverse(seeds, {seedToSoil, soilToFert, fertToWater, waterToLight,
                            lightToTemp, tempToHumid, humidToLoc});
  cout << "result:" << r << endl;

  auto r2 = traverse2(seeds, {seedToSoil, soilToFert, fertToWater, waterToLight,
                              lightToTemp, tempToHumid, humidToLoc});
  cout << "result2:" << r2 << endl;
}
