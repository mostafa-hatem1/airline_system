#include "ReportingUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

ReportingUI::ReportingUI(
    const std::vector<FlightRecord>& flights,
    const std::vector<ReservationRecord>& reservations,
    const std::vector<MaintenanceRecord>& maintenance,
    const std::vector<UserRecord>& users
) : m_flights(flights),
    m_reservations(reservations),
    m_maintenance(maintenance),
    m_users(users) {
}

void ReportingUI::showReportingMenu() {
    while (true) {
        std::cout << "\n--- Generate Reports ---\n";
        std::cout << "1. Operational Report\n";
        std::cout << "2. Maintenance Report\n";
        std::cout << "3. User Activity Report\n";
        std::cout << "4. Back to Admin Menu\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 4);

        if (choice == 1) {
            operationalReport();
        } else if (choice == 2) {
            maintenanceReport();
        } else if (choice == 3) {
            userActivityReport();
        } else {
            return;
        }
    }
}

void ReportingUI::operationalReport() {
    std::cout << "\n--- Operational Report ---\n";

    const std::string monthYear = ConsoleIO::readLine("Enter Month and Year for Report (MM-YYYY): ");
    if (monthYear.empty() || monthYear.length() != 7) {
        std::cout << "Error: Invalid format. Use MM-YYYY.\n";
        return;
    }

    std::cout << "\nGenerating Operational Report for " << monthYear << "...\n\n";

    // Summary statistics
    int totalFlightsScheduled = 0;
    int totalFlightsCompleted = 0;
    int totalFlightsDelayed = 0;
    int totalFlightsCanceled = 0;

    for (const auto& flight : m_flights) {
        if (flight.departureDateTime.substr(5, 2) == monthYear.substr(0, 2) &&
            flight.departureDateTime.substr(0, 4) == monthYear.substr(3, 4)) {
            totalFlightsScheduled++;

            if (flight.status == "Scheduled") {
                totalFlightsCompleted++;
            } else if (flight.status == "Delayed") {
                totalFlightsDelayed++;
            } else if (flight.status == "Canceled") {
                totalFlightsCanceled++;
            }
        }
    }

    int totalReservationsMade = countReservationsByMonth(monthYear);
    double totalRevenue = sumRevenueByMonth(monthYear);

    std::cout << "Report Summary:\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << "Total Flights Scheduled: " << totalFlightsScheduled << "\n";
    std::cout << "Flights Completed: " << totalFlightsCompleted << "\n";
    std::cout << "Flights Delayed: " << totalFlightsDelayed << "\n";
    std::cout << "Flights Canceled: " << totalFlightsCanceled << "\n";
    std::cout << "Total Reservations Made: " << totalReservationsMade << "\n";
    std::cout << "Total Revenue: $" << std::fixed << std::setprecision(2) << totalRevenue << "\n";
    std::cout << std::string(50, '-') << "\n\n";

    std::cout << "Detailed Flight Performance:\n";
    std::cout << std::string(80, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "Flight"
              << std::setw(18) << "Status"
              << std::setw(15) << "Bookings"
              << std::setw(15) << "Revenue"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

    int flightNum = 1;
    for (const auto& flight : m_flights) {
        if (flight.departureDateTime.substr(5, 2) == monthYear.substr(0, 2) &&
            flight.departureDateTime.substr(0, 4) == monthYear.substr(3, 4)) {

            int bookings = countReservationsByMonth(monthYear, flight.flightNumber);
            double flightRevenue = bookings * flight.price;

            std::cout << std::left
                      << std::setw(15) << flight.flightNumber
                      << std::setw(18) << flight.status
                      << std::setw(15) << bookings
                      << std::setw(15) << "$" + std::to_string(static_cast<long long>(flightRevenue))
                      << "\n";

            flightNum++;
        }
    }

    std::cout << std::string(80, '-') << "\n";
}

void ReportingUI::maintenanceReport() {
    std::cout << "\n--- Maintenance Report ---\n";

    if (m_maintenance.empty()) {
        std::cout << "No maintenance records found.\n";
        return;
    }

    std::cout << "\nMaintenance Summary:\n";
    std::cout << std::string(100, '-') << "\n";

    int totalScheduled = 0;
    int totalCompleted = 0;
    int totalInProgress = 0;
    double totalCost = 0.0;

    for (const auto& m : m_maintenance) {
        if (m.status == "Scheduled") {
            totalScheduled++;
        } else if (m.status == "Completed") {
            totalCompleted++;
        } else if (m.status == "InProgress") {
            totalInProgress++;
        }
        totalCost += m.estimatedCost;
    }

    std::cout << "Scheduled Maintenance: " << totalScheduled << "\n";
    std::cout << "Completed Maintenance: " << totalCompleted << "\n";
    std::cout << "In Progress: " << totalInProgress << "\n";
    std::cout << "Total Estimated Cost: $" << std::fixed << std::setprecision(2) << totalCost << "\n";
    std::cout << std::string(100, '-') << "\n\n";

    std::cout << "Detailed Maintenance Logs:\n";
    std::cout << std::string(130, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "ID"
              << std::setw(15) << "Aircraft"
              << std::setw(15) << "Start Date"
              << std::setw(15) << "End Date"
              << std::setw(15) << "Type"
              << std::setw(12) << "Status"
              << std::setw(15) << "Cost"
              << "\n";
    std::cout << std::string(130, '-') << "\n";

    for (const auto& m : m_maintenance) {
        std::cout << std::left
                  << std::setw(15) << m.maintenanceID
                  << std::setw(15) << m.aircraftType
                  << std::setw(15) << m.startDate
                  << std::setw(15) << (m.endDate.empty() ? "Ongoing" : m.endDate)
                  << std::setw(15) << m.maintenanceType
                  << std::setw(12) << m.status
                  << std::setw(15) << "$" + std::to_string(static_cast<long long>(m.estimatedCost))
                  << "\n";
    }

    std::cout << std::string(130, '-') << "\n";
}

void ReportingUI::userActivityReport() {
    std::cout << "\n--- User Activity Report ---\n";

    std::cout << "\nUser Summary:\n";
    std::cout << std::string(80, '-') << "\n";

    int totalAdmins = 0;
    int totalAgents = 0;
    int totalPassengers = 0;

    for (const auto& user : m_users) {
        if (user.role == "ADMINISTRATOR") {
            totalAdmins++;
        } else if (user.role == "BOOKING_AGENT") {
            totalAgents++;
        } else if (user.role == "PASSENGER") {
            totalPassengers++;
        }
    }

    std::cout << "Total Administrators: " << totalAdmins << "\n";
    std::cout << "Total Booking Agents: " << totalAgents << "\n";
    std::cout << "Total Passengers: " << totalPassengers << "\n";
    std::cout << "Total Users: " << m_users.size() << "\n";
    std::cout << std::string(80, '-') << "\n\n";

    std::cout << "User List:\n";
    std::cout << std::string(90, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "User ID"
              << std::setw(20) << "Username"
              << std::setw(20) << "Role"
              << "\n";
    std::cout << std::string(90, '-') << "\n";

    for (const auto& user : m_users) {
        std::cout << std::left
                  << std::setw(15) << user.userId
                  << std::setw(20) << user.username
                  << std::setw(20) << user.role
                  << "\n";
    }

    std::cout << std::string(90, '-') << "\n\n";

    // Booking activity by user
    std::cout << "Booking Activity by User:\n";
    std::cout << std::string(80, '-') << "\n";
    std::cout << std::left
              << std::setw(20) << "Username"
              << std::setw(15) << "Reservations"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& user : m_users) {
        if (user.role == "PASSENGER") {
            int bookingCount = 0;
            for (const auto& res : m_reservations) {
                if (res.passengerID == user.userId && res.status == "Confirmed") {
                    bookingCount++;
                }
            }
            std::cout << std::left
                      << std::setw(20) << user.username
                      << std::setw(15) << bookingCount
                      << "\n";
        }
    }

    std::cout << std::string(80, '-') << "\n";
}

int ReportingUI::countReservationsByMonth(const std::string& monthYear, const std::string& flightNumber) {
    int count = 0;

    for (const auto& res : m_reservations) {
        if (res.status != "Confirmed") continue;

        if (!flightNumber.empty() && res.flightNumber != flightNumber) continue;

        // Extract month-year from booking date (assuming format YYYY-MM-DD HH:MM:SS)
        if (res.bookingDateTime.length() >= 7) {
            std::string resMonthYear = res.bookingDateTime.substr(5, 2) + "-" + res.bookingDateTime.substr(0, 4);
            if (resMonthYear == monthYear) {
                count++;
            }
        }
    }

    return count;
}

double ReportingUI::sumRevenueByMonth(const std::string& monthYear) {
    double revenue = 0.0;

    for (const auto& res : m_reservations) {
        if (res.status != "Confirmed") continue;

        // Extract month-year from booking date
        if (res.bookingDateTime.length() >= 7) {
            std::string resMonthYear = res.bookingDateTime.substr(5, 2) + "-" + res.bookingDateTime.substr(0, 4);
            if (resMonthYear == monthYear) {
                revenue += res.totalCost;
            }
        }
    }

    return revenue;
}
