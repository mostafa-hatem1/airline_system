#ifndef JSON_FILE_HPP
#define JSON_FILE_HPP

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

/**
 * @brief Utility functions for JSON file I/O.
 * 
 * These are low-level file operations; they don't know about domain objects.
 * They just load/save JSON from/to disk.
 */
namespace JsonFile {

/**
 * @brief Read a JSON file from disk.
 * 
 * @param filePath Path to the JSON file (relative or absolute).
 * @return The parsed JSON object; throws if file doesn't exist or is invalid JSON.
 * @throws std::runtime_error if file cannot be read.
 * @throws nlohmann::json::parse_error if JSON is malformed.
 */
json readJsonFromFile(const std::string& filePath);

/**
 * @brief Write a JSON object to disk.
 * 
 * Creates the file if it doesn't exist; overwrites if it does.
 * 
 * @param filePath Path to write to (relative or absolute).
 * @param data The JSON object to write.
 * @throws std::runtime_error if file cannot be written.
 */
void writeJsonToFile(const std::string& filePath, const json& data);

} // namespace JsonFile

#endif // JSON_FILE_HPP
