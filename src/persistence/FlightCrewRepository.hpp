#ifndef FLIGHT_CREW_REPOSITORY_HPP
#define FLIGHT_CREW_REPOSITORY_HPP

#include "../core/FlightCrewRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting flight crew assignments to/from JSON.
 */
class FlightCrewRepository {
public:
    explicit FlightCrewRepository(const std::string& dataFilePath = "data/flight_crew.json");

    std::vector<FlightCrewRecord> loadAll();
    void saveAll(const std::vector<FlightCrewRecord>& flightCrew);

    std::optional<FlightCrewRecord> findByFlightNumber(
        const std::string& flightNumber,
        const std::vector<FlightCrewRecord>& flightCrew
    );

private:
    std::string m_dataFilePath;
};

#endif // FLIGHT_CREW_REPOSITORY_HPP
