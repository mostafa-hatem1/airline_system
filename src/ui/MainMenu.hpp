#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "../persistence/UserRepository.hpp"
#include "../persistence/FlightRepository.hpp"
#include "../persistence/AircraftRepository.hpp"
#include <vector>

/**
 * @brief Main menu and role selection orchestrator.
 */
class MainMenu {
public:
    /**
     * @brief Constructor.
     * 
     * @param userRepo The user repository for persistence.
     */
    explicit MainMenu(UserRepository& userRepo);

    /**
     * @brief Run the main menu loop (role selection → login → role menu → logout).
     */
    void run();

private:
    UserRepository& m_userRepo;
    FlightRepository m_flightRepo;
    AircraftRepository m_aircraftRepo;
    
    std::vector<UserRecord> m_users;
    std::vector<FlightRecord> m_flights;
    std::vector<AircraftRecord> m_aircraft;

    int showRoleSelection();
    void handleLogin(int roleChoice);
    void showAdminMenu(const UserRecord& adminUser);
    void showAgentMenu(const UserRecord& agentUser);
    void showPassengerMenu(const UserRecord& passengerUser);

    // Admin sub-menus
    void showManageFlightsMenu();
    void addNewFlight();
    void viewAllFlights();

    // Utility
    void resetAdminPassword();
    void resetUserPasswordByUsername();
};

#endif // MAIN_MENU_H
