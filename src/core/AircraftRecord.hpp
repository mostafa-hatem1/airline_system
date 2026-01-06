#ifndef AIRCRAFT_RECORD_HPP
#define AIRCRAFT_RECORD_HPP

#include <string>

/**
 * @brief Represents an aircraft record as stored in persistence (JSON).
 * 
 * Minimal structure for aircraft data. Aircraft identified by type.
 */
struct AircraftRecord {
    std::string aircraftType;           // e.g., "Boeing 737"
    int totalSeats;                     // e.g., 180
    std::string manufacturer;           // e.g., "Boeing"
    std::string status;                 // "Operational", "Maintenance", "Retired"

    AircraftRecord() = default;

    AircraftRecord(
        const std::string& type,
        int seats,
        const std::string& mfr,
        const std::string& stat
    ) : aircraftType(type), totalSeats(seats), manufacturer(mfr), status(stat) {}
};

#endif // AIRCRAFT_RECORD_HPP
