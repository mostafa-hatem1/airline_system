#include "MainMenu.hpp"
#include "../utils/ConsoleIO.hpp"
#include "../utils/PasswordHash.hpp"
#include "../utils/Role.hpp"
#include <algorithm>
#include <iostream>

MainMenu::MainMenu(UserRepository& userRepo)
    : m_userRepo(userRepo) {
    m_users = m_userRepo.loadAll();
}

void MainMenu::run() {
    std::cout << "\n=== Welcome to Airline Reservation and Management System ===\n\n";

    while (true) {
        int roleChoice = showRoleSelection();

        if (roleChoice == 0) {
            std::cout << "Exiting system. Goodbye!\n";
            // Save any changes before exit
            m_userRepo.saveAll(m_users);
            break;
        }
        if (roleChoice == 98) {
            resetUserPasswordByUsername();
            continue;
        }



        handleLogin(roleChoice);
    }
}

int MainMenu::showRoleSelection() {
    std::cout << "\nPlease select your role:\n";
    std::cout << "1. Administrator\n";
    std::cout << "2. Booking Agent\n";
    std::cout << "3. Passenger\n";
    std::cout << "0. Exit\n";

    // hidden utility command (don’t display)
    return ConsoleIO::readIntInRange("Enter choice: ", 0, 99);
}

void MainMenu::handleLogin(int roleChoice) {
    std::string roleStr;
    if (roleChoice == 1) {
        roleStr = "ADMINISTRATOR";
    } else if (roleChoice == 2) {
        roleStr = "BOOKING_AGENT";
    } else if (roleChoice == 3) {
        roleStr = "PASSENGER";
    } else {
        return;
    }

    std::cout << "\n--- " << (roleChoice == 1 ? "Administrator" 
                           : roleChoice == 2 ? "Booking Agent" 
                           : "Passenger") << " Login ---\n";

    AuthService authService(m_users);

    // Retry loop: allow user to retry login 3 times
    int attempts = 3;
    while (attempts > 0) {
        std::string username = ConsoleIO::readLine("Username: ");
        std::string password = ConsoleIO::readLine("Password: ");

        auto user = authService.login(username, password);

        if (user) {
            // Verify the user has the correct role for this login flow
            if (user->role != roleStr) {
                std::cout << "Error: Your account is not an " << roleStr << " account.\n";
                attempts--;
                if (attempts > 0) {
                    std::cout << "Attempts remaining: " << attempts << "\n";
                }
                continue;
            }

            std::cout << "\nLogin successful!\n\n";

            // Route to appropriate menu
            if (roleChoice == 1) {
                showAdminMenu(user.value());
            } else if (roleChoice == 2) {
                showAgentMenu(user.value());
            } else if (roleChoice == 3) {
                showPassengerMenu(user.value());
            }

            return;
        } else {
            attempts--;
            if (attempts > 0) {
                std::cout << "Error: Invalid username or password. Attempts remaining: " << attempts << "\n";
            } else {
                std::cout << "Error: Login failed. Maximum attempts exceeded.\n";
            }
        }
    }
}

void MainMenu::showAdminMenu(const UserRecord& adminUser) {
    std::cout << "--- Administrator Menu ---\n";

    while (true) {
        std::cout << "\n1. Manage Flights\n";
        std::cout << "2. Manage Aircraft\n";
        std::cout << "3. Manage Users\n";
        std::cout << "4. Generate Reports\n";
        std::cout << "5. Logout\n";

        int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            std::cout << "\n[Manage Flights - Not implemented yet]\n";
        } else if (choice == 2) {
            std::cout << "\n[Manage Aircraft - Not implemented yet]\n";
        } else if (choice == 3) {
            std::cout << "\n[Manage Users - Not implemented yet]\n";
        } else if (choice == 4) {
            std::cout << "\n[Generate Reports - Not implemented yet]\n";
        } else if (choice == 5) {
            std::cout << "\nLogging out...\n";
            break;
        }
    }
}

void MainMenu::showAgentMenu(const UserRecord& agentUser) {
    std::cout << "--- Booking Agent Menu ---\n";

    while (true) {
        std::cout << "\n1. Search Flights\n";
        std::cout << "2. Book a Flight\n";
        std::cout << "3. Modify Reservation\n";
        std::cout << "4. Cancel Reservation\n";
        std::cout << "5. Logout\n";

        int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            std::cout << "\n[Search Flights - Not implemented yet]\n";
        } else if (choice == 2) {
            std::cout << "\n[Book a Flight - Not implemented yet]\n";
        } else if (choice == 3) {
            std::cout << "\n[Modify Reservation - Not implemented yet]\n";
        } else if (choice == 4) {
            std::cout << "\n[Cancel Reservation - Not implemented yet]\n";
        } else if (choice == 5) {
            std::cout << "\nLogging out...\n";
            break;
        }
    }
}

void MainMenu::showPassengerMenu(const UserRecord& passengerUser) {
    std::cout << "--- Passenger Menu ---\n";

    while (true) {
        std::cout << "\n1. Search Flights\n";
        std::cout << "2. View My Reservations\n";
        std::cout << "3. Check-In\n";
        std::cout << "4. Logout\n";

        int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 4);

        if (choice == 1) {
            std::cout << "\n[Search Flights - Not implemented yet]\n";
        } else if (choice == 2) {
            std::cout << "\n[View My Reservations - Not implemented yet]\n";
        } else if (choice == 3) {
            std::cout << "\n[Check-In - Not implemented yet]\n";
        } else if (choice == 4) {
            std::cout << "\nLogging out...\n";
            break;
        }
    }
}

void MainMenu::resetUserPasswordByUsername() {
    std::cout << "\n--- Utility: Reset User Password ---\n";

    const std::string username = ConsoleIO::readLine("Enter username to reset: ");
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&](const UserRecord& u) { return u.username == username; });

    if (it == m_users.end()) {
        std::cout << "Error: No user found with username '" << username << "'.\n";
        return;
    }

    std::cout << "User found: userId=" << it->userId
              << ", role=" << it->role << "\n";

    const std::string newPw = ConsoleIO::readLine("Enter new password (plain text): ");
    if (newPw.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }

    it->passwordHash = PasswordHash::hash(newPw);

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "Success: Password reset and saved.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save users.json: " << e.what() << "\n";
    }
}
