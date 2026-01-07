#include "AircraftManagementUI.hpp"
#include "../utils/ConsoleIO.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

AircraftManagementUI::AircraftManagementUI(AircraftRepository& aircraftRepo)
    : m_aircraftRepo(aircraftRepo) {
    m_aircraft = m_aircraftRepo.loadAll();
}

void AircraftManagementUI::showAircraftManagementMenu() {
    while (true) {
        std::cout << "\n--- Manage Aircraft ---\n";
        std::cout << "1. Add New Aircraft\n";
        std::cout << "2. Update Aircraft\n";
        std::cout << "3. Remove Aircraft\n";
        std::cout << "4. List All Aircraft\n";
        std::cout << "5. View Aircraft Status\n";
        std::cout << "6. Back to Admin Menu\n";

        const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 6);

        if (choice == 1) {
            addAircraft();
        } else if (choice == 2) {
            updateAircraft();
        } else if (choice == 3) {
            removeAircraft();
        } else if (choice == 4) {
            listAllAircraft();
        } else if (choice == 5) {
            viewAircraftStatus();
        } else {
            return;
        }
    }
}

void AircraftManagementUI::addAircraft() {
    std::cout << "\n--- Add New Aircraft ---\n";

    const std::string aircraftType = ConsoleIO::readLine("Enter Aircraft Type (e.g., Boeing 737): ");
    if (aircraftType.empty()) {
        std::cout << "Error: Aircraft type cannot be empty.\n";
        return;
    }

    const std::string manufacturer = ConsoleIO::readLine("Enter Manufacturer (e.g., Boeing): ");
    if (manufacturer.empty()) {
        std::cout << "Error: Manufacturer cannot be empty.\n";
        return;
    }

    int totalSeats = 0;
    try {
        const std::string seatsStr = ConsoleIO::readLine("Enter Total Seats: ");
        totalSeats = std::stoi(seatsStr);
        if (totalSeats <= 0) {
            std::cout << "Error: Total seats must be greater than 0.\n";
            return;
        }
    } catch (...) {
        std::cout << "Error: Invalid seat count.\n";
        return;
    }

    const std::string manufactureYear = ConsoleIO::readLine("Enter Manufacture Year (e.g., 2020): ");
    if (manufactureYear.empty()) {
        std::cout << "Error: Manufacture year cannot be empty.\n";
        return;
    }

    const std::string aircraftID = getNextAircraftID();

    AircraftRecord newAircraft;
    newAircraft.aircraftID = aircraftID;
    newAircraft.aircraftType = aircraftType;
    newAircraft.manufacturer = manufacturer;
    newAircraft.totalSeats = totalSeats;
    newAircraft.status = "Available";
    newAircraft.manufactureYear = manufactureYear;
    newAircraft.lastMaintenanceDate = "N/A";
    newAircraft.nextMaintenanceDate = "N/A";

    m_aircraft.push_back(newAircraft);

    try {
        m_aircraftRepo.saveAll(m_aircraft);
        std::cout << "\nAircraft added successfully!\n";
        std::cout << "Aircraft ID: " << aircraftID << "\n";
        std::cout << "Type: " << aircraftType << "\n";
        std::cout << "Manufacturer: " << manufacturer << "\n";
        std::cout << "Total Seats: " << totalSeats << "\n";
        std::cout << "Status: Available\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save aircraft: " << e.what() << "\n";
        m_aircraft.pop_back();
    }
}

void AircraftManagementUI::updateAircraft() {
    std::cout << "\n--- Update Aircraft ---\n";

    const std::string aircraftID = ConsoleIO::readLine("Enter Aircraft ID to update: ");
    if (aircraftID.empty()) {
        std::cout << "Error: Aircraft ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_aircraft.begin(), m_aircraft.end(),
                           [&](const AircraftRecord& a) { return a.aircraftID == aircraftID; });
    if (it == m_aircraft.end()) {
        std::cout << "Error: Aircraft not found.\n";
        return;
    }

    std::cout << "Current Aircraft: " << it->aircraftID << " - " << it->aircraftType << " (Status: " << it->status << ")\n";
    std::cout << "\nWhat do you want to update?\n";
    std::cout << "1. Status (Available/InMaintenance/Grounded)\n";
    std::cout << "2. Next Maintenance Date\n";
    std::cout << "3. Back\n";

    const int choice = ConsoleIO::readIntInRange("Enter choice: ", 1, 3);

    if (choice == 1) {
        std::cout << "\nAvailable Status Values:\n";
        std::cout << "1. Available\n";
        std::cout << "2. InMaintenance\n";
        std::cout << "3. Grounded\n";

        const int statusChoice = ConsoleIO::readIntInRange("Select status: ", 1, 3);

        if (statusChoice == 1) {
            it->status = "Available";
        } else if (statusChoice == 2) {
            it->status = "InMaintenance";
        } else {
            it->status = "Grounded";
        }

        std::cout << "Status updated to: " << it->status << "\n";
    } else if (choice == 2) {
        const std::string newDate = ConsoleIO::readLine("Enter Next Maintenance Date (YYYY-MM-DD): ");
        if (!newDate.empty()) {
            it->nextMaintenanceDate = newDate;
            std::cout << "Next maintenance date updated to: " << newDate << "\n";
        } else {
            std::cout << "Error: Date cannot be empty.\n";
            return;
        }
    } else {
        return;
    }

    try {
        m_aircraftRepo.saveAll(m_aircraft);
        std::cout << "Aircraft updated successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save aircraft: " << e.what() << "\n";
    }
}

