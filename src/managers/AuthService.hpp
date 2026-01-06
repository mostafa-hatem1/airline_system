#ifndef AUTH_SERVICE_HPP
#define AUTH_SERVICE_HPP

#include "../core/UserRecord.hpp"
#include <vector>
#include <optional>

/**
 * @brief Service for user authentication.
 * 
 * Validates credentials against stored user records.
 */
class AuthService {
public:
    /**
     * @brief Constructor.
     * 
     * @param users Vector of user records to authenticate against.
     */
    explicit AuthService(const std::vector<UserRecord>& users);

    /**
     * @brief Authenticate a user by username and password.
     * 
     * @param username The username to authenticate.
     * @param password The plaintext password to verify.
     * @return The authenticated UserRecord if credentials are valid; 
     *         std::nullopt otherwise.
     */
    std::optional<UserRecord> login(const std::string& username, const std::string& password);

private:
    const std::vector<UserRecord>& m_users;
};

#endif // AUTH_SERVICE_HPP
