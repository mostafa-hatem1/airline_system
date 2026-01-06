#ifndef SEAT_MAP_HPP
#define SEAT_MAP_HPP

#include <string>
#include <vector>
#include <set>
#include <algorithm>

/**
 * @brief Manages seat occupancy for a flight.
 * 
 * Stores a set of booked seat numbers (e.g., "12A", "15B").
 * Provides utility methods to check availability and book seats.
 */
class SeatMap {
public:
    /**
     * @brief Constructor.
     * 
     * @param totalSeats Total number of seats on the aircraft (e.g., 180).
     * @param bookedSeats Initial list of booked seat numbers (optional).
     */
    SeatMap(int totalSeats, const std::vector<std::string>& bookedSeats = {});

    /**
     * @brief Check if a seat number is valid for this aircraft.
     * 
     * Valid format: digit(s) followed by a letter (e.g., "12A", "1B").
     * Also checks if row number is within bounds (1 to numRows).
     * 
     * @param seatNumber The seat number to validate.
     * @return true if valid; false otherwise.
     */
    bool isValidSeatNumber(const std::string& seatNumber) const;

    /**
     * @brief Check if a seat is currently booked.
     * 
     * @param seatNumber The seat to check.
     * @return true if booked; false if available.
     */
    bool isBooked(const std::string& seatNumber) const;

    /**
     * @brief Book a seat.
     * 
     * @param seatNumber The seat to book.
     * @return true if booking succeeded; false if seat already booked.
     */
    bool bookSeat(const std::string& seatNumber);

    /**
     * @brief Cancel a booking (release a seat).
     * 
     * @param seatNumber The seat to release.
     * @return true if cancellation succeeded; false if seat wasn't booked.
     */
    bool cancelSeat(const std::string& seatNumber);

    /**
     * @brief Get the list of all booked seats.
     * 
     * @return Vector of booked seat numbers.
     */
    std::vector<std::string> getBookedSeats() const;

    /**
     * @brief Get the number of available seats.
     * 
     * @return Count of unbooked seats.
     */
    int getAvailableSeats() const;

    /**
     * @brief Get total seats for this aircraft.
     * 
     * @return Total seat count.
     */
    int getTotalSeats() const { return m_totalSeats; }

private:
    int m_totalSeats;                   // e.g., 180
    std::set<std::string> m_bookedSeats; // e.g., {"12A", "15B"}

    /**
     * @brief Parse seat number into row and column.
     * 
     * @param seatNumber The seat (e.g., "12A").
     * @param row Output: row number (e.g., 12).
     * @param col Output: column letter (e.g., "A").
     * @return true if parsing succeeded; false otherwise.
     */
    bool parseSeat(const std::string& seatNumber, int& row, char& col) const;
};

#endif // SEAT_MAP_HPP
