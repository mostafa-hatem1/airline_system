#include "MainMenu.hpp"

#include "../ui/BookingUI.hpp"
#include "../ui/ReservationUI.hpp"

#include "../utils/ConsoleIO.hpp"
#include "../utils/PasswordHash.hpp"

#include <algorithm>
#include <exception>
#include <iomanip>
#include <iostream>

MainMenu::MainMenu(UserRepository& userRepo)
    : m_userRepo(userRepo),
      m_flightRepo("data/flights.json"),
      m_aircraftRepo("data/aircraft.json"),
      m_reservationRepo("data/reservations.json"),
      m_crewRepo("data/crew.json"),
      m_maintenanceRepo("data/maintenance.json"),
      m_flightCrewRepo("data/flight_crew.json") {
    // Load everything
    m_users = m_userRepo.loadAll();
    m_flights = m_flightRepo.loadAll();
    m_aircraft = m_aircraftRepo.loadAll();
    m_reservations = m_reservationRepo.loadAll();
    m_crew = m_crewRepo.loadAll();
    m_maintenance = m_maintenanceRepo.loadAll();
    m_flightCrew = m_flightCrewRepo.loadAll();

    // Initialize seat maps based on existing reservations
    m_bookingService.initializeSeats(m_flights, m_reservations);
}

