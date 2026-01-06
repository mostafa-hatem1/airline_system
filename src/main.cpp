#include "persistence/UserRepository.hpp"
#include "ui/MainMenu.hpp"
#include <iostream>

int main() {
    try {
        UserRepository userRepo("data/users.json");
        MainMenu menu(userRepo);
        menu.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
}
