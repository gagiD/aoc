# Advent of Code

## Description

Some implementation and 2 helper scripts(only work on Linux).
Run script will also watch for that language file changes and will run it automatically.

## Usage

### run

Run the solution for the specified day(default: today).

```bash
./run [-y YEAR] [-d DAY] [-l LANG] [-i INPUT_TYPE]
```

#### Options

- `-y YEAR`: Specify the year for the Advent of Code challenge (default: current year).
- `-d DAY`: Specify the day for which you want to run the solution (default: current day).
- `-l LANG`: Specify the programming language of your solution (default: "cpp"). Options include "cpp", "go", and "rs".
- `-i INPUT_TYPE`: Specify the input type: "e1" for examples, "i1" for input 1 "i2" for input 2 (default: "e1").

#### Example

```bash
./run -l rs -i i1
```

### createToday

This scripts generates files for current day.
Generates all languages files and also example and input files.

This scripts generates files for current day.

```bash
./createToday
```
