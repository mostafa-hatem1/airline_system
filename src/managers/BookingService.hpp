#ifndef BOOKING_SERVICE_HPP
#define BOOKING_SERVICE_HPP

#include "../core/FlightRecord.hpp"
#include "../core/ReservationRecord.hpp"
#include "../core/SeatMap.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>

/**
 * @brief Service for handling flight bookings and seat management.
 * 
 * Manages seat availability, reservation creation, and pricing.
 */
class BookingService {
public:
    /**
     * @brief Constructor.
     */
    BookingService();

    /**
     * @brief Initialize seat maps for all flights.
     * 
     * @param flights The list of flights.
     * @param reservations Existing reservations to populate seat maps.
     */
    void initializeSeats(
        const std::vector<FlightRecord>& flights,
        const std::vector<ReservationRecord>& reservations
    );

    /**
     * @brief Get the seat map for a flight.
     * 
     * @param flightNumber The flight to get seats for.
     * @return The SeatMap for that flight, or nullptr if not found.
     */
    std::shared_ptr<SeatMap> getSeatMap(const std::string& flightNumber);

    /**
     * @brief Check if a seat is available on a flight.
     * 
     * @param flightNumber The flight.
     * @param seatNumber The seat to check.
     * @return true if available; false otherwise.
     */
    bool isSeatAvailable(const std::string& flightNumber, const std::string& seatNumber);

    /**
     * @brief Book a seat on a flight.
     * 
     * @param flightNumber The flight.
     * @param seatNumber The seat to book.
     * @return true if booking succeeded; false otherwise.
     */
    bool bookSeat(const std::string& flightNumber, const std::string& seatNumber);

    /**
     * @brief Get the price for a flight (simple flat price per flight).
     * 
     * @param flight The flight record.
     * @return The price as a double.
     */
    double calculatePrice(const FlightRecord& flight);

    /**
     * @brief Generate a unique reservation ID.
     * 
     * @param existingReservations List of existing reservations to avoid conflicts.
     * @return A new unique reservation ID (e.g., "R001").
     */
    std::string generateReservationID(const std::vector<ReservationRecord>& existingReservations);

private:
    std::map<std::string, std::shared_ptr<SeatMap>> m_seatMaps;  // flightNumber -> SeatMap
};

#endif // BOOKING_SERVICE_HPP
