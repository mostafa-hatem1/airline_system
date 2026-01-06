#ifndef RESERVATION_REPOSITORY_HPP
#define RESERVATION_REPOSITORY_HPP

#include "../core/ReservationRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting reservations to/from JSON.
 */
class ReservationRepository {
public:
    /**
     * @brief Constructor.
     * 
     * @param dataFilePath Path to the reservations JSON file.
     */
    explicit ReservationRepository(const std::string& dataFilePath = "data/reservations.json");

    /**
     * @brief Load all reservations from the JSON file.
     * 
     * @return A vector of ReservationRecord objects.
     * @throws std::runtime_error if file can't be read.
     */
    std::vector<ReservationRecord> loadAll();

    /**
     * @brief Save all reservations to the JSON file.
     * 
     * @param reservations Vector of ReservationRecord objects to save.
     * @throws std::runtime_error if file can't be written.
     */
    void saveAll(const std::vector<ReservationRecord>& reservations);

    /**
     * @brief Find a reservation by reservation ID.
     * 
     * @param reservationID The ID to search for.
     * @param reservations The vector to search in.
     * @return The reservation if found; std::nullopt otherwise.
     */
    std::optional<ReservationRecord> findByID(
        const std::string& reservationID,
        const std::vector<ReservationRecord>& reservations
    );

    /**
     * @brief Find all reservations for a specific passenger.
     * 
     * @param passengerID The passenger ID to search for.
     * @param reservations The vector to search in.
     * @return A vector of matching reservations.
     */
    std::vector<ReservationRecord> findByPassengerID(
        const std::string& passengerID,
        const std::vector<ReservationRecord>& reservations
    );

    /**
     * @brief Find all reservations for a specific flight.
     * 
     * @param flightNumber The flight number to search for.
     * @param reservations The vector to search in.
     * @return A vector of matching reservations.
     */
    std::vector<ReservationRecord> findByFlightNumber(
        const std::string& flightNumber,
        const std::vector<ReservationRecord>& reservations
    );

private:
    std::string m_dataFilePath;
};

#endif // RESERVATION_REPOSITORY_HPP
