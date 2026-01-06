#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../persistence/UserRepository.hpp"
#include "../managers/AuthService.hpp"
#include <vector>

/**
 * @brief Main menu and role selection orchestrator.
 */
class MainMenu {
private:
    // add this private method declaration inside MainMenu:hpp
    void resetUserPasswordByUsername();


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
    std::vector<UserRecord> m_users;

    /**
     * @brief Display the role selection menu and return the chosen role.
     * 
     * @return 1 for Administrator, 2 for Booking Agent, 3 for Passenger, or 0 to exit.
     */
    int showRoleSelection();

    /**
     * @brief Handle login for the selected role.
     * 
     * @param roleChoice The role selected (1, 2, or 3).
     */
    void handleLogin(int roleChoice);

    /**
     * @brief Show the administrator menu and handle actions.
     * 
     * @param adminUser The authenticated administrator user record.
     */
    void showAdminMenu(const UserRecord& adminUser);

    /**
     * @brief Show the booking agent menu and handle actions.
     * 
     * @param agentUser The authenticated agent user record.
     */
    void showAgentMenu(const UserRecord& agentUser);

    /**
     * @brief Show the passenger menu and handle actions.
     * 
     * @param passengerUser The authenticated passenger user record.
     */
    void showPassengerMenu(const UserRecord& passengerUser);
};

#endif // MAIN_MENU_HPP
