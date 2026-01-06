#ifndef USER_REPOSITORY_HPP
#define USER_REPOSITORY_HPP

#include "../core/UserRecord.hpp"
#include <vector>
#include <string>

/**
 * @brief Repository for persisting users to/from JSON.
 * 
 * This class handles all user data I/O. It uses UserRecord (a simple struct)
 * and converts it to/from JSON format.
 * 
 * The actual file path defaults to "data/users.json" but can be customized.
 */
class UserRepository {
public:
    /**
     * @brief Constructor.
     * 
     * @param dataFilePath Path to the users JSON file. Defaults to "data/users.json".
     */
    explicit UserRepository(const std::string& dataFilePath = "data/users.json");

    /**
     * @brief Load all users from the JSON file.
     * 
     * If the file doesn't exist, returns an empty vector.
     * If the file exists but is empty/invalid, throws an exception.
     * 
     * @return A vector of UserRecord objects loaded from disk.
     * @throws std::runtime_error if file exists but can't be read or JSON is invalid.
     */
    std::vector<UserRecord> loadAll();

    /**
     * @brief Save all users to the JSON file.
     * 
     * Overwrites the file with the current users vector.
     * 
     * @param users Vector of UserRecord objects to save.
     * @throws std::runtime_error if file can't be written.
     */
    void saveAll(const std::vector<UserRecord>& users);

private:
    std::string m_dataFilePath;
};

#endif // USER_REPOSITORY_HPP
