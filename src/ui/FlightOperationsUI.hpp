#ifndef FLIGHT_OPERATIONS_UI_HPP
#define FLIGHT_OPERATIONS_UI_HPP

#include "../core/FlightRecord.hpp"
#include "../core/CrewRecord.hpp"
#include "../core/MaintenanceRecord.hpp"
#include "../core/FlightCrewRecord.hpp"
#include "../core/AircraftRecord.hpp"
#include "../persistence/FlightRepository.hpp"
#include "../persistence/CrewRepository.hpp"
#include "../persistence/MaintenanceRepository.hpp"
#include "../persistence/FlightCrewRepository.hpp"
#include "../persistence/AircraftRepository.hpp"
#include <vector>

/**
 * @brief UI for flight operations: crew assignments and maintenance.
 */
class FlightOperationsUI {
public:
    FlightOperationsUI(
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
    );

    void showFlightOperationsMenu();

private:
    std::vector<FlightRecord>& m_flights;
    std::vector<CrewRecord>& m_crew;
    std::vector<MaintenanceRecord>& m_maintenance;
    std::vector<FlightCrewRecord>& m_flightCrew;
    std::vector<AircraftRecord>& m_aircraft;

    FlightRepository& m_flightRepo;
    CrewRepository& m_crewRepo;
    MaintenanceRepository& m_maintenanceRepo;
    FlightCrewRepository& m_flightCrewRepo;
    AircraftRepository& m_aircraftRepo;

    void assignCrew();
    void scheduleMaintenance();
    void viewMaintenanceLogs();
    void viewAircraftStatus();

    std::string getNextMaintenanceID();
    std::string getNextCrewID();
};

#endif // FLIGHT_OPERATIONS_UI_HPP
