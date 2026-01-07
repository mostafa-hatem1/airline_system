#include "CrewRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

CrewRepository::CrewRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<CrewRecord> CrewRepository::loadAll() {
    std::vector<CrewRecord> crew;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Crew file does not exist: " << m_dataFilePath << "\n";
        return crew;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("crew") || !data["crew"].is_array()) {
            throw std::runtime_error("Invalid crew JSON structure");
        }

        for (const auto& crewJson : data["crew"]) {
            CrewRecord c;
            c.crewID = crewJson.value("crewID", "");
            c.name = crewJson.value("name", "");
            c.role = crewJson.value("role", "");
            c.maxFlightHours = crewJson.value("maxFlightHours", 100.0);
            c.currentFlightHours = crewJson.value("currentFlightHours", 0.0);
            c.status = crewJson.value("status", "Available");
            c.lastAssignment = crewJson.value("lastAssignment", "");

            crew.push_back(c);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load crew: " + std::string(e.what()));
    }

    return crew;
}

void CrewRepository::saveAll(const std::vector<CrewRecord>& crew) {
    json data;
    data["crew"] = json::array();

    for (const auto& c : crew) {
        json crewJson;
        crewJson["crewID"] = c.crewID;
        crewJson["name"] = c.name;
        crewJson["role"] = c.role;
        crewJson["maxFlightHours"] = c.maxFlightHours;
        crewJson["currentFlightHours"] = c.currentFlightHours;
        crewJson["status"] = c.status;
        crewJson["lastAssignment"] = c.lastAssignment;

        data["crew"].push_back(crewJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save crew: " + std::string(e.what()));
    }
}

std::optional<CrewRecord> CrewRepository::findByID(
    const std::string& crewID,
    const std::vector<CrewRecord>& crew
) {
    auto it = std::find_if(crew.begin(), crew.end(),
                           [&](const CrewRecord& c) { return c.crewID == crewID; });
    return (it != crew.end()) ? std::optional(*it) : std::nullopt;
}

std::vector<CrewRecord> CrewRepository::findByRole(
    const std::string& role,
    const std::vector<CrewRecord>& crew
) {
    std::vector<CrewRecord> results;
    std::copy_if(crew.begin(), crew.end(), std::back_inserter(results),
                 [&](const CrewRecord& c) { return c.role == role; });
    return results;
}

std::vector<CrewRecord> CrewRepository::findAvailable(
    const std::vector<CrewRecord>& crew
) {
    std::vector<CrewRecord> results;
    std::copy_if(crew.begin(), crew.end(), std::back_inserter(results),
                 [](const CrewRecord& c) { return c.status == "Available"; });
    return results;
}
