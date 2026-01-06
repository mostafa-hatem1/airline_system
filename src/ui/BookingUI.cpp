#include "BookingUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream>

BookingUI::BookingUI(
    std::vector<UserRecord>& users,
    std::vector<FlightRecord>& flights,
    std::vector<ReservationRecord>& reservations,
    FlightRepository& flightRepo,
    ReservationRepository& reservationRepo,
    BookingService& bookingService
) : m_users(users),
    m_flights(flights),
    m_reservations(reservations),
    m_flightRepo(flightRepo),
    m_reservationRepo(reservationRepo),
    m_bookingService(bookingService) {
}

void BookingUI::runBookingFlow(const UserRecord& user) {
    std::cout << "\n--- Book a Flight ---\n";

    // Step 1: Search flights
    auto matchingFlights = searchFlights();
    if (matchingFlights.empty()) {
        std::cout << "No flights found matching your criteria.\n";
        return;
    }

    // Step 2: Select a flight
    auto selectedFlight = selectFlight(matchingFlights);
    if (!selectedFlight) {
        std::cout << "Booking cancelled.\n";
        return;
    }

    // Step 3: Book a seat
    bookSeatOnFlight(selectedFlight.value(), user);
}

std::vector<FlightRecord> BookingUI::searchFlights() {
    std::string origin = ConsoleIO::readLine("Enter Origin: ");
    std::string destination = ConsoleIO::readLine("Enter Destination: ");
    std::string departureDate = ConsoleIO::readLine("Enter Departure Date (YYYY-MM-DD): ");

    return FlightSearchService::searchFlights(origin, destination, departureDate, m_flights);
}

std::optional<FlightRecord> BookingUI::selectFlight(const std::vector<FlightRecord>& flights) {
    std::cout << "\nAvailable Flights:\n";
    std::cout << std::string(140, '-') << "\n";
    std::cout << std::left
              << std::setw(3) << "#"
              << std::setw(12) << "Flight"
              << std::setw(20) << "Origin"
              << std::setw(20) << "Destination"
              << std::setw(18) << "Departure"
              << std::setw(15) << "Aircraft"
              << std::setw(10) << "Available"
              << std::setw(10) << "Price"
              << "\n";
    std::cout << std::string(140, '-') << "\n";

    int index = 1;
    for (const auto& flight : flights) {
        auto seatMap = m_bookingService.getSeatMap(flight.flightNumber);
        int available = seatMap ? seatMap->getAvailableSeats() : 0;

        std::cout << std::left
                  << std::setw(3) << index
                  << std::setw(12) << flight.flightNumber
                  << std::setw(20) << flight.origin
                  << std::setw(20) << flight.destination
                  << std::setw(18) << flight.departureDateTime
                  << std::setw(15) << flight.aircraftType
                  << std::setw(10) << available
                  << std::setw(10) << std::fixed << std::setprecision(2) << flight.price
                  << "\n";

        index++;
    }

    std::cout << std::string(140, '-') << "\n";
    std::cout << "0. Cancel\n";

    int choice = ConsoleIO::readIntInRange("Enter the Flight Number you wish to book or 0 to cancel: ", 0, static_cast<int>(flights.size()));

    if (choice == 0) {
        return std::nullopt;
    }

    return flights[choice - 1];
}

