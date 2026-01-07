#ifndef USER_MANAGEMENT_UI_HPP
#define USER_MANAGEMENT_UI_HPP

#include "../core/UserRecord.hpp"
#include "../persistence/UserRepository.hpp"
#include <vector>
#include <string>

/**
 * @brief UI for administrator user account management.
 */
class UserManagementUI {
public:
    explicit UserManagementUI(UserRepository& userRepo);

    void showUserManagementMenu();

private:
    UserRepository& m_userRepo;
    std::vector<UserRecord> m_users;

    void createUser();
    void updateUser();
    void deleteUser();
    void listAllUsers();

    std::string getNextUserID();
};

#endif // USER_MANAGEMENT_UI_H
