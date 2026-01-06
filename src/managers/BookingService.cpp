#include "BookingService.hpp"
#include "../core/SeatMap.hpp"
#include <algorithm>
#include <sstream>
#include <iomanip>

BookingService::BookingService() {
}

void BookingService::initializeSeats(
    const std::vector<FlightRecord>& flights,
    const std::vector<ReservationRecord>& reservations
) {
    // Create a seat map for each flight
    for (const auto& flight : flights) {
        std::vector<std::string> bookedSeats;

        // Populate booked seats from existing reservations
        for (const auto& res : reservations) {
            if (res.flightNumber == flight.flightNumber && res.status == "Confirmed") {
                bookedSeats.push_back(res.seatNumber);
            }
        }

        // Create seat map with booked seats
        m_seatMaps[flight.flightNumber] = 
            std::make_shared<SeatMap>(flight.totalSeats, bookedSeats);
    }
}

std::shared_ptr<SeatMap> BookingService::getSeatMap(const std::string& flightNumber) {
    auto it = m_seatMaps.find(flightNumber);
    return (it != m_seatMaps.end()) ? it->second : nullptr;
}

bool BookingService::isSeatAvailable(const std::string& flightNumber, const std::string& seatNumber) {
    auto seatMap = getSeatMap(flightNumber);
    if (!seatMap) {
        return false;
    }
    return !seatMap->isBooked(seatNumber) && seatMap->isValidSeatNumber(seatNumber);
}

bool BookingService::bookSeat(const std::string& flightNumber, const std::string& seatNumber) {
    auto seatMap = getSeatMap(flightNumber);
    if (!seatMap) {
        return false;
    }
    return seatMap->bookSeat(seatNumber);
}

double BookingService::calculatePrice(const FlightRecord& flight) {
    // Simple flat price: $250 base + $0.50 per seat available
    // This is a placeholder; adjust as needed
    // For now, just return a fixed price based on aircraft type
    if (flight.aircraftType.find("Boeing") != std::string::npos) {
        return 350.00;
    } else if (flight.aircraftType.find("Airbus") != std::string::npos) {
        return 300.00;
    }
    return 250.00;
}

std::string BookingService::generateReservationID(const std::vector<ReservationRecord>& existingReservations) {
    int maxID = 0;

    for (const auto& res : existingReservations) {
        if (res.reservationID.length() > 1 && res.reservationID[0] == 'R') {
            try {
                int id = std::stoi(res.reservationID.substr(1));
                maxID = std::max(maxID, id);
            } catch (...) {
                // Ignore parse errors
            }
        }
    }

    // Generate next ID
    std::ostringstream oss;
    oss << "R" << std::setfill('0') << std::setw(3) << (maxID + 1);
    return oss.str();
}
