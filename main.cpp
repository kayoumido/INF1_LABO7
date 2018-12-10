#include <iostream>
#include <cmath>

using namespace std;

/**
 * @brief convert a roman numeral to and arabic number
 *
 * @param[string] roman numeral to convert
 * @return[int] arabic number for the given numeral
 */
int toArab(string roman);

/**
 * @brief convert an arabic number into a roman numeral
 *
 * @param[int] arab
 * @return[string] roman numeral for the given number
 */
string toRoman(int arab);

/**
 * @brief get the numeric value of one of the seven roman numerals
 *
 * @param[char] roman numeral to get it's numeric value
 * @return[int] the numeric value of the given numeral
 */
int getRomanArabValue(char roman);

/**
 * @brief get the largest roman numeral for a given number
 * @example 9 --> V, 30 --> X
 *
 * @param[int] arab number to get it's largest numeral
 * @return[char] the roman numeral
 */
char getLargestRomanNumeral(int arab);

/**
 * @brief get the complement of a roman numeral
 * @note the complement of a numeral, is the numeral that
 *       helps with addition/subtractions of the numeral
 *
 * @example X's complement is I. With it, we can compose values like XI(11) and IX(9)
 * @param[char] roman numeral to get it's complement
 * @return[char] the complementary numeral
 */
char getRomanNumeralComplement(char roman);

/**
 * @brief compose an additive numeral by repeating a numeral
 * @example arab number 3 is composed of 3 times the numeral I --> III
 *
 * @param[char] numeral to repeat
 * @param[unsigned] repetition number of repetition
 * @return[string] a numeral composed of a repetition of another numeral
 */
string addNumerals(char numeral, unsigned repetition);

/**
 * @brief compose a subtractive numeral by using a numeral and it's complement
 * @example arab number 9 is composed of the numeral X and it's complement I --> IX
 *
 * @param[char] numeral
 * @param[char] complement
 * @return[string] a numeral composed of a numeral and it's complement
 */
string subNumerals(char numeral, char complement);

const int REPETITION_OVERFLOW = 4;
const int SPECIAL_SUBSTRACT = 4;

const char ROMAN_I = 'I';
const char ROMAN_V = 'V';
const char ROMAN_X = 'X';
const char ROMAN_L = 'L';
const char ROMAN_C = 'C';
const char ROMAN_D = 'D';
const char ROMAN_M = 'M';

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

    // check if we need to subtract the current number
    if (i + 1 < roman.length() and current < getRomanArabValue(roman[i + 1])) current *= -1;

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
      currentNumeral = addNumerals(numeral, 1);
    } else {
      // calculate the difference between the current digit and the arab value.
      // this difference will be able to determine the number of repetition is needed
      // to build the current digit with it's numeral
      auto difference = static_cast<unsigned>((currentDigit - arabValue) / pow(10, i));

      // if the difference is greater or equal than the REPETITION_OVERFLOW or equal to the special case,
      // then, the numeral is the next largest numeral subtracted by it's complement.
      if (difference >= REPETITION_OVERFLOW or currentDigit == SPECIAL_SUBSTRACT * pow(10, i)) {
        numeral = getLargestRomanNumeral(static_cast<int>(currentDigit + pow(10, i)));
        currentNumeral = subNumerals(numeral, getRomanNumeralComplement(numeral));
      } else {
        //
        currentNumeral += addNumerals(numeral, 1);
        if (numeral == ROMAN_V or numeral == ROMAN_L or numeral == ROMAN_D) {
          currentNumeral += addNumerals(getRomanNumeralComplement(numeral), difference);
        } else {
          currentNumeral += addNumerals(numeral, difference);
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
    case ROMAN_I:
      return 1;
    case ROMAN_V:
      return 5;
    case ROMAN_X:
      return 10;
    case ROMAN_L:
      return 50;
    case ROMAN_C:
      return 100;
    case ROMAN_D:
      return 500;
    case ROMAN_M:
      return 1000;
    default:
      return 0;
  }
}

char getLargestRomanNumeral(int arab) {

  if (arab >= 1000) {
    return ROMAN_M;
  } else if (arab >= 500) {
    return ROMAN_D;
  } else if (arab >= 100) {
    return ROMAN_C;
  } else if (arab >= 50) {
    return ROMAN_L;
  } else if (arab >= 10) {
    return ROMAN_X;
  } else if (arab >= 5) {
    return ROMAN_V;
  } else if (arab >= 1) {
    return ROMAN_I;
  }
}

char getRomanNumeralComplement(char roman) {
  switch (roman) {
    case ROMAN_V:
    case ROMAN_X:
      return ROMAN_I;
    case ROMAN_L:
    case ROMAN_C:
      return ROMAN_X;
    case ROMAN_D:
    case ROMAN_M:
      return ROMAN_C;
    default:
      return ROMAN_I;
  }
}

string addNumerals(char numeral, unsigned repetition) {
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
