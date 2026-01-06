#ifndef CONSOLE_IO_HPP
#define CONSOLE_IO_HPP

#include <string>
#include <iostream>
#include <limits>

/**
 * @brief Utility functions for safe console I/O.
 */
namespace ConsoleIO {

/**
 * @brief Read a line of text from standard input.
 * 
 * @param prompt The prompt to display to the user.
 * @return The trimmed input line.
 */
std::string readLine(const std::string& prompt = "");

/**
 * @brief Read an integer from standard input with validation.
 * 
 * @param prompt The prompt to display.
 * @param minValue Minimum allowed value (inclusive).
 * @param maxValue Maximum allowed value (inclusive).
 * @return A valid integer within the range.
 */
int readIntInRange(const std::string& prompt, int minValue, int maxValue);

/**
 * @brief Pause and wait for user to press Enter.
 */
void pause();

/**
 * @brief Clear the console screen (platform-specific).
 */
void clearScreen();

} // namespace ConsoleIO

#endif // CONSOLE_IO_HPP
