#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../core/UserRecord.hpp"
#include "../core/FlightRecord.hpp"
#include "../core/AircraftRecord.hpp"
#include "../core/ReservationRecord.hpp"

#include "../persistence/UserRepository.hpp"
#include "../persistence/FlightRepository.hpp"
#include "../persistence/AircraftRepository.hpp"
#include "../persistence/ReservationRepository.hpp"

#include "../managers/AuthService.hpp"
#include "../managers/BookingService.hpp"

#include <vector>
#include <string>

class MainMenu {
public:
    explicit MainMenu(UserRepository& userRepo);

    void run();

private:
    // Persistence
    UserRepository& m_userRepo;
    FlightRepository m_flightRepo;
    AircraftRepository m_aircraftRepo;
    ReservationRepository m_reservationRepo;

    // In-memory data
    std::vector<UserRecord> m_users;
    std::vector<FlightRecord> m_flights;
    std::vector<AircraftRecord> m_aircraft;
    std::vector<ReservationRecord> m_reservations;

    // Services
    BookingService m_bookingService;

    // Main flow
    int showRoleSelection();
    void handleLogin(int roleChoice);

    // Role menus
    void showAdminMenu(const UserRecord& adminUser);
    void showAgentMenu(const UserRecord& agentUser);
    void showPassengerMenu(const UserRecord& passengerUser);

    // Admin: Flight management (implemented in previous phase)
    void showManageFlightsMenu();
    void addNewFlight();
    void viewAllFlights();

    // Booking flow entry points
    void runBookingFlow(const UserRecord& user);

    // Utility commands (hidden)
    void resetAdminPassword();
    void resetUserPasswordByUsername();

    // Save everything
    void saveAll();
};

#endif // MAIN_MENU_H
