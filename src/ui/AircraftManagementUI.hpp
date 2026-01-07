#ifndef AIRCRAFT_MANAGEMENT_UI_HPP
#define AIRCRAFT_MANAGEMENT_UI_HPP

#include "../core/AircraftRecord.hpp"
#include "../persistence/AircraftRepository.hpp"
#include <vector>
#include <string>

/**
 * @brief UI for administrator aircraft fleet management.
 */
class AircraftManagementUI {
public:
    explicit AircraftManagementUI(AircraftRepository& aircraftRepo);

    void showAircraftManagementMenu();

private:
    AircraftRepository& m_aircraftRepo;
    std::vector<AircraftRecord> m_aircraft;

    void addAircraft();
    void updateAircraft();
    void removeAircraft();
    void listAllAircraft();
    void viewAircraftStatus();

    std::string getNextAircraftID();
};

#endif // AIRCRAFT_MANAGEMENT_UI_HPP
