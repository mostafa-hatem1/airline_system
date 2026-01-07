#include "FlightOperationsUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

FlightOperationsUI::FlightOperationsUI(
    std::vector<FlightRecord>& flights,
    std::vector<CrewRecord>& crew,
    std::vector<MaintenanceRecord>& maintenance,
    std::vector<FlightCrewRecord>& flightCrew,
    std::vector<AircraftRecord>& aircraft,
    FlightRepository& flightRepo,
    CrewRepository& crewRepo,
    MaintenanceRepository& maintenanceRepo,
    FlightCrewRepository& flightCrewRepo,
    AircraftRepository& aircraftRepo
)
    : m_flights(flights),
      m_crew(crew),
      m_maintenance(maintenance),
      m_flightCrew(flightCrew),
      m_aircraft(aircraft),
      m_flightRepo(flightRepo),
      m_crewRepo(crewRepo),
      m_maintenanceRepo(maintenanceRepo),
      m_flightCrewRepo(flightCrewRepo),
      m_aircraftRepo(aircraftRepo)
{
}


void FlightOperationsUI::showFlightOperationsMenu() {
    while (true) {
        std::cout << "\n--- Flight Operations ---\n";
        std::cout << "1. Assign Crew to Flight\n";
        std::cout << "2. Schedule Maintenance\n";
        std::cout << "3. View Maintenance Logs\n";
        std::cout << "4. View Aircraft Status\n";
        std::cout << "5. Back to Admin Menu\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 5);

        if (choice == 1) {
            assignCrew();
        } else if (choice == 2) {
            scheduleMaintenance();
        } else if (choice == 3) {
            viewMaintenanceLogs();
        } else if (choice == 4) {
            viewAircraftStatus();
        } else {
            return;
        }
    }
}

