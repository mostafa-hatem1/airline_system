#include "PasswordHash.hpp"

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string_view>

namespace {
uint64_t fnv1a64(std::string_view s) {
    uint64_t h = 14695981039346656037ull;      // offset basis
    for (unsigned char c : s) {
        h ^= static_cast<uint64_t>(c);
        h *= 1099511628211ull;                // FNV prime
    }
    return h;
}
} // namespace

std::string PasswordHash::hash(const std::string& plaintext) {
    const uint64_t h = fnv1a64(plaintext);
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << h;
    return oss.str();
}

bool PasswordHash::verify(const std::string& plaintext, const std::string& storedHash) {
    return hash(plaintext) == storedHash;
}
