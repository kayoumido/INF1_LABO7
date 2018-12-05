#include <iostream>

using namespace std;

int convertRomanToArab(string roman);

/**
 * @brief get the numeric value of a roman symbol
 *
 * @param[char] roman symbol to convert
 * @return[int] the numeric value of a romain symbol
 */
int getRomanArabValue(char roman);

int main() {
  string input;
  while (cin >> input) {
    cout << convertRomanToArab(input) << endl;
  }

  return 0;
}

int convertRomanToArab(string roman) {
  int arabNumber = 0;

  for (size_t i = 0; i < roman.length(); ++i) {
    int current = getRomanArabValue(roman[i]);
    int next = getRomanArabValue(roman[i + 1]);

    // check if we need to subtract the current number
    if (current < next) current *= -1;

    arabNumber += current;
  }

  return arabNumber;
}

int getRomanArabValue(char roman) {

  switch (roman) {
    case 'i':
    case 'I':
      return 1;
    case 'v':
    case 'V':
      return 5;
    case 'x':
    case 'X':
      return 10;
    case 'l':
    case 'L':
      return 50;
    case 'c':
    case 'C':
      return 100;
    case 'D':
      return 500;
    case 'm':
    case 'M':
      return 1000;
    default:
      return 0;
  }
}
