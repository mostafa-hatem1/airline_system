#ifndef USER_RECORD_HPP
#define USER_RECORD_HPP

#include <string>

/**
 * @brief Represents a user record as stored in persistence (JSON/CSV).
 * 
 * This is a simple data structure that can be serialized to/from JSON.
 * It does NOT contain business logic—just data fields needed for persistence.
 * 
 * Later, we'll wrap this in proper User/Administrator/BookingAgent/Passenger
 * classes with authentication and role-based access.
 */
struct UserRecord {
    std::string userId;           // e.g., "ADM001", "P456", "BA001"
    std::string username;         // login username
    std::string passwordHash;     // hashed password (not plaintext)
    std::string role;             // "ADMINISTRATOR", "BOOKING_AGENT", or "PASSENGER"
    std::string email;            // contact email
    std::string phone;            // contact phone

    /**
     * @brief Default constructor—initializes empty record.
     */
    UserRecord() = default;

    /**
     * @brief Full constructor for convenience.
     */
    UserRecord(
        const std::string& id,
        const std::string& user,
        const std::string& hash,
        const std::string& r,
        const std::string& e,
        const std::string& p
    ) : userId(id), username(user), passwordHash(hash), role(r), email(e), phone(p) {}
};

#endif // USER_RECORD_HPP
