#ifndef BOOKING_UI_H
#define BOOKING_UI_H

#include "../persistence/FlightRepository.hpp"
#include "../persistence/ReservationRepository.hpp"
#include "../managers/BookingService.hpp"
#include "../managers/FlightSearchService.hpp"
#include "../core/UserRecord.hpp"
#include <vector>
#include <optional>

/**
 * @brief UI module for flight search and booking workflow.
 * 
 * Handles the complete booking flow: search → select → view seats → book → pay.
 */
class BookingUI {
public:
    /**
     * @brief Constructor.
     * 
     * @param users Reference to user list (needed for passenger lookup by booking agents).
     * @param flights Reference to flight list.
     * @param reservations Reference to reservation list.
     * @param flightRepo Flight repository for persistence.
     * @param reservationRepo Reservation repository for persistence.
     * @param bookingService Booking service for seat/reservation logic.
     */
    BookingUI(
        std::vector<UserRecord>& users,
        std::vector<FlightRecord>& flights,
        std::vector<ReservationRecord>& reservations,
        FlightRepository& flightRepo,
        ReservationRepository& reservationRepo,
        BookingService& bookingService
    );

    /**
     * @brief Run the full booking workflow for a passenger/agent.
     * 
     * @param user The logged-in user (passenger or booking agent).
     */
    void runBookingFlow(const UserRecord& user);

private:
    std::vector<UserRecord>& m_users;
    std::vector<FlightRecord>& m_flights;
    std::vector<ReservationRecord>& m_reservations;
    FlightRepository& m_flightRepo;
    ReservationRepository& m_reservationRepo;
    BookingService& m_bookingService;

    /**
     * @brief Search for flights based on user input.
     * 
     * @return A vector of matching flights, or empty if no matches.
     */
    std::vector<FlightRecord> searchFlights();

    /**
     * @brief Display available flights and let user select one.
     * 
     * @param flights The flights to display.
     * @return The selected flight, or nullopt if user cancels.
     */
    std::optional<FlightRecord> selectFlight(const std::vector<FlightRecord>& flights);

    /**
     * @brief Display available seats and let user book one.
     * 
     * @param flight The flight to book.
     * @param user The passenger/agent booking.
     */
    void bookSeatOnFlight(const FlightRecord& flight, const UserRecord& user);

    /**
     * @brief Handle payment details input.
     * 
     * @param method The payment method chosen.
     * @return Masked payment details string.
     */
    std::string getPaymentDetails(const std::string& method);

    /**
     * @brief Find a passenger by user ID.
     * 
     * @param userId The user ID to search for.
     * @return The passenger user record if found; std::nullopt otherwise.
     */
    std::optional<UserRecord> findPassengerById(const std::string& userId) const;
};

#endif // BOOKING_UI_H
