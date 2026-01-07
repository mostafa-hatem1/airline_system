#include "FlightCrewRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

FlightCrewRepository::FlightCrewRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<FlightCrewRecord> FlightCrewRepository::loadAll() {
    std::vector<FlightCrewRecord> flightCrew;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Flight crew file does not exist: " << m_dataFilePath << "\n";
        return flightCrew;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("flight_crew") || !data["flight_crew"].is_array()) {
            throw std::runtime_error("Invalid flight crew JSON structure");
        }

        for (const auto& fcJson : data["flight_crew"]) {
            FlightCrewRecord fc;
            fc.flightNumber = fcJson.value("flightNumber", "");
            fc.pilotID = fcJson.value("pilotID", "");
            fc.assignmentDate = fcJson.value("assignmentDate", "");

            if (fcJson.contains("attendantIDs") && fcJson["attendantIDs"].is_array()) {
                for (const auto& attID : fcJson["attendantIDs"]) {
                    fc.attendantIDs.push_back(attID.get<std::string>());
                }
            }

            flightCrew.push_back(fc);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load flight crew: " + std::string(e.what()));
    }

    return flightCrew;
}

void FlightCrewRepository::saveAll(const std::vector<FlightCrewRecord>& flightCrew) {
    json data;
    data["flight_crew"] = json::array();

    for (const auto& fc : flightCrew) {
        json fcJson;
        fcJson["flightNumber"] = fc.flightNumber;
        fcJson["pilotID"] = fc.pilotID;
        fcJson["assignmentDate"] = fc.assignmentDate;
        fcJson["attendantIDs"] = fc.attendantIDs;

        data["flight_crew"].push_back(fcJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save flight crew: " + std::string(e.what()));
    }
}

std::optional<FlightCrewRecord> FlightCrewRepository::findByFlightNumber(
    const std::string& flightNumber,
    const std::vector<FlightCrewRecord>& flightCrew
) {
    auto it = std::find_if(flightCrew.begin(), flightCrew.end(),
                           [&](const FlightCrewRecord& fc) { return fc.flightNumber == flightNumber; });
    return (it != flightCrew.end()) ? std::optional(*it) : std::nullopt;
}