void BookingUI::bookSeatOnFlight(const FlightRecord& flight, const UserRecord& user) {
    std::cout << "\n--- Book a Seat on " << flight.flightNumber << " ---\n";

    // Display available seats (simple grid)
    auto seatMap = m_bookingService.getSeatMap(flight.flightNumber);
    if (!seatMap) {
        std::cout << "Error: Seat map not found for this flight.\n";
        return;
    }

    std::cout << "Available seats: " << seatMap->getAvailableSeats() << " / " << flight.totalSeats << "\n";
    std::cout << "Booked seats: " << seatMap->getBookedSeats().size() << "\n";
    std::cout << "Example seats: 1A, 1B, 2A, 2B, ..., 30F\n\n";

    // Determine actual passenger (role-based)
    std::string passengerId;
    std::string passengerName;

    if (user.role == "BOOKING_AGENT") {
        // Booking agent: prompt for passenger ID
        passengerId = ConsoleIO::readLine("Enter Passenger ID: ");
        auto passengerOpt = findPassengerById(passengerId);
        if (!passengerOpt) {
            std::cout << "Error: Passenger ID " << passengerId << " not found.\n";
            return;
        }
        passengerName = passengerOpt->username;
    } else {
        // Passenger: use logged-in user
        passengerId = user.userId;
        passengerName = user.username;
    }

    // Get seat number
    std::string seatNumber = ConsoleIO::readLine("Enter Seat Number (e.g., 12A): ");

    // Validate seat
    if (!seatMap->isValidSeatNumber(seatNumber)) {
        std::cout << "Error: Seat number " << seatNumber << " is invalid. Please enter a valid seat number e.g., 12A.\n";
        return;
    }

    if (!m_bookingService.isSeatAvailable(flight.flightNumber, seatNumber)) {
        std::cout << "Error: Seat " << seatNumber << " on Flight " << flight.flightNumber << " is already occupied. Please choose a different seat.\n";
        return;
    }

    // Payment method
    std::cout << "\nPayment Method:\n";
    std::cout << "1. Credit Card\n";
    std::cout << "2. Cash\n";
    std::cout << "3. PayPal\n";
    int paymentChoice = ConsoleIO::readIntInRange("Enter choice: ", 1, 3);

    std::string paymentMethod;
    if (paymentChoice == 1) {
        paymentMethod = "Credit Card";
    } else if (paymentChoice == 2) {
        paymentMethod = "Cash";
    } else {
        paymentMethod = "PayPal";
    }

    // Get payment details
    std::string paymentDetails = getPaymentDetails(paymentMethod);

    // Book the seat
    if (!m_bookingService.bookSeat(flight.flightNumber, seatNumber)) {
        std::cout << "Error: Failed to book seat. Please try again.\n";
        return;
    }

    // Create reservation
    double cost = flight.price;
    std::string resID = m_bookingService.generateReservationID(m_reservations);

    // Get current timestamp
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream timeStream;
    timeStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::string bookingTime = timeStream.str();

    ReservationRecord reservation(
        resID,
        flight.flightNumber,
        passengerId,
        passengerName,
        seatNumber,
        cost,
        paymentMethod,
        paymentDetails,
        "Confirmed",
        bookingTime
    );

    m_reservations.push_back(reservation);

    // Save to persistence
    try {
        m_reservationRepo.saveAll(m_reservations);
        std::cout << "\nBooking successful!\n";
        std::cout << "Reservation ID: " << resID << "\n";
        std::cout << "Passenger: " << passengerName << "\n";
        std::cout << "Flight: " << flight.flightNumber << " from " << flight.origin << " to " << flight.destination << "\n";
        std::cout << "Seat: " << seatNumber << "\n";
        std::cout << "Total Cost: $" << std::fixed << std::setprecision(2) << cost << "\n";
        std::cout << "Payment Method: " << paymentMethod << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save reservation: " << e.what() << "\n";
        // Rollback seat booking
        seatMap->cancelSeat(seatNumber);
        m_reservations.pop_back();
    }
}

std::string BookingUI::getPaymentDetails(const std::string& method) {
    if (method == "Credit Card") {
        std::string cardNum = ConsoleIO::readLine("Enter Payment Details (XXXX-XXXX-XXXX-1234): ");
        if (cardNum.length() >= 4) {
            return "XXXX-XXXX-XXXX-" + cardNum.substr(cardNum.length() - 4);
        }
        return "XXXX-XXXX-XXXX-0000";
    } else if (method == "PayPal") {
        return ConsoleIO::readLine("Enter PayPal Email: ");
    } else {
        return "Cash";
    }
}

std::optional<UserRecord> BookingUI::findPassengerById(const std::string& userId) const {
    auto it = std::find_if(m_users.begin(), m_users.end(),
                           [&](const UserRecord& u) {
                               return u.userId == userId && u.role == "PASSENGER";
                           });
    return (it != m_users.end()) ? std::optional(*it) : std::nullopt;
}
