#include "AircraftRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

AircraftRepository::AircraftRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<AircraftRecord> AircraftRepository::loadAll() {
    std::vector<AircraftRecord> aircraft;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Aircraft file does not exist: " << m_dataFilePath << "\n";
        return aircraft;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("aircraft") || !data["aircraft"].is_array()) {
            throw std::runtime_error(
                "Invalid aircraft JSON structure: expected 'aircraft' array"
            );
        }

        for (const auto& aircraftJson : data["aircraft"]) {
            AircraftRecord ac;
            ac.aircraftID = aircraftJson.value("aircraftID", "");
            ac.aircraftType = aircraftJson.value("aircraftType", "");
            ac.totalSeats = aircraftJson.value("totalSeats", 0);
            ac.manufacturer = aircraftJson.value("manufacturer", "");
            ac.status = aircraftJson.value("status", "");
            ac.manufactureYear = aircraftJson.value("manufactureYear", "");
            ac.nextMaintenanceDate = aircraftJson.value("nextMaintenanceDate", "");
            ac.lastMaintenanceDate = aircraftJson.value("lastMaintenanceDate", "");

            aircraft.push_back(ac);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to load aircraft from " + m_dataFilePath + ": " + e.what()
        );
    }

    return aircraft;
}

void AircraftRepository::saveAll(const std::vector<AircraftRecord>& aircraft) {
    json data;
    data["aircraft"] = json::array();

    for (const auto& ac : aircraft) {
        json aircraftJson;
        aircraftJson["aircraftID"] = ac.aircraftID;
        aircraftJson["aircraftType"] = ac.aircraftType;
        aircraftJson["totalSeats"] = ac.totalSeats;
        aircraftJson["manufacturer"] = ac.manufacturer;
        aircraftJson["status"] = ac.status;
        aircraftJson["manufactureYear"] = ac.manufactureYear;
        aircraftJson["nextMaintenanceDate"] = ac.nextMaintenanceDate;
        aircraftJson["lastMaintenanceDate"] = ac.lastMaintenanceDate;

        data["aircraft"].push_back(aircraftJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to save aircraft to " + m_dataFilePath + ": " + e.what()
        );
    }
}

std::optional<AircraftRecord> AircraftRepository::findByID(
    const std::string& aircraftID,
    const std::vector<AircraftRecord>& aircraft
) {
    auto it = std::find_if(aircraft.begin(), aircraft.end(),
                           [&](const AircraftRecord& a) {
                               return a.aircraftID == aircraftID;
                           });
    return (it != aircraft.end()) ? std::optional(*it) : std::nullopt;
}
