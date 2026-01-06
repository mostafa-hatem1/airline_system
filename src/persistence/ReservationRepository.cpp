#include "ReservationRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
namespace fs = std::filesystem;

ReservationRepository::ReservationRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<ReservationRecord> ReservationRepository::loadAll() {
    std::vector<ReservationRecord> reservations;

    if (!fs::exists(m_dataFilePath)) {
        std::cout << "Reservations file does not exist: " << m_dataFilePath << "\n";
        return reservations;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        if (!data.contains("reservations") || !data["reservations"].is_array()) {
            throw std::runtime_error(
                "Invalid reservations JSON structure: expected 'reservations' array"
            );
        }

        for (const auto& resJson : data["reservations"]) {
            ReservationRecord res;
            res.reservationID = resJson.value("reservationID", "");
            res.flightNumber = resJson.value("flightNumber", "");
            res.passengerID = resJson.value("passengerID", "");
            res.passengerName = resJson.value("passengerName", "");
            res.seatNumber = resJson.value("seatNumber", "");
            res.totalCost = resJson.value("totalCost", 0.0);
            res.paymentMethod = resJson.value("paymentMethod", "");
            res.paymentDetails = resJson.value("paymentDetails", "");
            res.status = resJson.value("status", "");
            res.bookingDateTime = resJson.value("bookingDateTime", "");

            reservations.push_back(res);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to load reservations from " + m_dataFilePath + ": " + e.what()
        );
    }

    return reservations;
}

void ReservationRepository::saveAll(const std::vector<ReservationRecord>& reservations) {
    json data;
    data["reservations"] = json::array();

    for (const auto& res : reservations) {
        json resJson;
        resJson["reservationID"] = res.reservationID;
        resJson["flightNumber"] = res.flightNumber;
        resJson["passengerID"] = res.passengerID;
        resJson["passengerName"] = res.passengerName;
        resJson["seatNumber"] = res.seatNumber;
        resJson["totalCost"] = res.totalCost;
        resJson["paymentMethod"] = res.paymentMethod;
        resJson["paymentDetails"] = res.paymentDetails;
        resJson["status"] = res.status;
        resJson["bookingDateTime"] = res.bookingDateTime;

        data["reservations"].push_back(resJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to save reservations to " + m_dataFilePath + ": " + e.what()
        );
    }
}

std::optional<ReservationRecord> ReservationRepository::findByID(
    const std::string& reservationID,
    const std::vector<ReservationRecord>& reservations
) {
    auto it = std::find_if(reservations.begin(), reservations.end(),
                           [&](const ReservationRecord& r) {
                               return r.reservationID == reservationID;
                           });
    return (it != reservations.end()) ? std::optional(*it) : std::nullopt;
}

std::vector<ReservationRecord> ReservationRepository::findByPassengerID(
    const std::string& passengerID,
    const std::vector<ReservationRecord>& reservations
) {
    std::vector<ReservationRecord> results;
    std::copy_if(reservations.begin(), reservations.end(),
                 std::back_inserter(results),
                 [&](const ReservationRecord& r) {
                     return r.passengerID == passengerID;
                 });
    return results;
}

std::vector<ReservationRecord> ReservationRepository::findByFlightNumber(
    const std::string& flightNumber,
    const std::vector<ReservationRecord>& reservations
) {
    std::vector<ReservationRecord> results;
    std::copy_if(reservations.begin(), reservations.end(),
                 std::back_inserter(results),
                 [&](const ReservationRecord& r) {
                     return r.flightNumber == flightNumber;
                 });
    return results;
}