void AircraftManagementUI::removeAircraft() {
    std::cout << "\n--- Remove Aircraft ---\n";

    const std::string aircraftID = ConsoleIO::readLine("Enter Aircraft ID to remove: ");
    if (aircraftID.empty()) {
        std::cout << "Error: Aircraft ID cannot be empty.\n";
        return;
    }

    auto it = std::find_if(m_aircraft.begin(), m_aircraft.end(),
                           [&](const AircraftRecord& a) { return a.aircraftID == aircraftID; });
    if (it == m_aircraft.end()) {
        std::cout << "Error: Aircraft not found.\n";
        return;
    }

    std::cout << "Remove aircraft: " << it->aircraftType << " (ID: " << it->aircraftID << ")? (yes/no): ";
    const std::string confirm = ConsoleIO::readLine("");

    if (confirm != "yes") {
        std::cout << "Removal aborted.\n";
        return;
    }

    m_aircraft.erase(it);

    try {
        m_aircraftRepo.saveAll(m_aircraft);
        std::cout << "Aircraft removed successfully.\n";
    } catch (const std::exception& e) {
        std::cout << "Error: Failed to save aircraft: " << e.what() << "\n";
    }
}

void AircraftManagementUI::listAllAircraft() {
    std::cout << "\n--- All Aircraft ---\n";

    if (m_aircraft.empty()) {
        std::cout << "No aircraft in fleet.\n";
        return;
    }

    std::cout << std::string(130, '-') << "\n";
    std::cout << std::left
              << std::setw(15) << "Aircraft ID"
              << std::setw(20) << "Type"
              << std::setw(20) << "Manufacturer"
              << std::setw(12) << "Seats"
              << std::setw(15) << "Year"
              << std::setw(15) << "Status"
              << "\n";
    std::cout << std::string(130, '-') << "\n";

    for (const auto& aircraft : m_aircraft) {
        std::cout << std::left
                  << std::setw(15) << aircraft.aircraftID
                  << std::setw(20) << aircraft.aircraftType
                  << std::setw(20) << aircraft.manufacturer
                  << std::setw(12) << aircraft.totalSeats
                  << std::setw(15) << aircraft.manufactureYear
                  << std::setw(15) << aircraft.status
                  << "\n";
    }

    std::cout << std::string(130, '-') << "\n";
}

void AircraftManagementUI::viewAircraftStatus() {
    std::cout << "\n--- Aircraft Status Report ---\n";

    if (m_aircraft.empty()) {
        std::cout << "No aircraft in fleet.\n";
        return;
    }

    int availableCount = 0;
    int maintenanceCount = 0;
    int groundedCount = 0;

    for (const auto& aircraft : m_aircraft) {
        if (aircraft.status == "Available") {
            availableCount++;
        } else if (aircraft.status == "InMaintenance") {
            maintenanceCount++;
        } else if (aircraft.status == "Grounded") {
            groundedCount++;
        }
    }

    std::cout << "\nFleet Summary:\n";
    std::cout << std::string(50, '-') << "\n";
    std::cout << "Total Aircraft: " << m_aircraft.size() << "\n";
    std::cout << "Available: " << availableCount << "\n";
    std::cout << "In Maintenance: " << maintenanceCount << "\n";
    std::cout << "Grounded: " << groundedCount << "\n";
    std::cout << std::string(50, '-') << "\n\n";

    std::cout << "Detailed Status by Aircraft:\n";
    std::cout << std::string(120, '-') << "\n";
    std::cout << std::left
              << std::setw(20) << "Aircraft Type"
              << std::setw(20) << "Status"
              << std::setw(20) << "Last Maintenance"
              << std::setw(20) << "Next Maintenance"
              << "\n";
    std::cout << std::string(120, '-') << "\n";

    for (const auto& aircraft : m_aircraft) {
        std::cout << std::left
                  << std::setw(20) << aircraft.aircraftType
                  << std::setw(20) << aircraft.status
                  << std::setw(20) << aircraft.lastMaintenanceDate
                  << std::setw(20) << aircraft.nextMaintenanceDate
                  << "\n";
    }

    std::cout << std::string(120, '-') << "\n";
}

std::string AircraftManagementUI::getNextAircraftID() {
    int maxID = 0;

    for (const auto& aircraft : m_aircraft) {
        if (aircraft.aircraftID.length() > 2 && aircraft.aircraftID.substr(0, 2) == "AC") {
            try {
                int id = std::stoi(aircraft.aircraftID.substr(2));
                maxID = std::max(maxID, id);
            } catch (...) {
                // Ignore
            }
        }
    }

    std::ostringstream oss;
    oss << "AC" << std::setfill('0') << std::setw(3) << (maxID + 1);
    return oss.str();
}
