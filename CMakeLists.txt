cmake_minimum_required(VERSION 3.16)
project(AirlineReservationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json 3.12.0 REQUIRED)

add_executable(airline_system
    src/main.cpp
    src/persistence/JsonFile.cpp
    src/persistence/UserRepository.cpp
    src/utils/Role.cpp
    src/utils/ConsoleIO.cpp
    src/utils/PasswordHash.cpp
    src/managers/AuthService.cpp
    src/ui/MainMenu.cpp
)

target_link_libraries(airline_system PRIVATE nlohmann_json::nlohmann_json)
