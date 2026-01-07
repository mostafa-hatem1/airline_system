#include "MaintenanceRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

MaintenanceRepository::MaintenanceRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<MaintenanceRecord> MaintenanceRepository::loadAll() {
    std::vector<MaintenanceRecord> maintenance;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Maintenance file does not exist: " << m_dataFilePath << "\n";
        return maintenance;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("maintenance") || !data["maintenance"].is_array()) {
            throw std::runtime_error("Invalid maintenance JSON structure");
        }

        for (const auto& mJson : data["maintenance"]) {
            MaintenanceRecord m;
            m.maintenanceID = mJson.value("maintenanceID", "");
            m.aircraftType = mJson.value("aircraftType", "");
            m.startDate = mJson.value("startDate", "");
            m.endDate = mJson.value("endDate", "");
            m.maintenanceType = mJson.value("maintenanceType", "");
            m.description = mJson.value("description", "");
            m.status = mJson.value("status", "");
            m.estimatedCost = mJson.value("estimatedCost", 0.0);

            maintenance.push_back(m);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load maintenance: " + std::string(e.what()));
    }

    return maintenance;
}

void MaintenanceRepository::saveAll(const std::vector<MaintenanceRecord>& maintenance) {
    json data;
    data["maintenance"] = json::array();

    for (const auto& m : maintenance) {
        json mJson;
        mJson["maintenanceID"] = m.maintenanceID;
        mJson["aircraftType"] = m.aircraftType;
        mJson["startDate"] = m.startDate;
        mJson["endDate"] = m.endDate;
        mJson["maintenanceType"] = m.maintenanceType;
        mJson["description"] = m.description;
        mJson["status"] = m.status;
        mJson["estimatedCost"] = m.estimatedCost;

        data["maintenance"].push_back(mJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save maintenance: " + std::string(e.what()));
    }
}

std::optional<MaintenanceRecord> MaintenanceRepository::findByID(
    const std::string& maintenanceID,
    const std::vector<MaintenanceRecord>& maintenance
) {
    auto it = std::find_if(maintenance.begin(), maintenance.end(),
                           [&](const MaintenanceRecord& m) { return m.maintenanceID == maintenanceID; });
    return (it != maintenance.end()) ? std::optional(*it) : std::nullopt;
}

std::vector<MaintenanceRecord> MaintenanceRepository::findByAircraftType(
    const std::string& aircraftType,
    const std::vector<MaintenanceRecord>& maintenance
) {
    std::vector<MaintenanceRecord> results;
    std::copy_if(maintenance.begin(), maintenance.end(), std::back_inserter(results),
                 [&](const MaintenanceRecord& m) { return m.aircraftType == aircraftType; });
    return results;
}

std::vector<MaintenanceRecord> MaintenanceRepository::findByStatus(
    const std::string& status,
    const std::vector<MaintenanceRecord>& maintenance
) {
    std::vector<MaintenanceRecord> results;
    std::copy_if(maintenance.begin(), maintenance.end(), std::back_inserter(results),
                 [&](const MaintenanceRecord& m) { return m.status == status; });
    return results;
}
