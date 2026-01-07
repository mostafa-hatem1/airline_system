#ifndef AIRCRAFT_RECORD_HPP
#define AIRCRAFT_RECORD_HPP

#include <string>

/**
 * @brief Represents an aircraft record as stored in persistence (JSON).
 * 
 * Minimal structure for aircraft data. Aircraft identified by type.
 */
struct AircraftRecord {
    std::string aircraftID;                 // Unique identifier
    std::string aircraftType;           // e.g., "Boeing 737"
    int totalSeats;                     // e.g., 180
    std::string manufacturer;           // e.g., "Boeing"
    std::string status;                 // "Operational", "Maintenance", "Retired"
    std::string manufactureYear;       // e.g., "2015"
    std::string nextMaintenanceDate; // e.g., "2024-12-01"
    std::string lastMaintenanceDate; // e.g., "2024-06-01"
    
    AircraftRecord() = default;

    AircraftRecord(
        const std::string& type,
        int seats,
        const std::string& mfr,
        const std::string& stat
    ) : aircraftType(type), totalSeats(seats), manufacturer(mfr), status(stat) {}
};

#endif // AIRCRAFT_RECORD_HPP
