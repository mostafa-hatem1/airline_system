#include "ConsoleIO.hpp"
#include <algorithm>

std::string ConsoleIO::readLine(const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
        std::cout.flush();
    }

    std::string input;
    if (!std::getline(std::cin, input)) {
        throw std::runtime_error("Error reading input");
    }

    // Trim leading/trailing whitespace
    auto start = input.begin();
    while (start != input.end() && std::isspace(*start)) {
        ++start;
    }
    input.erase(input.begin(), start);

    auto end = input.rbegin();
    while (end.base() != input.begin() && std::isspace(*end)) {
        ++end;
    }
    input.erase(end.base(), input.end());

    return input;
}

int ConsoleIO::readIntInRange(const std::string& prompt, int minValue, int maxValue) {
    int choice = -1;
    while (true) {
        try {
            std::string input = readLine(prompt);
            if (input.empty()) {
                std::cerr << "Input cannot be empty. Please try again.\n";
                continue;
            }

            choice = std::stoi(input);

            if (choice < minValue || choice > maxValue) {
                std::cerr << "Please enter a number between " << minValue 
                          << " and " << maxValue << ".\n";
                continue;
            }

            return choice;
        } catch (const std::invalid_argument&) {
            std::cerr << "Invalid input. Please enter a valid integer.\n";
        } catch (const std::out_of_range&) {
            std::cerr << "Number out of range. Please try again.\n";
        }
    }
}

void ConsoleIO::pause() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleIO::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
