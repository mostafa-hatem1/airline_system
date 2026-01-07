#include "UserManagementUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include "../utils/PasswordHash.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

UserManagementUI::UserManagementUI(UserRepository& userRepo)
    : m_userRepo(userRepo) {
    m_users = m_userRepo.loadAll();
}

void UserManagementUI::showUserManagementMenu() {
    while (true) {
        std::cout << "\n--- Manage Users ---\n";
        std::cout << "1. Create User\n";
        std::cout << "2. Update User\n";
        std::cout << "3. Delete User\n";
        std::cout << "4. List All Users\n";
        std::cout << "5. Back to Admin Menu\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            createUser();
        } else if (choice == 2) {
            updateUser();
        } else if (choice == 3) {
            deleteUser();
        } else if (choice == 4) {
            listAllUsers();
        } else {
            return;
        }
    }
}

void UserManagementUI::createUser() {
    std::cout << "\n--- Create User ---\n";

    const std::string username = ConsoleIO::readLine("Enter username: ");
    if (username.empty()) {
        std::cout << "Error: Username cannot be empty.\n";
        return;
    }

    // Check duplicate
    if (std::any_of(m_users.begin(), m_users.end(),
                    [&](const UserRecord& u) { return u.username == username; })) {
        std::cout << "Error: Username already exists.\n";
        return;
    }

    const std::string password = ConsoleIO::readLine("Enter password: ");
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }

    // NEW (required by UserRecord): email + phone
    const std::string email = ConsoleIO::readLine("Enter email (optional): ");
    const std::string phone = ConsoleIO::readLine("Enter phone (optional): ");

    std::cout << "\nRole:\n";
    std::cout << "1. Administrator\n";
    std::cout << "2. Booking Agent\n";
    std::cout << "3. Passenger\n";
    const int roleChoice = ConsoleIO::readIntInRange("Select role: ", 1, 3);

    std::string role;
    if (roleChoice == 1) {
        role = "ADMINISTRATOR";
    } else if (roleChoice == 2) {
        role = "BOOKING_AGENT";
    } else {
        role = "PASSENGER";
    }

    const std::string userID = getNextUserID();
    const std::string passwordHash = PasswordHash::hash(password);

    // FIX: Use the 6-arg constructor with the correct parameter order
    // UserRecord(id, user, hash, role, email, phone)
    UserRecord newUser(userID, username, passwordHash, role, email, phone);

    m_users.push_back(newUser);

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "\nUser created successfully!\n";
        std::cout << "User ID: " << userID << "\n";
        std::cout << "Username: " << username << "\n";
        std::cout << "Role: " << role << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save user: " << e.what() << "\n";
        m_users.pop_back();
    }
}

void UserManagementUI::updateUser() {
    std::cout << "\n--- Update User ---\n";

    const std::string userID = ConsoleIO::readLine("Enter User ID to update: ");
    if (userID.empty()) {
        std::cout << "Error: User ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&](const UserRecord& u) { return u.userId == userID; });
    if (it == m_users.end()) {
        std::cout << "Error: User not found.\n";
        return;
    }

    std::cout << "Current User: " << it->username << " (" << it->role << ")\n";
    std::cout << "\nWhat do you want to update?\n";
    std::cout << "1. Password\n";
    std::cout << "2. Role\n";
    std::cout << "3. Back\n";

    const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 3);

    if (choice == 1) {
        const std::string newPassword = ConsoleIO::readLine("Enter new password: ");
        if (newPassword.empty()) {
            std::cout << "Error: Password cannot be empty.\n";
            return;
        }
        it->passwordHash = PasswordHash::hash(newPassword);
        std::cout << "Password updated.\n";
    } else if (choice == 2) {
        std::cout << "\nNew Role:\n";
        std::cout << "1. Administrator\n";
        std::cout << "2. Booking Agent\n";
        std::cout << "3. Passenger\n";
        const int roleChoice = ConsoleIO::readIntInRange("Select role: ", 1, 3);

        if (roleChoice == 1) {
            it->role = "ADMINISTRATOR";
        } else if (roleChoice == 2) {
            it->role = "BOOKING_AGENT";
        } else {
            it->role = "PASSENGER";
        }
        std::cout << "Role updated to " << it->role << ".\n";
    } else {
        return;
    }

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "User updated successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save user: " << e.what() << "\n";
    }
}

void UserManagementUI::deleteUser() {
    std::cout << "\n--- Delete User ---\n";

    const std::string userID = ConsoleIO::readLine("Enter User ID to delete: ");
    if (userID.empty()) {
        std::cout << "Error: User ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&](const UserRecord& u) { return u.userId == userID; });
    if (it == m_users.end()) {
        std::cout << "Error: User not found.\n";
        return;
    }

    std::cout << "Delete user: " << it->username << " (" << it->role << ")? (yes/no): ";
    const std::string confirm = ConsoleIO::readLine("");

    if (confirm != "yes") {
        std::cout << "Deletion aborted.\n";
        return;
    }

    m_users.erase(it);

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "User deleted successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save users: " << e.what() << "\n";
    }
}

void UserManagementUI::listAllUsers() {
    std::cout << "\n--- All Users ---\n";

    if (m_users.empty()) {
        std::cout << "No users found.\n";
        return;
    }

    std::cout << std::string(90, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "User ID"
              << std::setw(20) << "Username"
              << std::setw(20) << "Role"
              << "\n";
    std::cout << std::string(90, '-') << "\n";

    for (const auto& user : m_users) {
        std::cout << std::left
                  << std::setw(15) << user.userId
                  << std::setw(20) << user.username
                  << std::setw(20) << user.role
                  << "\n";
    }

    std::cout << std::string(90, '-') << "\n";
}

std::string UserManagementUI::getNextUserID() {
    int maxID = 0;

    for (const auto& user : m_users) {
        if (user.userId.length() > 1 && user.userId[0] == 'U') {
            try {
                int id = std::stoi(user.userId.substr(1));
                maxID = std::max(maxID, id);
            } catch (...) {
                // Ignore
            }
        }
    }

    std::ostringstream oss;
    oss << "U" << std::setfill('0') << std::setw(3) << (maxID + 1);
    return oss.str();
}
