#include "Role.hpp"

std::string roleToString(Role role) {
    switch (role) {
        case Role::ADMINISTRATOR:
            return "ADMINISTRATOR";
        case Role::BOOKING_AGENT:
            return "BOOKING_AGENT";
        case Role::PASSENGER:
            return "PASSENGER";
        default:
            return "UNKNOWN";
    }
}

Role stringToRole(const std::string& str) {
    if (str == "ADMINISTRATOR") {
        return Role::ADMINISTRATOR;
    } else if (str == "BOOKING_AGENT") {
        return Role::BOOKING_AGENT;
    } else if (str == "PASSENGER") {
        return Role::PASSENGER;
    } else {
        throw std::invalid_argument("Unknown role: " + str);
    }
}
