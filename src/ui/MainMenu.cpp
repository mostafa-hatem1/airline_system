#include "MainMenu.hpp"
#include "../utils/ConsoleIO.hpp"
#include "../managers/AuthService.hpp"
#include "../utils/Role.hpp"
#include "../utils/PasswordHash.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

MainMenu::MainMenu(UserRepository& userRepo)
    : m_userRepo(userRepo),
      m_flightRepo("data/flights.json"),
      m_aircraftRepo("data/aircraft.json") {
    m_users = m_userRepo.loadAll();
    m_flights = m_flightRepo.loadAll();
    m_aircraft = m_aircraftRepo.loadAll();
}

void MainMenu::run() {
    std::cout << "\n=== Welcome to Airline Reservation and Management System ===\n\n";

    while (true) {
        int roleChoice = showRoleSelection();

        if (roleChoice == 0) {
            std::cout << "Exiting system. Saving data and goodbye!\n";
            m_userRepo.saveAll(m_users);
            m_flightRepo.saveAll(m_flights);
            m_aircraftRepo.saveAll(m_aircraft);
            break;
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

    return ConsoleIO::readIntInRange("Enter choice: ", 0, 99);
}

void MainMenu::handleLogin(int roleChoice) {
    if (roleChoice == 98) {
        resetUserPasswordByUsername();
        return;
    }

    if (roleChoice == 99) {
        resetAdminPassword();
        return;
    }

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

    int attempts = 3;
    while (attempts > 0) {
        std::string username = ConsoleIO::readLine("Username: ");
        std::string password = ConsoleIO::readLine("Password: ");

        auto user = authService.login(username, password);

        if (user) {
            if (user->role != roleStr) {
                std::cout << "Error: Your account is not an " << roleStr << " account.\n";
                attempts--;
                if (attempts > 0) {
                    std::cout << "Attempts remaining: " << attempts << "\n";
                }
                continue;
            }

            std::cout << "\nLogin successful!\n\n";

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
            showManageFlightsMenu();
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

void MainMenu::showManageFlightsMenu() {
    while (true) {
        std::cout << "\n--- Manage Flights ---\n";
        std::cout << "1. Add New Flight\n";
        std::cout << "2. Update Existing Flight\n";
        std::cout << "3. Remove Flight\n";
        std::cout << "4. View All Flights\n";
        std::cout << "5. Back to Main Menu\n";

        int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            addNewFlight();
        } else if (choice == 2) {
            std::cout << "\n[Update Existing Flight - Not implemented yet]\n";
        } else if (choice == 3) {
            std::cout << "\n[Remove Flight - Not implemented yet]\n";
        } else if (choice == 4) {
            viewAllFlights();
        } else if (choice == 5) {
            break;
        }
    }
}

void MainMenu::addNewFlight() {
    std::cout << "\n--- Add New Flight ---\n";

    std::string flightNumber = ConsoleIO::readLine("Enter Flight Number: ");
    if (flightNumber.empty()) {
        std::cout << "Error: Flight number cannot be empty.\n";
        return;
    }

    // Check for duplicate
    if (m_flightRepo.findByFlightNumber(flightNumber, m_flights)) {
        std::cout << "Error: Flight " << flightNumber << " already exists.\n";
        return;
    }

    std::string origin = ConsoleIO::readLine("Enter Origin: ");
    std::string destination = ConsoleIO::readLine("Enter Destination: ");
    std::string departureDateTime = ConsoleIO::readLine("Enter Departure Date and Time (YYYY-MM-DD HHMM): ");
    std::string arrivalDateTime = ConsoleIO::readLine("Enter Arrival Date and Time (YYYY-MM-DD HHMM): ");
    std::string aircraftType = ConsoleIO::readLine("Enter Aircraft Type: ");

    int totalSeats = ConsoleIO::readIntInRange("Enter Total Seats: ", 1, 1000);

    std::cout << "Enter Status (Scheduled/Delayed/Canceled): ";
    std::string status = ConsoleIO::readLine("");
    if (status != "Scheduled" && status != "Delayed" && status != "Canceled") {
        std::cout << "Error: Status must be Scheduled, Delayed, or Canceled.\n";
        return;
    }

    // Create flight record
    FlightRecord newFlight(
        flightNumber,
        origin,
        destination,
        departureDateTime,
        arrivalDateTime,
        aircraftType,
        totalSeats,
        status
    );

    m_flights.push_back(newFlight);

    try {
        m_flightRepo.saveAll(m_flights);
        std::cout << "\nFlight " << flightNumber << " has been successfully added to the schedule.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save flight: " << e.what() << "\n";
        m_flights.pop_back();  // Rollback
    }
}

void MainMenu::viewAllFlights() {
    if (m_flights.empty()) {
        std::cout << "\nNo flights scheduled.\n";
        return;
    }

    std::cout << "\n--- All Flights ---\n";
    std::cout << std::string(130, '-') << "\n";
    std::cout << std::left
              << std::setw(12) << "Flight #"
              << std::setw(20) << "Origin"
              << std::setw(20) << "Destination"
              << std::setw(18) << "Departure"
              << std::setw(18) << "Arrival"
              << std::setw(15) << "Aircraft"
              << std::setw(8) << "Seats"
              << std::setw(12) << "Status"
              << "\n";
    std::cout << std::string(130, '-') << "\n";

    for (const auto& flight : m_flights) {
        std::cout << std::left
                  << std::setw(12) << flight.flightNumber
                  << std::setw(20) << flight.origin
                  << std::setw(20) << flight.destination
                  << std::setw(18) << flight.departureDateTime
                  << std::setw(18) << flight.arrivalDateTime
                  << std::setw(15) << flight.aircraftType
                  << std::setw(8) << flight.totalSeats
                  << std::setw(12) << flight.status
                  << "\n";
    }

    std::cout << std::string(130, '-') << "\n";
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

void MainMenu::resetAdminPassword() {
    std::cout << "\n--- Utility: Reset Admin Password ---\n";

    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [](const UserRecord& u) {
                               return u.role == "ADMINISTRATOR" && u.username == "adminUser";
                           });

    if (it == m_users.end()) {
        it = std::find_if(m_users.begin(), m_users.end(),
                          [](const UserRecord& u) {
                              return u.role == "ADMINISTRATOR";
                          });
    }

    if (it == m_users.end()) {
        std::cout << "Error: No administrator account found in users.json\n";
        return;
    }

    std::cout << "Admin account found: userId=" << it->userId
              << ", username=" << it->username << "\n";

    const std::string newPw = ConsoleIO::readLine("Enter new admin password (plain text): ");
    if (newPw.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }

    it->passwordHash = PasswordHash::hash(newPw);

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "Success: Admin password has been reset and saved.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save users.json: " << e.what() << "\n";
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