void FlightOperationsUI::assignCrew() {
    std::cout << "\n--- Assign Crew to Flight ---\n";

    const std::string flightNumber = ConsoleIO::readLine("Enter Flight Number: ");
    if (flightNumber.empty()) {
        std::cout << "Error: Flight number cannot be empty.\n";
        return;
    }

    auto flight = std::find_if(m_flights.begin(), m_flights.end(),
                               [&](const FlightRecord& f) { return f.flightNumber == flightNumber; });
    if (flight == m_flights.end()) {
        std::cout << "Error: Flight not found.\n";
        return;
    }

    // List available pilots
    auto pilots = m_crewRepo.findByRole("Pilot", m_crew);
    std::cout << "\nAvailable Pilots:\n";
    for (size_t i = 0; i < pilots.size(); ++i) {
        std::cout << i + 1 << ". " << pilots[i].crewID << " - " << pilots[i].name
                  << " (Hours: " << pilots[i].currentFlightHours << "/" << pilots[i].maxFlightHours << ")\n";
    }

    if (pilots.empty()) {
        std::cout << "Error: No pilots available.\n";
        return;
    }

    int pilotChoice = ConsoleIO::readIntInRange("Select Pilot by number: ", 1, static_cast<int>(pilots.size()));
    const std::string pilotID = pilots[pilotChoice - 1].crewID;

    // List available attendants
    auto attendants = m_crewRepo.findByRole("Attendant", m_crew);
    std::cout << "\nAvailable Attendants:\n";
    for (size_t i = 0; i < attendants.size(); ++i) {
        std::cout << i + 1 << ". " << attendants[i].crewID << " - " << attendants[i].name
                  << " (Hours: " << attendants[i].currentFlightHours << "/" << attendants[i].maxFlightHours << ")\n";
    }

    if (attendants.empty()) {
        std::cout << "Error: No attendants available.\n";
        return;
    }

    std::cout << "\nHow many attendants to assign? ";
    int numAttendants = ConsoleIO::readIntInRange("", 1, static_cast<int>(attendants.size()));

    std::vector<std::string> assignedAttendants;
    for (int i = 0; i < numAttendants; ++i) {
        int attChoice = ConsoleIO::readIntInRange("Select Attendant " + std::to_string(i + 1) + ": ", 1, static_cast<int>(attendants.size()));
        assignedAttendants.push_back(attendants[attChoice - 1].crewID);
    }

    // Check if assignment already exists
    auto existingAssignment = m_flightCrewRepo.findByFlightNumber(flightNumber, m_flightCrew);
    if (existingAssignment) {
        std::cout << "Warning: Flight already has crew assigned. Updating assignment.\n";
        auto it = std::find_if(m_flightCrew.begin(), m_flightCrew.end(),
                               [&](const FlightCrewRecord& fc) { return fc.flightNumber == flightNumber; });
        if (it != m_flightCrew.end()) {
            it->pilotID = pilotID;
            it->attendantIDs = assignedAttendants;
        }
    } else {
        FlightCrewRecord newAssignment(flightNumber, pilotID, assignedAttendants);
        m_flightCrew.push_back(newAssignment);
    }

    try {
        m_flightCrewRepo.saveAll(m_flightCrew);
        std::cout << "\nCrew assigned successfully to Flight " << flightNumber << "!\n";
        std::cout << "Pilot: " << pilotID << "\n";
        std::cout << "Attendants: ";
        for (const auto& attID : assignedAttendants) {
            std::cout << attID << " ";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save crew assignment: " << e.what() << "\n";
    }
}

void FlightOperationsUI::scheduleMaintenance() {
    std::cout << "\n--- Schedule Maintenance ---\n";

    const std::string aircraftType = ConsoleIO::readLine("Enter Aircraft Type: ");
    if (aircraftType.empty()) {
        std::cout << "Error: Aircraft type cannot be empty.\n";
        return;
    }

    auto aircraft = std::find_if(m_aircraft.begin(), m_aircraft.end(),
                                 [&](const AircraftRecord& a) { return a.aircraftType == aircraftType; });
    if (aircraft == m_aircraft.end()) {
        std::cout << "Error: Aircraft type not found.\n";
        return;
    }

    const std::string startDate = ConsoleIO::readLine("Enter Start Date (YYYY-MM-DD): ");
    const std::string endDate = ConsoleIO::readLine("Enter End Date (YYYY-MM-DD, leave empty if ongoing): ");

    std::cout << "Maintenance Type:\n";
    std::cout << "1. Scheduled\n";
    std::cout << "2. Emergency\n";
    std::cout << "3. Inspection\n";
    int typeChoice = ConsoleIO::readIntInRange("Select type: ", 1, 3);

    std::string maintenanceType;
    if (typeChoice == 1) {
        maintenanceType = "Scheduled";
    } else if (typeChoice == 2) {
        maintenanceType = "Emergency";
    } else {
        maintenanceType = "Inspection";
    }

    const std::string description = ConsoleIO::readLine("Enter Description: ");
    double cost = 0.0;
    try {
        const std::string costStr = ConsoleIO::readLine("Enter Estimated Cost (0 if unknown): ");
        if (!costStr.empty()) {
            cost = std::stod(costStr);
        }
    } catch (...) {
        std::cout << "Warning: Invalid cost, using 0.\n";
    }

    const std::string maintenanceID = getNextMaintenanceID();

    MaintenanceRecord newMaintenance(
        maintenanceID,
        aircraftType,
        startDate,
        endDate,
        maintenanceType,
        description,
        "Scheduled",
        cost
    );

    m_maintenance.push_back(newMaintenance);

    try {
        m_maintenanceRepo.saveAll(m_maintenance);
        std::cout << "\nMaintenance scheduled successfully!\n";
        std::cout << "Maintenance ID: " << maintenanceID << "\n";
        std::cout << "Aircraft: " << aircraftType << "\n";
        std::cout << "Start Date: " << startDate << "\n";
        if (!endDate.empty()) {
            std::cout << "End Date: " << endDate << "\n";
        }
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save maintenance: " << e.what() << "\n";
        m_maintenance.pop_back();
    }
}

void FlightOperationsUI::viewMaintenanceLogs() {
    std::cout << "\n--- Maintenance Logs ---\n";

    if (m_maintenance.empty()) {
        std::cout << "No maintenance records found.\n";
        return;
    }

    std::cout << std::string(130, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "Maintenance ID"
              << std::setw(18) << "Aircraft"
              << std::setw(15) << "Start Date"
              << std::setw(15) << "End Date"
              << std::setw(15) << "Type"
              << std::setw(12) << "Status"
              << std::setw(12) << "Cost"
              << "\n";
    std::cout << std::string(130, '-') << "\n";

    for (const auto& m : m_maintenance) {
        std::cout << std::left
                  << std::setw(15) << m.maintenanceID
                  << std::setw(18) << m.aircraftType
                  << std::setw(15) << m.startDate
                  << std::setw(15) << (m.endDate.empty() ? "Ongoing" : m.endDate)
                  << std::setw(15) << m.maintenanceType
                  << std::setw(12) << m.status
                  << std::setw(12) << "$" + std::to_string(static_cast<int>(m.estimatedCost))
                  << "\n";
    }

    std::cout << std::string(130, '-') << "\n";
}

void FlightOperationsUI::viewAircraftStatus() {
    std::cout << "\n--- Aircraft Status ---\n";

    if (m_aircraft.empty()) {
        std::cout << "No aircraft found.\n";
        return;
    }

    std::cout << std::string(100, '-') << "\n";
    std::cout << std::left
              << std::setw(20) << "Aircraft Type"
              << std::setw(12) << "Total Seats"
              << std::setw(18) << "Manufacturer"
              << std::setw(15) << "Status"
              << "\n";
    std::cout << std::string(100, '-') << "\n";

    for (const auto& ac : m_aircraft) {
        std::cout << std::left
                  << std::setw(20) << ac.aircraftType
                  << std::setw(12) << ac.totalSeats
                  << std::setw(18) << ac.manufacturer
                  << std::setw(15) << ac.status
                  << "\n";
    }

    std::cout << std::string(100, '-') << "\n";
}

std::string FlightOperationsUI::getNextMaintenanceID() {
    int maxID = 0;

    for (const auto& m : m_maintenance) {
        if (m.maintenanceID.length() > 1 && m.maintenanceID[0] == 'M') {
            try {
                int id = std::stoi(m.maintenanceID.substr(1));
                maxID = std::max(maxID, id);
            } catch (...) {
                // Ignore
            }
        }
    }

    std::ostringstream oss;
    oss << "M" << std::setfill('0') << std::setw(3) << (maxID + 1);
    return oss.str();
}

std::string FlightOperationsUI::getNextCrewID() {
    int maxID = 0;

    for (const auto& c : m_crew) {
        if (c.crewID.length() > 2) {
            try {
                int id = std::stoi(c.crewID.substr(2));
                maxID = std::max(maxID, id);
            } catch (...) {
                // Ignore
            }
        }
    }

    std::ostringstream oss;
    oss << "C" << std::setfill('0') << std::setw(3) << (maxID + 1);
    return oss.str();
}
