#ifndef ROLE_HPP
#define ROLE_HPP

#include <string>
#include <stdexcept>

/**
 * @brief Enum representing user roles in the system.
 */
enum class Role {
    ADMINISTRATOR,
    BOOKING_AGENT,
    PASSENGER
};

/**
 * @brief Convert Role enum to string.
 * 
 * @param role The role to convert.
 * @return String representation (e.g., "ADMINISTRATOR").
 */
std::string roleToString(Role role);

/**
 * @brief Convert string to Role enum.
 * 
 * @param str String representation (e.g., "ADMINISTRATOR").
 * @return The corresponding Role enum.
 * @throws std::invalid_argument if string doesn't match any role.
 */
Role stringToRole(const std::string& str);

#endif // ROLE_HPP
