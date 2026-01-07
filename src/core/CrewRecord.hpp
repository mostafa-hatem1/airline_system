#ifndef CREW_RECORD_HPP
#define CREW_RECORD_HPP

#include <string>

/**
 * @brief Represents a crew member (pilot or flight attendant).
 */
struct CrewRecord {
    std::string crewID;                 // e.g., "PL001", "FA001"
    std::string name;                   // e.g., "Captain Sarah Connor"
    std::string role;                   // "Pilot" or "Attendant"
    double maxFlightHours;              // Max hours per month (e.g., 100.0)
    double currentFlightHours;          // Hours already worked this month
    std::string status;                 // "Available", "OnDuty", "Rest", "OnLeave"
    std::string lastAssignment;         // Last flight number assigned

    CrewRecord() = default;

    CrewRecord(
        const std::string& id,
        const std::string& n,
        const std::string& r,
        double maxHours,
        double currHours,
        const std::string& stat,
        const std::string& lastAssign = ""
    ) : crewID(id), name(n), role(r), maxFlightHours(maxHours),
        currentFlightHours(currHours), status(stat), lastAssignment(lastAssign) {}
};

#endif // CREW_RECORD_HPP
