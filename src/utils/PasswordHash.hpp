#ifndef PASSWORD_HASH_HPP
#define PASSWORD_HASH_HPP

#include <string>

/**
 * @brief Simple deterministic password hashing.
 * 
 * For a course project, this uses std::hash for simplicity.
 * In production, use bcrypt, argon2, or similar.
 */
namespace PasswordHash {

/**
 * @brief Hash a plaintext password.
 * 
 * @param plaintext The plaintext password.
 * @return A hashed string representation.
 */
std::string hash(const std::string& plaintext);

/**
 * @brief Verify a plaintext password against a stored hash.
 * 
 * @param plaintext The plaintext password to verify.
 * @param storedHash The stored hash to compare against.
 * @return true if the plaintext matches the hash, false otherwise.
 */
bool verify(const std::string& plaintext, const std::string& storedHash);

} // namespace PasswordHash

#endif // PASSWORD_HASH_HPP
