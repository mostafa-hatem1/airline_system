#ifndef MAINTENANCE_RECORD_HPP
#define MAINTENANCE_RECORD_HPP

#include <string>

/**
 * @brief Represents a maintenance activity for an aircraft.
 */
struct MaintenanceRecord {
    std::string maintenanceID;          // e.g., "M001", "M002"
    std::string aircraftType;           // e.g., "Boeing 737"
    std::string startDate;              // e.g., "2023-12-15"
    std::string endDate;                // e.g., "2023-12-20" (empty if ongoing)
    std::string maintenanceType;        // "Scheduled", "Emergency", "Inspection"
    std::string description;            // e.g., "Engine overhaul"
    std::string status;                 // "Scheduled", "InProgress", "Completed"
    double estimatedCost;               // e.g., 50000.00

    MaintenanceRecord() = default;

    MaintenanceRecord(
        const std::string& id,
        const std::string& aircraft,
        const std::string& startDt,
        const std::string& endDt,
        const std::string& type,
        const std::string& desc,
        const std::string& stat,
        double cost = 0.0
    ) : maintenanceID(id), aircraftType(aircraft), startDate(startDt),
        endDate(endDt), maintenanceType(type), description(desc), 
        status(stat), estimatedCost(cost) {}
};

#endif // MAINTENANCE_RECORD_HPP
