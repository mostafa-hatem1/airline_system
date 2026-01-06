#ifndef FLIGHT_SEARCH_SERVICE_H
#define FLIGHT_SEARCH_SERVICE_H

#include "../core/FlightRecord.hpp"
#include <vector>
#include <string>

/**
 * @brief Service for searching and filtering flights.
 * 
 * Provides simple search functionality based on origin, destination, and date.
 */
class FlightSearchService {
public:
    /**
     * @brief Search flights by origin and destination.
     * 
     * Simple substring matching (case-insensitive).
     * 
     * @param origin The origin to search for.
     * @param destination The destination to search for.
     * @param departureDate Optional departure date filter (e.g., "2023-12-20").
     * @param flights The flight list to search in.
     * @return A vector of matching flights.
     */
    static std::vector<FlightRecord> searchFlights(
        const std::string& origin,
        const std::string& destination,
        const std::string& departureDate,
        const std::vector<FlightRecord>& flights
    );

    /**
     * @brief Filter flights by status (e.g., only "Scheduled").
     * 
     * @param flights The flight list to filter.
     * @param status The status to filter by.
     * @return A vector of flights matching the status.
     */
    static std::vector<FlightRecord> filterByStatus(
        const std::vector<FlightRecord>& flights,
        const std::string& status
    );
};

#endif // FLIGHT_SEARCH_SERVICE_HPP
