#ifndef MAINTENANCE_REPOSITORY_HPP
#define MAINTENANCE_REPOSITORY_HPP

#include "../core/MaintenanceRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting maintenance records to/from JSON.
 */
class MaintenanceRepository {
public:
    explicit MaintenanceRepository(const std::string& dataFilePath = "data/maintenance.json");

    std::vector<MaintenanceRecord> loadAll();
    void saveAll(const std::vector<MaintenanceRecord>& maintenance);

    std::optional<MaintenanceRecord> findByID(
        const std::string& maintenanceID,
        const std::vector<MaintenanceRecord>& maintenance
    );

    std::vector<MaintenanceRecord> findByAircraftType(
        const std::string& aircraftType,
        const std::vector<MaintenanceRecord>& maintenance
    );

    std::vector<MaintenanceRecord> findByStatus(
        const std::string& status,
        const std::vector<MaintenanceRecord>& maintenance
    );

private:
    std::string m_dataFilePath;
};

#endif // MAINTENANCE_REPOSITORY_HPP
