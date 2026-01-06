#include "AuthService.hpp"
#include "../utils/PasswordHash.hpp"

AuthService::AuthService(const std::vector<UserRecord>& users)
    : m_users(users) {
}

std::optional<UserRecord> AuthService::login(const std::string& username, const std::string& password) {
    // Find user by username
    for (const auto& user : m_users) {
        if (user.username == username) {
            // Verify password
            if (PasswordHash::verify(password, user.passwordHash)) {
                return user;
            } else {
                // Password mismatch (don't reveal which users exist for security)
                return std::nullopt;
            }
        }
    }

    // User not found
    return std::nullopt;
}
