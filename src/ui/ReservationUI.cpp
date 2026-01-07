#include "ReservationUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

ReservationUI::ReservationUI(
    std::vector<FlightRecord>& flights,
    std::vector<ReservationRecord>& reservations,
    FlightRepository& flightRepo,
    ReservationRepository& reservationRepo,
    BookingService& bookingService
) : m_flights(flights),
    m_reservations(reservations),
    m_flightRepo(flightRepo),
    m_reservationRepo(reservationRepo),
    m_bookingService(bookingService) {
}

void ReservationUI::showMyReservations(const UserRecord& passenger) {
    std::cout << "\n--- My Reservations ---\n";
    std::cout << "Fetching reservations for " << passenger.username << "...\n\n";

    auto passengerRes = m_reservationRepo.findByPassengerID(passenger.userId, m_reservations);

    if (passengerRes.empty()) {
        std::cout << "No reservations found.\n";
        return;
    }

    std::cout << std::string(120, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "Reservation ID"
              << std::setw(12) << "Flight"
              << std::setw(20) << "From"
              << std::setw(20) << "To"
              << std::setw(18) << "Departure"
              << std::setw(8) << "Seat"
              << std::setw(15) << "Status"
              << "\n";
    std::cout << std::string(120, '-') << "\n";

    for (size_t i = 0; i < passengerRes.size(); ++i) {
        const auto& res = passengerRes[i];
        auto flight = std::find_if(m_flights.begin(), m_flights.end(),
                                   [&](const FlightRecord& f) { return f.flightNumber == res.flightNumber; });

        std::string origin = (flight != m_flights.end()) ? flight->origin : "N/A";
        std::string dest = (flight != m_flights.end()) ? flight->destination : "N/A";
        std::string dept = (flight != m_flights.end()) ? flight->departureDateTime : "N/A";

        std::cout << std::left
                  << std::setw(15) << res.reservationID
                  << std::setw(12) << res.flightNumber
                  << std::setw(20) << origin
                  << std::setw(20) << dest
                  << std::setw(18) << dept
                  << std::setw(8) << res.seatNumber
                  << std::setw(15) << res.status
                  << "\n";
    }

    std::cout << std::string(120, '-') << "\n";
}

void ReservationUI::checkIn(const UserRecord& passenger) {
    std::cout << "\n--- Check-In ---\n";

    const std::string resID = ConsoleIO::readLine("Enter Reservation ID: ");
    if (resID.empty()) {
        std::cout << "Error: Reservation ID cannot be empty.\n";
        return;
    }

    auto resOpt = m_reservationRepo.findByID(resID, m_reservations);
    if (!resOpt) {
        std::cout << "Error: Reservation " << resID << " not found.\n";
        return;
    }

    const ReservationRecord& res = *resOpt;

    // Validate ownership
    if (res.passengerID != passenger.userId) {
        std::cout << "Error: This reservation does not belong to you.\n";
        return;
    }

    // Validate status
    if (res.status != "Confirmed") {
        std::cout << "Error: Only confirmed reservations can check in. Current status: " << res.status << "\n";
        return;
    }

    // Find flight
    auto flight = std::find_if(m_flights.begin(), m_flights.end(),
                               [&](const FlightRecord& f) { return f.flightNumber == res.flightNumber; });
    if (flight == m_flights.end()) {
        std::cout << "Error: Flight not found.\n";
        return;
    }

    // Mark as checked in (update status in reservations)
    auto it = std::find_if(m_reservations.begin(), m_reservations.end(),
                           [&](const ReservationRecord& r) { return r.reservationID == resID; });
    if (it != m_reservations.end()) {
        it->status = "CheckedIn";
    }

    // Save
    try {
        m_reservationRepo.saveAll(m_reservations);
        std::cout << "\nCheck-In Successful!\n";
        printBoardingPass(res, *flight);
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to check in: " << e.what() << "\n";
    }
}

void ReservationUI::modifyReservation() {
    std::cout << "\n--- Modify Reservation ---\n";

    const std::string resID = ConsoleIO::readLine("Enter Reservation ID to modify: ");
    if (resID.empty()) {
        std::cout << "Error: Reservation ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_reservations.begin(), m_reservations.end(),
                           [&](const ReservationRecord& r) { return r.reservationID == resID; });
    if (it == m_reservations.end()) {
        std::cout << "Error: Reservation not found.\n";
        return;
    }

    if (it->status != "Confirmed") {
        std::cout << "Error: Can only modify confirmed reservations.\n";
        return;
    }

    const std::string oldSeat = it->seatNumber;
    const std::string newSeat = ConsoleIO::readLine("Enter new seat number: ");

    if (newSeat.empty() || newSeat == oldSeat) {
        std::cout << "Error: Invalid new seat.\n";
        return;
    }

    // Check availability
    auto seatMap = m_bookingService.getSeatMap(it->flightNumber);
    if (!seatMap || !seatMap->isValidSeatNumber(newSeat)) {
        std::cout << "Error: Seat number " << newSeat << " is invalid.\n";
        return;
    }

    if (!m_bookingService.isSeatAvailable(it->flightNumber, newSeat)) {
        std::cout << "Error: Seat " << newSeat << " is already occupied.\n";
        return;
    }

    // Release old seat, book new seat
    seatMap->cancelSeat(oldSeat);
    if (!seatMap->bookSeat(newSeat)) {
        seatMap->bookSeat(oldSeat);  // Rollback
        std::cout << "Error: Failed to book new seat.\n";
        return;
    }

    // Update reservation
    it->seatNumber = newSeat;

    try {
        m_reservationRepo.saveAll(m_reservations);
        std::cout << "\nReservation modified successfully!\n";
        std::cout << "Old Seat: " << oldSeat << " → New Seat: " << newSeat << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save: " << e.what() << "\n";
        seatMap->cancelSeat(newSeat);
        seatMap->bookSeat(oldSeat);
        it->seatNumber = oldSeat;
    }
}

void ReservationUI::cancelReservation() {
    std::cout << "\n--- Cancel Reservation ---\n";

    const std::string resID = ConsoleIO::readLine("Enter Reservation ID to cancel: ");
    if (resID.empty()) {
        std::cout << "Error: Reservation ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_reservations.begin(), m_reservations.end(),
                           [&](const ReservationRecord& r) { return r.reservationID == resID; });
    if (it == m_reservations.end()) {
        std::cout << "Error: Reservation not found.\n";
        return;
    }

    std::cout << "Are you sure you want to cancel Reservation ID " << resID << "? (yes/no): ";
    const std::string confirm = ConsoleIO::readLine("");

    if (confirm != "yes") {
        std::cout << "Cancellation aborted.\n";
        return;
    }

    // Release seat
    auto seatMap = m_bookingService.getSeatMap(it->flightNumber);
    if (seatMap) {
        seatMap->cancelSeat(it->seatNumber);
    }

    const double refundAmount = it->totalCost;
    const std::string paymentMethod = it->paymentMethod;
    const std::string paymentDetails = it->paymentDetails;

    // Mark as canceled
    it->status = "Canceled";

    try {
        m_reservationRepo.saveAll(m_reservations);

        std::cout << "\nReservation " << resID << " has been successfully canceled.\n";
        std::cout << "Processing refund to " << paymentMethod << " account " << paymentDetails << "\n";
        std::cout << "Refund Amount: $" << std::fixed << std::setprecision(2) << refundAmount
                  << " has been credited to your " << paymentMethod << " account.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to cancel: " << e.what() << "\n";
        it->status = "Confirmed";  // Rollback
        seatMap->bookSeat(it->seatNumber);
    }
}

void ReservationUI::printBoardingPass(const ReservationRecord& res, const FlightRecord& flight) {
    // Simple gate assignment (deterministic based on flight number)
    char gatePrefix = 'A' + (res.seatNumber[0] % 3);
    int gateNum = 10 + (std::stoi(res.seatNumber.substr(0, res.seatNumber.length() - 1)) % 20);

    // Boarding time: 1 hour before departure (simplified)
    std::string boardingTime = "0730";  // Placeholder

    std::cout << "\nBoarding Pass\n";
    std::cout << std::string(45, '-') << "\n";
    std::cout << "Reservation ID: " << res.reservationID << "\n";
    std::cout << "Passenger: " << res.passengerName << "\n";
    std::cout << "Flight: " << flight.flightNumber << "\n";
    std::cout << "Origin: " << flight.origin << "\n";
    std::cout << "Destination: " << flight.destination << "\n";
    std::cout << "Departure: " << flight.departureDateTime << "\n";
    std::cout << "Seat: " << res.seatNumber << "\n";
    std::cout << "Gate: " << gatePrefix << gateNum << "\n";
    std::cout << "Boarding Time: " << boardingTime << "\n";
    std::cout << std::string(45, '-') << "\n";
}
