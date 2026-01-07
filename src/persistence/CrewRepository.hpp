#ifndef CREW_REPOSITORY_HPP
#define CREW_REPOSITORY_HPP

#include "../core/CrewRecord.hpp"
#include <vector>
#include <string>
#include <optional>

/**
 * @brief Repository for persisting crew members to/from JSON.
 */
class CrewRepository {
public:
    explicit CrewRepository(const std::string& dataFilePath = "data/crew.json");

    std::vector<CrewRecord> loadAll();
    void saveAll(const std::vector<CrewRecord>& crew);

    std::optional<CrewRecord> findByID(
        const std::string& crewID,
        const std::vector<CrewRecord>& crew
    );

    std::vector<CrewRecord> findByRole(
        const std::string& role,
        const std::vector<CrewRecord>& crew
    );

    std::vector<CrewRecord> findAvailable(
        const std::vector<CrewRecord>& crew
    );

private:
    std::string m_dataFilePath;
};

#endif // CREW_REPOSITORY_HPP
