package main

import (
	"fmt"
	"strings"
)

func getFirstDigit(str string) int {
	for i := range str {
		if str[i] >= '0' && str[i] <= '9' {
			return int(str[i] - '0')
		}
	}
	return 0
}

func getLastDigit(str string) int {
	for i := range str {
		if str[len(str)-i-1] >= '0' && str[len(str)-i-1] <= '9' {
			return int(str[len(str)-i-1] - '0')
		}
	}
	return 0
}

func getFirstLastNumberFromLine(str string) (int, int) {
	return getFirstDigit(str), getLastDigit(str)
}

var numberStrs = []string{"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"}

func getFirstDigitParsed(str string) int {
	for i := range str {
		if str[i] >= '0' && str[i] <= '9' {
			return int(str[i] - '0')
		}

		for di, ds := range numberStrs {
			if strings.HasPrefix(str[i:], ds) {
				return di + 1
			}
		}
	}
	return 0
}

func getLastDigitParsed(str string) int {
	for i := range str {
		if str[len(str)-i-1] >= '0' && str[len(str)-i-1] <= '9' {
			return int(str[len(str)-i-1] - '0')
		}

		for di, ds := range numberStrs {
			if len(str)-i-len(ds) >= 0 && strings.HasPrefix(str[len(str)-i-len(ds):], ds) {
				return di + 1
			}
		}
	}
	return 0
}

func getFirstLastNumberFromLineParsed(str string) (int, int) {
	return getFirstDigitParsed(str), getLastDigitParsed(str)
}

func main() {
	var sum1, sum2 int

	for {
		var s string
		_, err := fmt.Scanln(&s)
		if err != nil {
			break
		}

		first, last := getFirstLastNumberFromLine(s)
		sum1 += first*10 + last

		first2, last2 := getFirstLastNumberFromLineParsed(s)
		sum2 += first2*10 + last2
	}

	fmt.Println("Sum1: ", sum1)
	fmt.Println("Sum2: ", sum2)
}
