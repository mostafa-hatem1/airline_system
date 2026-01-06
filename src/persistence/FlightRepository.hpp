#ifndef FLIGHT_REPOSITORY_HPP
#define FLIGHT_REPOSITORY_HPP

#include "../core/FlightRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting flights to/from JSON.
 */
class FlightRepository {
public:
    /**
     * @brief Constructor.
     * 
     * @param dataFilePath Path to the flights JSON file. Defaults to "data/flights.json".
     */
    explicit FlightRepository(const std::string& dataFilePath = "data/flights.json");

    /**
     * @brief Load all flights from the JSON file.
     * 
     * @return A vector of FlightRecord objects loaded from disk.
     * @throws std::runtime_error if file can't be read or JSON is invalid.
     */
    std::vector<FlightRecord> loadAll();

    /**
     * @brief Save all flights to the JSON file.
     * 
     * @param flights Vector of FlightRecord objects to save.
     * @throws std::runtime_error if file can't be written.
     */
    void saveAll(const std::vector<FlightRecord>& flights);

    /**
     * @brief Find a flight by flight number (case-sensitive).
     * 
     * @param flightNumber The flight number to search for.
     * @param flights The vector to search in.
     * @return The flight record if found; std::nullopt otherwise.
     */
    std::optional<FlightRecord> findByFlightNumber(
        const std::string& flightNumber,
        const std::vector<FlightRecord>& flights
    );

private:
    std::string m_dataFilePath;
};

#endif // FLIGHT_REPOSITORY_HPP
