/*
-----------------------------------------------------------------------------------
Laboratoire : Labo07
Fichier     : Labo07.cpp
Auteur(s)   : Simon Mattei, Doran Kayoumi, Melodie Uhan
Date        : 05.12.2018

But         : Convertisseur de nombre Arab en Romain et Romain en Arab

Remarque(s) :

Compilateur : MinGW-g++ <6.3.0>
-----------------------------------------------------------------------------------
 */

#include <iostream>
#include <cmath>
#include <sstream>

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

/**
 * @brief check if a value is a valid roman numeral
 *
 * @param[string] input value to check
 * @param[int] MAX value the numeral can have
 * @param[int] MIN value the numeral can have
 * @return[bool] true if it's a numeral, otherwise false
 */
bool isValidRomanNumeral(const string &input, const int &MAX, const int &MIN);

/**
 * @brief check if a value is a valid arabic number
 *
 * @param[string] input value to check
 * @param[int] MAX value the number can have
 * @param[int] MIN value the number can have
 * @return[bool] true if it's a number, otherwise false
 */
bool isValidArabNumber(const string &input, const int &MAX, const int &MIN);

/**
 * @brief convert a sting to int
 * 
 * @param toConvert value to convert to int
 * @return[int] converted value
 */
int stringToInt(const string &toConvert);

const int REPETITION_OVERFLOW = 4;
const int SPECIAL_SUBTRACT = 4;
const int MAX_VALUE = 4999;
const int MIN_VALUE = 1;

const string ERROR_MESSAGE = "Non valide";

enum Roman {
    I = 'I',
    V = 'V',
    X = 'X',
    L = 'L',
    C = 'C',
    D = 'D',
    M = 'M'
};

int main() {
  string input;

//  for (int i = 1; i <= MAX_VALUE; ++i) {
//    cout << i << " " << toRoman(i) << " " << toArab(toRoman(i)) << endl;
//  }

  while (getline(cin, input) and !input.empty()) {
    if (isValidArabNumber(input, MAX_VALUE, MIN_VALUE)) {
      cout << toRoman(stringToInt(input));
    } else if (isValidRomanNumeral(input, MAX_VALUE, MIN_VALUE)) {
      cout << toArab(input);
    } else {
      cout << ERROR_MESSAGE;
    }
    cout << endl;
  }

  return 0;
}

int toArab(string roman) {
  int arabNumber = 0;

  for (size_t i = 0; i < roman.length(); ++i) {
    int current = getRomanArabValue(roman.at(i));

    // check if we need to subtract the current number
    if (i + 1 < roman.length() and current < getRomanArabValue(roman.at(i + 1))) current *= -1;

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

    if (currentDigit == 0) {
      arab /= 10;
      continue;
    }

    // get the numeral value closest to the current digit and get the arab value of that numeral
    char numeral = getLargestRomanNumeral(currentDigit);
    int arabValue = getRomanArabValue(numeral);

    // if the current digit is equal to the arab value,
    // then nothing special is needed.
    if (currentDigit == arabValue) {
      currentNumeral = addNumerals(numeral, 1);

      // check if we're in the thousands
    } else if (numeral == Roman::M) {
      // if so, we can repeat the thousands an infinite amount of time.
      currentNumeral = addNumerals(numeral, static_cast<unsigned>(currentDigit / pow(10, i)));
    } else {
      // calculate the difference between the current digit and the arab value.
      // this difference will be able to determine the number of repetition is needed
      // to build the current digit with it's numeral
      auto difference = static_cast<unsigned>((currentDigit - arabValue) / pow(10, i));

      // if the difference is greater or equal than the REPETITION_OVERFLOW or equal to the special case,
      // then, the numeral is the next largest numeral subtracted by it's complement.
      if (difference >= REPETITION_OVERFLOW or currentDigit == SPECIAL_SUBTRACT * pow(10, i)) {
        numeral = getLargestRomanNumeral(static_cast<int>(currentDigit + pow(10, i)));
        currentNumeral = subNumerals(numeral, getRomanNumeralComplement(numeral));
      } else {
        //
        currentNumeral += addNumerals(numeral, 1);
        if (numeral == Roman::V or numeral == Roman::L or numeral == Roman::D) {
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

bool isValidArabNumber(const string &input, const int &MAX, const int &MIN) {
  return input.length() == to_string(stringToInt(input)).length()
          and (stringToInt(input) >= MIN and stringToInt(input) <= MAX);
}

bool isValidRomanNumeral(const string &input, const int &MAX, const int &MIN) {
  return input == toRoman(toArab(input)) and (toArab(input) >= MIN and toArab(input) <= MAX);
}

int getRomanArabValue(char roman) {

  switch (roman) {
    case Roman::I:
      return 1;
    case Roman::V:
      return 5;
    case Roman::X:
      return 10;
    case Roman::L:
      return 50;
    case Roman::C:
      return 100;
    case Roman::D:
      return 500;
    case Roman::M:
      return 1000;
    default:
      return 0;
  }
}

char getLargestRomanNumeral(int arab) {

  if (arab >= 1000) {
    return Roman::M;
  } else if (arab >= 500) {
    return Roman::D;
  } else if (arab >= 100) {
    return Roman::C;
  } else if (arab >= 50) {
    return Roman::L;
  } else if (arab >= 10) {
    return Roman::X;
  } else if (arab >= 5) {
    return Roman::V;
  } else {
    return Roman::I;
  }
}

char getRomanNumeralComplement(char roman) {
  switch (roman) {
    case Roman::V:
    case Roman::X:
      return Roman::I;
    case Roman::L:
    case Roman::C:
      return Roman::X;
    case Roman::D:
    case Roman::M:
      return Roman::C;
    default:
      return Roman::I;
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

int stringToInt(const string &toConvert) {
  int convertedInput = 0;
  stringstream ss(toConvert);
  ss >> convertedInput;

  return convertedInput;
}
