#ifndef RESERVATION_UI_H
#define RESERVATION_UI_H

#include "../core/UserRecord.hpp"
#include "../core/FlightRecord.hpp"
#include "../core/ReservationRecord.hpp"
#include "../persistence/ReservationRepository.hpp"
#include "../persistence/FlightRepository.hpp"
#include "../managers/BookingService.hpp"
#include <vector>

/**
 * @brief UI for passenger/agent reservation management and check-in.
 */
class ReservationUI {
public:
    ReservationUI(
        std::vector<FlightRecord>& flights,
        std::vector<ReservationRecord>& reservations,
        FlightRepository& flightRepo,
        ReservationRepository& reservationRepo,
        BookingService& bookingService
    );

    // Passenger: view their reservations
    void showMyReservations(const UserRecord& passenger);

    // Passenger: check in to a flight
    void checkIn(const UserRecord& passenger);

    // Booking agent: modify a reservation (change seat)
    void modifyReservation();

    // Booking agent: cancel a reservation
    void cancelReservation();

private:
    std::vector<FlightRecord>& m_flights;
    std::vector<ReservationRecord>& m_reservations;
    FlightRepository& m_flightRepo;
    ReservationRepository& m_reservationRepo;
    BookingService& m_bookingService;

    void printBoardingPass(const ReservationRecord& res, const FlightRecord& flight);
};

#endif // RESERVATION_UI_HPP