void MainMenu::run() {
    std::cout << "\n=== Welcome to Airline Reservation and Management System ===\n\n";

    while (true) {
        const int roleChoice = showRoleSelection();

        if (roleChoice == 0) {
            std::cout << "Exiting system. Saving data and goodbye!\n";
            saveAll();
            return;
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

    // Hidden utilities:
    // 98 = reset any user password by username
    // 99 = reset admin password
    return ConsoleIO::readIntInRange("Enter choice: ", 0, 99);
}

void MainMenu::handleLogin(int roleChoice) {
    if (roleChoice == 98) {
        resetUserPasswordByUsername();
        return;
    }

    std::string roleStr;
    std::string roleLabel;

    if (roleChoice == 1) {
        roleStr = "ADMINISTRATOR";
        roleLabel = "Administrator";
    } else if (roleChoice == 2) {
        roleStr = "BOOKING_AGENT";
        roleLabel = "Booking Agent";
    } else if (roleChoice == 3) {
        roleStr = "PASSENGER";
        roleLabel = "Passenger";
    } else {
        return;
    }

    std::cout << "\n--- " << roleLabel << " Login ---\n";

    AuthService authService(m_users);

    int attempts = 3;
    while (attempts-- > 0) {
        const std::string username = ConsoleIO::readLine("Username: ");
        const std::string password = ConsoleIO::readLine("Password: ");

        auto userOpt = authService.login(username, password);

        if (!userOpt) {
            if (attempts > 0) {
                std::cout << "Error: Invalid username or password. Attempts remaining: " << attempts << "\n";
            } else {
                std::cout << "Error: Login failed. Maximum attempts exceeded.\n";
            }
            continue;
        }

        const UserRecord user = *userOpt;

        if (user.role != roleStr) {
            if (attempts > 0) {
                std::cout << "Error: Account role mismatch. Attempts remaining: " << attempts << "\n";
            } else {
                std::cout << "Error: Login failed. Maximum attempts exceeded.\n";
            }
            continue;
        }

        std::cout << "\nLogin successful!\n\n";

        if (roleChoice == 1) {
            showAdminMenu(user);
        } else if (roleChoice == 2) {
            showAgentMenu(user);
        } else {
            showPassengerMenu(user);
        }

        return; // back to role selection after logout
    }
}

void MainMenu::showAdminMenu(const UserRecord& adminUser) {
    std::cout << "--- Administrator Menu ---\n";

    while (true) {
        std::cout << "\n1. Manage Flights\n";
        std::cout << "2. Manage Aircraft\n";
        std::cout << "3. Manage Users\n";
        std::cout << "4. Flight Operations\n";
        std::cout << "5. Generate Reports\n";
        std::cout << "6. Logout\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 6);

        if (choice == 1) {
            showManageFlightsMenu();
        } else if (choice == 2) {
            AircraftManagementUI aircraftMgmt(m_aircraftRepo);
            aircraftMgmt.showAircraftManagementMenu();
            m_aircraft = m_aircraftRepo.loadAll();
        } else if (choice == 3) {
            UserManagementUI userMgmt(m_userRepo);
            userMgmt.showUserManagementMenu();
            m_users = m_userRepo.loadAll();  // Reload after changes
        } else if (choice == 4) {
            FlightOperationsUI flightOps(
                m_flights, m_crew, m_maintenance, m_flightCrew, m_aircraft,
                m_flightRepo, m_crewRepo, m_maintenanceRepo, m_flightCrewRepo, m_aircraftRepo
            );
            flightOps.showFlightOperationsMenu();
            m_crew = m_crewRepo.loadAll();
            m_maintenance = m_maintenanceRepo.loadAll();
            m_flightCrew = m_flightCrewRepo.loadAll();
        } else if (choice == 5) {
            ReportingUI reporting(m_flights, m_reservations, m_maintenance, m_users);
            reporting.showReportingMenu();
        } else {
            std::cout << "\nLogging out...\n";
            return;
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

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            addNewFlight();
        } else if (choice == 2) {
            std::cout << "\n[Update Existing Flight - Not implemented yet]\n";
        } else if (choice == 3) {
            std::cout << "\n[Remove Flight - Not implemented yet]\n";
        } else if (choice == 4) {
            viewAllFlights();
        } else {
            return;
        }
    }
}

void MainMenu::addNewFlight() {
    std::cout << "\n--- Add New Flight ---\n";

    const std::string flightNumber = ConsoleIO::readLine("Enter Flight Number: ");
    if (flightNumber.empty()) {
        std::cout << "Error: Flight number cannot be empty.\n";
        return;
    }

    const bool exists = std::any_of(m_flights.begin(), m_flights.end(),
                                   [&](const FlightRecord& f) {
                                       return f.flightNumber == flightNumber;
                                   });
    if (exists) {
        std::cout << "Error: Flight " << flightNumber << " already exists.\n";
        return;
    }

    const std::string origin = ConsoleIO::readLine("Enter Origin: ");
    const std::string destination = ConsoleIO::readLine("Enter Destination: ");
    const std::string departureDateTime =
        ConsoleIO::readLine("Enter Departure Date and Time (YYYY-MM-DD HHMM): ");
    const std::string arrivalDateTime =
        ConsoleIO::readLine("Enter Arrival Date and Time (YYYY-MM-DD HHMM): ");
    const std::string aircraftType = ConsoleIO::readLine("Enter Aircraft Type: ");

    const int totalSeats = ConsoleIO::readIntInRange("Enter Total Seats: ", 1, 1000);

    const std::string status = ConsoleIO::readLine("Enter Status (Scheduled/Delayed/Canceled): ");
    if (status != "Scheduled" && status != "Delayed" && status != "Canceled") {
        std::cout << "Error: Status must be Scheduled, Delayed, or Canceled.\n";
        return;
    }

    // Keep price simple; can be edited later or auto-derived.
    // If your FlightRecord requires price, prompt it here.
    double price = 250.00;
    try {
        const std::string priceStr = ConsoleIO::readLine("Enter Price (e.g., 350.00): ");
        if (!priceStr.empty()) {
            price = std::stod(priceStr);
            if (price < 0.0) {
                std::cout << "Error: Price must be non-negative.\n";
                return;
            }
        }
    } catch (...) {
        std::cout << "Error: Invalid price.\n";
        return;
    }

    FlightRecord newFlight;
    newFlight.flightNumber = flightNumber;
    newFlight.origin = origin;
    newFlight.destination = destination;
    newFlight.departureDateTime = departureDateTime;
    newFlight.arrivalDateTime = arrivalDateTime;
    newFlight.aircraftType = aircraftType;
    newFlight.totalSeats = totalSeats;
    newFlight.status = status;
    newFlight.price = price;

    m_flights.push_back(newFlight);

    try {
        m_flightRepo.saveAll(m_flights);

        // IMPORTANT: new flight needs a seatmap entry
        m_bookingService.initializeSeats(m_flights, m_reservations);

        std::cout << "\nFlight " << flightNumber << " has been successfully added to the schedule.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save flight: " << e.what() << "\n";
        m_flights.pop_back();
    }
}

void MainMenu::viewAllFlights() {
    if (m_flights.empty()) {
        std::cout << "\nNo flights scheduled.\n";
        return;
    }

    std::cout << "\n--- All Flights ---\n";
    std::cout << std::string(150, '-') << "\n";
    std::cout << std::left
              << std::setw(12) << "Flight #"
              << std::setw(20) << "Origin"
              << std::setw(20) << "Destination"
              << std::setw(18) << "Departure"
              << std::setw(18) << "Arrival"
              << std::setw(15) << "Aircraft"
              << std::setw(8) << "Seats"
              << std::setw(12) << "Status"
              << std::setw(10) << "Price"
              << "\n";
    std::cout << std::string(150, '-') << "\n";

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
                  << std::setw(10) << std::fixed << std::setprecision(2) << flight.price
                  << "\n";
    }

    std::cout << std::string(150, '-') << "\n";
}

static void printFlightsTable(const std::vector<FlightRecord>& flights) {
    if (flights.empty()) {
        std::cout << "No flights found matching your criteria.\n";
        return;
    }

    std::cout << "\nFlights\n";
    std::cout << std::string(140, '-') << "\n";
    std::cout << std::left
              << std::setw(3)  << "#"
              << std::setw(12) << "Flight"
              << std::setw(20) << "Origin"
              << std::setw(20) << "Destination"
              << std::setw(18) << "Departure"
              << std::setw(18) << "Arrival"
              << std::setw(15) << "Aircraft"
              << std::setw(10) << "Seats"
              << std::setw(10) << "Price"
              << std::setw(12) << "Status"
              << "\n";
    std::cout << std::string(140, '-') << "\n";

    int idx = 1;
    for (const auto& f : flights) {
        std::cout << std::left
                  << std::setw(3)  << idx++
                  << std::setw(12) << f.flightNumber
                  << std::setw(20) << f.origin
                  << std::setw(20) << f.destination
                  << std::setw(18) << f.departureDateTime
                  << std::setw(18) << f.arrivalDateTime
                  << std::setw(15) << f.aircraftType
                  << std::setw(10) << f.totalSeats
                  << std::setw(10) << std::fixed << std::setprecision(2) << f.price
                  << std::setw(12) << f.status
                  << "\n";
    }
    std::cout << std::string(140, '-') << "\n";
}

void MainMenu::searchFlightsUI() {
    std::cout << "\n--- Search Flights ---\n";
    const std::string origin = ConsoleIO::readLine("Enter Origin: ");
    const std::string destination = ConsoleIO::readLine("Enter Destination: ");
    const std::string departureDate = ConsoleIO::readLine("Enter Departure Date (YYYY-MM-DD): ");

    const auto results = FlightSearchService::searchFlights(origin, destination, departureDate, m_flights);
    printFlightsTable(results);
}

void MainMenu::showAgentMenu(const UserRecord& agentUser) {
    std::cout << "--- Booking Agent Menu ---\n";

    while (true) {
        std::cout << "\n1. Search Flights\n";
        std::cout << "2. Book a Flight\n";
        std::cout << "3. Modify Reservation\n";
        std::cout << "4. Cancel Reservation\n";
        std::cout << "5. Logout\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            searchFlightsUI();
        } else if (choice == 2) {
            runBookingFlow(agentUser);
        } else if (choice == 3) {
            ReservationUI resUI(m_flights, m_reservations, m_flightRepo, m_reservationRepo, m_bookingService);
            resUI.modifyReservation();
        } else if (choice == 4) {
            ReservationUI resUI(m_flights, m_reservations, m_flightRepo, m_reservationRepo, m_bookingService);
            resUI.cancelReservation();
        } else {
            std::cout << "\nLogging out...\n";
            return;
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

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 4);

        if (choice == 1) {
            runBookingFlow(passengerUser);
        } else if (choice == 2) {
            ReservationUI resUI(m_flights, m_reservations, m_flightRepo, m_reservationRepo, m_bookingService);
            resUI.showMyReservations(passengerUser);
        } else if (choice == 3) {
            ReservationUI resUI(m_flights, m_reservations, m_flightRepo, m_reservationRepo, m_bookingService);
            resUI.checkIn(passengerUser);
        } else {
            std::cout << "\nLogging out...\n";
            return;
        }
    }
}

