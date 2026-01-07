#ifndef REPORTING_UI_HPP
#define REPORTING_UI_HPP

#include "../core/FlightRecord.hpp"
#include "../core/ReservationRecord.hpp"
#include "../core/MaintenanceRecord.hpp"
#include "../core/UserRecord.hpp"
#include <vector>
#include <string>

/**
 * @brief UI for generating reports on flights, reservations, maintenance, and users.
 */
class ReportingUI {
public:
    ReportingUI(
        const std::vector<FlightRecord>& flights,
        const std::vector<ReservationRecord>& reservations,
        const std::vector<MaintenanceRecord>& maintenance,
        const std::vector<UserRecord>& users
    );

    void showReportingMenu();

private:
    const std::vector<FlightRecord>& m_flights;
    const std::vector<ReservationRecord>& m_reservations;
    const std::vector<MaintenanceRecord>& m_maintenance;
    const std::vector<UserRecord>& m_users;

    void operationalReport();
    void maintenanceReport();
    void userActivityReport();

    int countReservationsByMonth(const std::string& monthYear, const std::string& flightNumber = "");
    double sumRevenueByMonth(const std::string& monthYear);
};

#endif // REPORTING_UI_HPP
