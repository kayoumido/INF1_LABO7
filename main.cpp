#include <iostream>
#include <cmath>

using namespace std;

int toArab(string roman);

string toRoman(int arab);

/**
 * @brief get the numeric value of a roman symbol
 *
 * @param[char] roman symbol to convert
 * @return[int] the numeric value of a roman symbol
 */
int getRomanArabValue(char roman);
char getLargestRomanNumeral(int arab);
char getRomanNumeralComplement(char roman);

string numerals(char numeral, unsigned repetition);
string subNumerals(char numeral, char complement);

const int MAX_REPETITION = 4;

int main() {
  int input;
  while (cin >> input) {
    cout << toRoman(input) << endl;
  }

  return 0;
}

int toArab(string roman) {
  int arabNumber = 0;

  for (size_t i = 0; i < roman.length(); ++i) {
    int current = getRomanArabValue(roman.at(i));
    int next = getRomanArabValue(roman[i + 1]);

    // check if we need to subtract the current number
    if (current < next) current *= -1;

    arabNumber += current;
  }

  return arabNumber;
}

string toRoman(int arab) {
  string romanNumber;

  // loop through the arab number digit by digit
  //  e.g. 21 --> first iteration : 1, second : 20
  for (int i = 0; arab > 0; ++i) {
    string currentNumeral;

    int currentDigit = static_cast<int>(arab % 10 * pow(10, i));

    // get the numeral value closest to the current digit and get the arab value of that numeral
    char numeral = getLargestRomanNumeral(currentDigit);
    int arabValue = getRomanArabValue(numeral);

    // if the current digit is equal to the arab value,
    // then nothing special is needed.
    if (currentDigit == arabValue) {
      currentNumeral = numerals(numeral, 1);
    } else {
      // calculate the difference between the current digit and the arab value.
      // this difference will be able to determine the number of repetition is needed
      // to build the current digit with it's numeral
      auto difference = static_cast<unsigned>((currentDigit - arabValue) / pow(10, i));

      // if the difference is greater than the MAX_REPETITION or equal to the special case,
      // then, the numeral is the next largest numeral subtracted by it's complement.
      if (difference >= MAX_REPETITION or currentDigit == 4 * pow(10, i)) {
        numeral = getLargestRomanNumeral(static_cast<int>(currentDigit + pow(10, i)));
        currentNumeral = subNumerals(numeral, getRomanNumeralComplement(numeral));
      } else {
        //
        currentNumeral += numerals(numeral, 1);
        if (numeral == 'V' or numeral == 'L' or numeral == 'D') {
          currentNumeral += numerals(getRomanNumeralComplement(numeral), difference);
        } else {
          currentNumeral += numerals(numeral, difference);
        }
      }
    }

    romanNumber.insert(0, currentNumeral);

    arab /= 10;
  }
  return romanNumber;
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
    case 'd':
    case 'D':
      return 500;
    case 'm':
    case 'M':
      return 1000;
    default:
      return 0;
  }
}

char getLargestRomanNumeral(int arab) {

  if (arab >= 1000) {
    return 'M';
  } else if (arab >= 500) {
    return 'D';
  } else if (arab >= 100) {
    return 'C';
  } else if (arab >= 50) {
    return 'L';
  } else if (arab >= 10) {
    return 'X';
  } else if (arab >= 5) {
    return 'V';
  } else if (arab >= 1) {
    return 'I';
  }
}

char getRomanNumeralComplement(char roman) {
  switch (roman) {
    case 'V':
    case 'X':
      return 'I';
    case 'L':
    case 'C':
      return 'X';
    case 'D':
    case 'M':
      return 'C';
    default:
      return 'I';
  }
}

string numerals(char numeral, unsigned repetition) {
  string string;
  for (unsigned j = 0; j < repetition; ++j) {
    string += numeral;
  }
  return string;
}

string subNumerals(char numeral, char complement) {
  string string;
  string += complement;
  string += numeral;

  return string;
}