void MainMenu::runBookingFlow(const UserRecord& user) {
    // Make sure seat maps reflect latest flights/reservations
    m_bookingService.initializeSeats(m_flights, m_reservations);

    BookingUI bookingUI(m_users,                   
        m_flights, 
        m_reservations,
        m_flightRepo, 
        m_reservationRepo, 
        m_bookingService);


    bookingUI.runBookingFlow(user);

    // After booking flow, reservations may have changed; persist them.
    // BookingUI might already save, but keeping this ensures state is consistent.
    try {
        m_reservationRepo.saveAll(m_reservations);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save reservations: " << e.what() << "\n";
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
                          [](const UserRecord& u) { return u.role == "ADMINISTRATOR"; });
    }

    if (it == m_users.end()) {
        std::cout << "Error: No administrator account found in users.json\n";
        return;
    }

    std::cout << "Admin found: userId=" << it->userId << ", username=" << it->username << "\n";

    const std::string newPw = ConsoleIO::readLine("Enter new admin password (plain text): ");
    if (newPw.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }

    it->passwordHash = PasswordHash::hash(newPw);

    try {
        m_userRepo.saveAll(m_users);
        std::cout << "Success: Admin password reset and saved.\n";
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

    std::cout << "User found: userId=" << it->userId << ", role=" << it->role << "\n";

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

void MainMenu::saveAll() {
    try {
        m_userRepo.saveAll(m_users);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save users: " << e.what() << "\n";
    }

    try {
        m_flightRepo.saveAll(m_flights);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save flights: " << e.what() << "\n";
    }

    try {
        m_aircraftRepo.saveAll(m_aircraft);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save aircraft: " << e.what() << "\n";
    }

    try {
        m_reservationRepo.saveAll(m_reservations);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save reservations: " << e.what() << "\n";
    }

    try {
        m_crewRepo.saveAll(m_crew);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save crew: " << e.what() << "\n";
    }

    try {
        m_maintenanceRepo.saveAll(m_maintenance);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save maintenance: " << e.what() << "\n";
    }

    try {
        m_flightCrewRepo.saveAll(m_flightCrew);
    } catch (const std::exception& e) {
        std::cout << "Warning: Failed to save flight crew: " << e.what() << "\n";
    }
}
