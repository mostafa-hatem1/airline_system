#include "FlightSearchService.hpp"
#include <algorithm>
#include <cctype>
#include <string>
#include <iterator>

namespace {
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }
}

std::vector<FlightRecord> FlightSearchService::searchFlights(
    const std::string& origin,
    const std::string& destination,
    const std::string& departureDate,
    const std::vector<FlightRecord>& flights
) {
    std::vector<FlightRecord> results;

    std::string lowerOrigin = toLower(origin);
    std::string lowerDest = toLower(destination);
    std::string lowerDate = toLower(departureDate);

    for (const auto& flight : flights) {
        // Match origin and destination (case-insensitive substring)
        bool originMatch = (toLower(flight.origin).find(lowerOrigin) != std::string::npos);
        bool destMatch = (toLower(flight.destination).find(lowerDest) != std::string::npos);

        // Match departure date (if provided)
        bool dateMatch = departureDate.empty() || 
                         flight.departureDateTime.find(lowerDate) != std::string::npos;

        // Only include flights that are Scheduled
        bool statusMatch = (flight.status == "Scheduled");

        if (originMatch && destMatch && dateMatch && statusMatch) {
            results.push_back(flight);
        }
    }

    return results;
}

std::vector<FlightRecord> FlightSearchService::filterByStatus(
    const std::vector<FlightRecord>& flights,
    const std::string& status
) {
    std::vector<FlightRecord> results;
    std::copy_if(flights.begin(), flights.end(),
                 std::back_inserter(results),
                 [&](const FlightRecord& f) { return f.status == status; });
    return results;
}
