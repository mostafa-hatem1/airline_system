#ifndef AIRCRAFT_REPOSITORY_HPP
#define AIRCRAFT_REPOSITORY_HPP

#include "../core/AircraftRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting aircraft to/from JSON.
 */
class AircraftRepository {
public:
    /**
     * @brief Constructor.
     * 
     * @param dataFilePath Path to the aircraft JSON file. Defaults to "data/aircraft.json".
     */
    explicit AircraftRepository(const std::string& dataFilePath = "data/aircraft.json");

    /**
     * @brief Load all aircraft from the JSON file.
     * 
     * @return A vector of AircraftRecord objects loaded from disk.
     * @throws std::runtime_error if file can't be read or JSON is invalid.
     */
    std::vector<AircraftRecord> loadAll();

    /**
     * @brief Save all aircraft to the JSON file.
     * 
     * @param aircraft Vector of AircraftRecord objects to save.
     * @throws std::runtime_error if file can't be written.
     */
    void saveAll(const std::vector<AircraftRecord>& aircraft);

    /**
     * @brief Find aircraft by type.
     * 
     * @param aircraftType The aircraft type to search for.
     * @param aircraft The vector to search in.
     * @return The aircraft record if found; std::nullopt otherwise.
     */
    std::optional<AircraftRecord> findByID(
        const std::string& aircraftID,
        const std::vector<AircraftRecord>& aircraft
    );

private:
    std::string m_dataFilePath;
};

#endif // AIRCRAFT_REPOSITORY_HPP
