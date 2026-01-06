#ifndef RESERVATION_RECORD_HPP
#define RESERVATION_RECORD_HPP

#include <string>

/**
 * @brief Represents a reservation record as stored in persistence (JSON).
 * 
 * A reservation ties together a passenger, a flight, a seat, and payment info.
 */
struct ReservationRecord {
    std::string reservationID;          // e.g., "R001", "R002"
    std::string flightNumber;           // e.g., "AA123"
    std::string passengerID;            // e.g., "P456"
    std::string passengerName;          // Full name of passenger
    std::string seatNumber;             // e.g., "12A"
    double totalCost;                   // e.g., 350.00
    std::string paymentMethod;          // "Credit Card", "Cash", "PayPal"
    std::string paymentDetails;         // XXXX-XXXX-XXXX-1234 or email (masked)
    std::string status;                 // "Confirmed", "Canceled"
    std::string bookingDateTime;        // Timestamp, e.g., "2023-12-15 14:30:00"

    ReservationRecord() = default;

    ReservationRecord(
        const std::string& resID,
        const std::string& flight,
        const std::string& pID,
        const std::string& pName,
        const std::string& seat,
        double cost,
        const std::string& method,
        const std::string& details,
        const std::string& stat,
        const std::string& datetime
    ) : reservationID(resID), flightNumber(flight), passengerID(pID),
        passengerName(pName), seatNumber(seat), totalCost(cost),
        paymentMethod(method), paymentDetails(details), status(stat),
        bookingDateTime(datetime) {}
};

#endif // RESERVATION_RECORD_HPP
