#include "JsonFile.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

json JsonFile::readJsonFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filePath);
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        throw std::runtime_error(
            "JSON parsing error in file '" + filePath + "': " + std::string(e.what())
        );
    }

    return data;
}

void JsonFile::writeJsonToFile(const std::string& filePath, const json& data) {
    // Create parent directories if they don't exist
    fs::path path(filePath);
    fs::create_directories(path.parent_path());

    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }

    // Write with 2-space indentation for readability
    file << data.dump(2) << std::endl;

    if (file.fail()) {
        throw std::runtime_error("Error writing to file: " + filePath);
    }
}
