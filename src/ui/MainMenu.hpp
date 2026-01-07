#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../core/UserRecord.hpp"
#include "../core/FlightRecord.hpp"
#include "../core/AircraftRecord.hpp"
#include "../core/ReservationRecord.hpp"
#include "../core/CrewRecord.hpp"
#include "../core/MaintenanceRecord.hpp"
#include "../core/FlightCrewRecord.hpp"

#include "../persistence/UserRepository.hpp"
#include "../persistence/FlightRepository.hpp"
#include "../persistence/AircraftRepository.hpp"
#include "../persistence/ReservationRepository.hpp"
#include "../persistence/CrewRepository.hpp"
#include "../persistence/MaintenanceRepository.hpp"
#include "../persistence/FlightCrewRepository.hpp"

#include "../ui/ReservationUI.hpp"
#include "../ui/UserManagementUI.hpp"
#include "../ui/FlightOperationsUI.hpp"
#include "../ui/ReportingUI.hpp"
#include "../ui/AircraftManagementUI.hpp"



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
    CrewRepository m_crewRepo;
    MaintenanceRepository m_maintenanceRepo;
    FlightCrewRepository m_flightCrewRepo;

    // In-memory data
    std::vector<UserRecord> m_users;
    std::vector<FlightRecord> m_flights;
    std::vector<AircraftRecord> m_aircraft;
    std::vector<ReservationRecord> m_reservations;
    std::vector<CrewRecord> m_crew;
    std::vector<MaintenanceRecord> m_maintenance;
    std::vector<FlightCrewRecord> m_flightCrew;

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

#endif // MAIN_MENU_HPP
