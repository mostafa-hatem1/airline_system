#include "SeatMap.hpp"
#include <cctype>
#include <sstream>

SeatMap::SeatMap(int totalSeats, const std::vector<std::string>& bookedSeats)
    : m_totalSeats(totalSeats) {
    for (const auto& seat : bookedSeats) {
        m_bookedSeats.insert(seat);
    }
}

bool SeatMap::parseSeat(const std::string& seatNumber, int& row, char& col) const {
    if (seatNumber.empty()) {
        return false;
    }

    // Last character should be a letter (column)
    col = seatNumber.back();
    if (!std::isalpha(col)) {
        return false;
    }

    // Rest should be digits (row)
    std::string rowStr = seatNumber.substr(0, seatNumber.length() - 1);
    if (rowStr.empty()) {
        return false;
    }

    try {
        row = std::stoi(rowStr);
    } catch (...) {
        return false;
    }

    return row > 0;
}

bool SeatMap::isValidSeatNumber(const std::string& seatNumber) const {
    int row;
    char col;
    if (!parseSeat(seatNumber, row, col)) {
        return false;
    }

    // Estimate number of rows (assume 6 columns per row: A-F)
    // For a 180-seat aircraft: 180 / 6 = 30 rows
    // Adjust as needed based on typical aircraft layouts
    int estimatedRows = (m_totalSeats + 5) / 6;  // Round up
    
    return row >= 1 && row <= estimatedRows && 
           col >= 'A' && col <= 'F';  // Standard: A-F per row
}

bool SeatMap::isBooked(const std::string& seatNumber) const {
    return m_bookedSeats.count(seatNumber) > 0;
}

bool SeatMap::bookSeat(const std::string& seatNumber) {
    if (isBooked(seatNumber)) {
        return false;  // Already booked
    }
    m_bookedSeats.insert(seatNumber);
    return true;
}

bool SeatMap::cancelSeat(const std::string& seatNumber) {
    if (!isBooked(seatNumber)) {
        return false;  // Not booked
    }
    m_bookedSeats.erase(seatNumber);
    return true;
}

std::vector<std::string> SeatMap::getBookedSeats() const {
    return std::vector<std::string>(m_bookedSeats.begin(), m_bookedSeats.end());
}

int SeatMap::getAvailableSeats() const {
    return m_totalSeats - static_cast<int>(m_bookedSeats.size());
}
