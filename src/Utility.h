//
// Created by Filip Dabkowski on 17/11/2023.
//

#ifndef BACKGAMMONGAME_UTILITY_H
#define BACKGAMMONGAME_UTILITY_H

typedef unsigned int uint;


/**
 * @brief Reverses a given string.
 *
 * @param str The string to be reversed.
 * @return A new string which is the reverse of the input.
 */
char* reverseString(const char* str);

/**
 * @brief Multiplies a base color value by a multiplier.
 *
 * @param baseColor The base color value.
 * @param multiplier The multiplier value.
 * @return The result of the multiplication.
 */
short multiply(short baseColor, float multiplier);

/**
 * @brief Caps a value at a specified maximum.
 *
 * @param value The value to be capped.
 * @param max The maximum value.
 * @return The capped value.
 */
short capAt(short value, short max);

/**
 * @brief Returns the maximum of two integers.
 *
 * @param a First integer.
 * @param b Second integer.
 * @return The greater of the two integers.
 */
int max(int a, int b);

/**
 * @brief Returns the minimum of two integers.
 *
 * @param a First integer.
 * @param b Second integer.
 * @return The smaller of the two integers.
 */
int min(int a, int b);

/**
 * @brief Calculates the length of a string.
 *
 * @param text The string whose length is to be calculated.
 * @return The length of the string.
 */
uint len(const char *text);

/**
 * @brief Counts the number of digits in an integer based on a given base.
 *
 * @param n The integer to be evaluated.
 * @param base The numerical base for the digit count.
 * @return The number of digits in the integer.
 */
uint nDigits(int n, int base);

/**
 * @brief Finds the highest power of the base that is less than or equal to the number.
 *
 * @param n The number to be evaluated.
 * @param base The base for the calculation.
 * @return The highest power of the base that is <= n.
 */
uint maxInBase(int n, int base);

/**
 * @brief Converts a number to a string with a given width.
 *
 * @param number The number to be converted.
 * @param width The width of the output string.
 * @return The string representation of the number.
 */
char* numberToString(int number, uint width);

/**
 * @brief Converts a string to an integer.
 *
 * @param str The string to be converted.
 * @return The integer representation of the string.
 */
int stringToNumber(const char *str);

/**
 * @brief Reverts the order of strings in a table.
 *
 * @param from The starting pointer of the table.
 * @param to The ending pointer of the table.
 */
void revertTable(char **from, char **to);

/**
 * @brief Joins an array of strings into a single string.
 *
 * @param buffer The buffer where the joined string will be stored.
 * @param data The array of strings to be joined.
 * @param count The number of strings in the array.
 */
void joinStrings0(char *&buffer, const char **data, int count);

/**
 * @brief Joins two strings with specified lengths into a single string.
 *
 * @param string1 The first string.
 * @param len1 The length of the first string.
 * @param string2 The second string.
 * @param len2 The length of the second string.
 * @return The joined string.
 */
char *joinStrings1(const char *string1, uint len1, const char *string2, uint len2);

/**
 * @brief Joins two strings into a single string.
 *
 * @param string1 The first string.
 * @param string2 The second string.
 * @return The joined string.
 */
char *joinStrings2(const char*string1, const char *string2);

/**
 * @brief Initializes an array of integers with a specified value.
 *
 * @param table The array to be initialized.
 * @param count The number of elements in the array.
 * @param value The value to initialize each element of the array.
 */
void initTable(int *&table, int count, int value);

/**
 * @brief Increase the size of given table.
 *
 * @param table The array to be initialized.
 * @param size Current size of the given table.
 * @param increase The value representing how many elements to add.
 */
void resizeTable(int *&table, int &size, int increase=1);

#endif //BACKGAMMONGAME_UTILITY_H
