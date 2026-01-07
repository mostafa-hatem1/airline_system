#ifndef FLIGHT_CREW_RECORD_HPP
#define FLIGHT_CREW_RECORD_HPP

#include <string>
#include <vector>

/**
 * @brief Maps crew members to a specific flight.
 */
struct FlightCrewRecord {
    std::string flightNumber;           // e.g., "AA123"
    std::string pilotID;                // e.g., "PL001"
    std::vector<std::string> attendantIDs;  // e.g., ["FA001", "FA002", "FA003"]
    std::string assignmentDate;         // e.g., "2023-12-15"

    FlightCrewRecord() = default;

    FlightCrewRecord(
        const std::string& flight,
        const std::string& pilot,
        const std::vector<std::string>& attendants = {},
        const std::string& assignDate = ""
    ) : flightNumber(flight), pilotID(pilot), attendantIDs(attendants),
        assignmentDate(assignDate) {}
};

#endif // FLIGHT_CREW_RECORD_HPP
