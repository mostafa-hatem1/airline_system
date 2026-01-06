#include "FlightRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

FlightRepository::FlightRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<FlightRecord> FlightRepository::loadAll() {
    std::vector<FlightRecord> flights;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Flight file does not exist: " << m_dataFilePath << "\n";
        return flights;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("flights") || !data["flights"].is_array()) {
            throw std::runtime_error(
                "Invalid flight JSON structure: expected 'flights' array"
            );
        }

        for (const auto& flightJson : data["flights"]) {
            FlightRecord flight;
            flight.flightNumber = flightJson.value("flightNumber", "");
            flight.origin = flightJson.value("origin", "");
            flight.destination = flightJson.value("destination", "");
            flight.departureDateTime = flightJson.value("departureDateTime", "");
            flight.arrivalDateTime = flightJson.value("arrivalDateTime", "");
            flight.aircraftType = flightJson.value("aircraftType", "");
            flight.totalSeats = flightJson.value("totalSeats", 0);
            flight.status = flightJson.value("status", "");

            flights.push_back(flight);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to load flights from " + m_dataFilePath + ": " + e.what()
        );
    }

    return flights;
}

void FlightRepository::saveAll(const std::vector<FlightRecord>& flights) {
    json data;
    data["flights"] = json::array();

    for (const auto& flight : flights) {
        json flightJson;
        flightJson["flightNumber"] = flight.flightNumber;
        flightJson["origin"] = flight.origin;
        flightJson["destination"] = flight.destination;
        flightJson["departureDateTime"] = flight.departureDateTime;
        flightJson["arrivalDateTime"] = flight.arrivalDateTime;
        flightJson["aircraftType"] = flight.aircraftType;
        flightJson["totalSeats"] = flight.totalSeats;
        flightJson["status"] = flight.status;

        data["flights"].push_back(flightJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to save flights to " + m_dataFilePath + ": " + e.what()
        );
    }
}

std::optional<FlightRecord> FlightRepository::findByFlightNumber(
    const std::string& flightNumber,
    const std::vector<FlightRecord>& flights
) {
    auto it = std::find_if(flights.begin(), flights.end(),
                           [&](const FlightRecord& f) {
                               return f.flightNumber == flightNumber;
                           });
    return (it != flights.end()) ? std::optional(*it) : std::nullopt;
}
