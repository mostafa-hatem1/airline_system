#ifndef FLIGHT_RECORD_HPP
#define FLIGHT_RECORD_HPP

#include <string>

/**
 * @brief Represents a flight record as stored in persistence (JSON).
 * 
 * Minimal structure for flight data. Later, we'll wrap this in a full Flight class
 * with seat management and business logic.
 */
struct FlightRecord {
    std::string flightNumber;           // e.g., "AA123"
    std::string origin;                 // e.g., "New York JFK"
    std::string destination;            // e.g., "Los Angeles LAX"
    std::string departureDateTime;      // e.g., "2023-12-15 0830"
    std::string arrivalDateTime;        // e.g., "2023-12-15 1145"
    std::string aircraftType;           // e.g., "Boeing 737"
    int totalSeats;                     // e.g., 180
    std::string status;                 // "Scheduled", "Delayed", or "Canceled"
    double price;                       // e.g., 299.99
    FlightRecord() = default;

    FlightRecord(
        const std::string& number,
        const std::string& orig,
        const std::string& dest,
        const std::string& dept,
        const std::string& arrv,
        const std::string& aircraft,
        int seats,
        const std::string& stat,
        double p = 250.00
    ) : flightNumber(number), origin(orig), destination(dest),
        departureDateTime(dept), arrivalDateTime(arrv),
        aircraftType(aircraft), totalSeats(seats), status(stat), price(p) {}
};

#endif // FLIGHT_RECORD_HPP
