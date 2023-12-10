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
  const char *name;
  long destination;
  long source;
  long length;
};

void inputMap(const char *name, vector<SeedMap> &map, istream &in) {
  string line;
  long d, s, l;
  while (true) {
    getline(in, line);
    if (line.empty())
      break;
    auto ss = istringstream{line};
    ss >> d >> s >> l;
    map.push_back({name, d, s, l});
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
  long sn = 0;
  long min = numeric_limits<long>::max();

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

  for (int ri = 0; ri < ranges.size(); ri++) {
    const auto &range = ranges[ri];
    long rangeStart = range.start;
    long rangeEnd = range.start + range.length - 1;

    bool found = false;
    for (int i = 0; i < map.size(); i++) {
      const auto &mapping = map[i];
      long sourceStart = mapping.source;
      long sourceEnd = mapping.source + mapping.length - 1;

      long mStart = max(rangeStart, sourceStart);
      long mEnd = min(rangeEnd, sourceEnd);
      long mLength = mEnd - mStart + 1;

      if (mLength > 0) {
        found = true;

        long roffset = mStart - mapping.destination;
        long moffset = mapping.source - mapping.destination;

        updatedRanges.push_back({mapping.destination + roffset - moffset, mLength});
      }
    }

    if (!found) {
      long length = range.length;

      long maxStart = map.back().destination;
      if (maxStart > range.start + length)
        length = maxStart - range.start;

      if (ri != ranges.size() - 1) {
        const SeedRange &next = ranges[ri + 1];
        if (next.start > range.start + length)
          length = next.start - range.start;
      }
      if (length > 0)
        updatedRanges.push_back({range.start, (long)length});
      else if (length < 0)
        updatedRanges.push_back({range.start, 1});
    }
  }

  return updatedRanges;
}

long minRange(const vector<SeedRange> &ranges) {
  long min = numeric_limits<long>::max();
  for (const auto &r : ranges) {
    if (r.start < min)
      min = r.start;
  }

  return min;
}

long traverse2(const vector<long> &seeds, vector<vector<SeedMap>> &maps) {
  vector<SeedRange> seedRanges;

  for (int i = 0; i < seeds.size(); i += 2) {
    seedRanges.push_back({seeds[i], seeds[i + 1]});
  }

  long mRange = numeric_limits<long>::max();
  vector<SeedRange> currRanges;
  for (const auto &r : seedRanges) {
    currRanges = {r};

    for (auto &map : maps) {
      cout << "\nsranges:" << endl;
      for (const auto &r : currRanges)
        cout << r.start << " " << r.length << endl;

      currRanges = getNextRange(currRanges, map);
    }

    cout << "\nsranges:" << endl;
    for (const auto &r : currRanges)
      cout << r.start << " " << r.length << endl;

    auto cm = minRange(currRanges);
    if (cm < mRange)
      mRange = cm;
  }

  return mRange;
}

void prepareMaps(vector<vector<SeedMap>> &maps) {
  for (auto &map : maps) {
    sort(map.begin(), map.end(),
         [](const auto &m1, const auto &m2) { return m1.source < m2.source; });

    long start = 0;
    for (int i = 0; i < map.size(); i++) {
      if (map[i].source > start) {
        map.insert(map.begin() + i,
                   {map[i].name, start, start, map[i].source - start});
        i++;
      }
      start = map[i].source + map[i].length;
    }
  }
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
    inputMap("seed-to-soil", seedToSoil, cin);
  }

  getline(cin, line);
  if (line == "soil-to-fertilizer map:") {
    inputMap("soil-to-fert", soilToFert, cin);
  }

  getline(cin, line);
  if (line == "fertilizer-to-water map:") {
    inputMap("fert-to-water", fertToWater, cin);
  }

  getline(cin, line);
  if (line == "water-to-light map:") {
    inputMap("water-to-light", waterToLight, cin);
  }

  getline(cin, line);
  if (line == "light-to-temperature map:") {
    inputMap("light-to-temp", lightToTemp, cin);
  }

  getline(cin, line);
  if (line == "temperature-to-humidity map:") {
    inputMap("temp-to-humid", tempToHumid, cin);
  }

  getline(cin, line);
  if (line == "humidity-to-location map:") {
    inputMap("humid-to-loc", humidToLoc, cin);
  }

  auto r = traverse(seeds, {seedToSoil, soilToFert, fertToWater, waterToLight,
                            lightToTemp, tempToHumid, humidToLoc});
  cout << "result:" << r << endl;

  vector<vector<SeedMap>> maps = {seedToSoil,   soilToFert,  fertToWater,
                                  waterToLight, lightToTemp, tempToHumid,
                                  humidToLoc};

  prepareMaps(maps);
  auto r2 = traverse2(seeds, maps);
  cout << "result2:" << r2 << endl;
}
