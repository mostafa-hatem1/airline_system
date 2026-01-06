#include "UserRepository.hpp"
#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

UserRepository::UserRepository(const std::string& dataFilePath)
    : m_dataFilePath(dataFilePath) {
}

std::vector<UserRecord> UserRepository::loadAll() {
    std::vector<UserRecord> users;

    // If file doesn't exist, return empty vector
    if (!fs::exists(m_dataFilePath)) {
        std::cout << "User file does not exist: " << m_dataFilePath << "\n";
        return users;
    }

    try {
        json data = JsonFile::readJsonFromFile(m_dataFilePath);

        // Expect JSON structure: { "users": [ {...}, {...}, ... ] }
        if (!data.contains("users") || !data["users"].is_array()) {
            throw std::runtime_error(
                "Invalid user JSON structure: expected 'users' array"
            );
        }

        // Deserialize each user record
        for (const auto& userJson : data["users"]) {
            UserRecord user;
            user.userId = userJson.value("userId", "");
            user.username = userJson.value("username", "");
            user.passwordHash = userJson.value("passwordHash", "");
            user.role = userJson.value("role", "");
            user.email = userJson.value("email", "");
            user.phone = userJson.value("phone", "");

            users.push_back(user);
        }

    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to load users from " + m_dataFilePath + ": " + e.what()
        );
    }

    return users;
}

void UserRepository::saveAll(const std::vector<UserRecord>& users) {
    json data;
    data["users"] = json::array();

    // Serialize each user record
    for (const auto& user : users) {
        json userJson;
        userJson["userId"] = user.userId;
        userJson["username"] = user.username;
        userJson["passwordHash"] = user.passwordHash;
        userJson["role"] = user.role;
        userJson["email"] = user.email;
        userJson["phone"] = user.phone;

        data["users"].push_back(userJson);
    }

    try {
        JsonFile::writeJsonToFile(m_dataFilePath, data);
    } catch (const std::exception& e) {
        throw std::runtime_error(
            "Failed to save users to " + m_dataFilePath + ": " + e.what()
        );
    }
}
