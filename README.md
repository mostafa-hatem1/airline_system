# Airline Reservation System

A comprehensive C++ console-based application for managing airline operations. This system handles the complete lifecycle of airline management, from user authentication and role-based access to flight scheduling, seat reservations, and aircraft maintenance.

## Table of Contents

- [Features](#features)
- [Architecture Overview](#architecture-overview)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Technical Stack](#technical-stack)
- [Configuration](#configuration)
- [Contributing](#contributing)
- [License](#license)

## Features

- **Role-Based Access Control**: Secure login system supporting multiple roles:
  - **Administrator**: Full system access, including user and fleet management.
  - **Booking Agent**: Manages reservations and passenger bookings.
  - **Passenger**: Searches for flights and manages personal bookings.
- **Flight Management**: Schedule new flights, update routes, and manage flight status.
- **Reservation System**:
  - Real-time seat availability map.
  - Booking and cancellation capabilities.
  - Passenger itinerary management.
- **Fleet Management**:
  - Track aircraft status and specifications.
  - Monitor maintenance schedules and logs.
- **Crew Management**: Assign pilots and cabin crew to specific flights.
- **Data Persistence**: All system data is persistently stored using JSON files, ensuring data integrity across sessions.

## Architecture Overview

The application follows a modular Layered Architecture pattern to ensure separation of concerns and maintainability:

1.  **Presentation Layer (`src/ui`)**: Handles user interaction via the console. It routes user commands to the appropriate service managers.
2.  **Business Logic Layer (`src/managers`)**: Contains the core business rules.
    - `AuthService`: Manages authentication and authorization.
    - `BookingService`: Handles reservation logic.
    - `FlightSearchService`: Processes flight queries.
3.  **Persistence Layer (`src/persistence`)**: responsible for data access. It uses the Repository pattern to abstract the underlying JSON storage mechanism.
4.  **Core Domain (`src/core`)**: Defines the fundamental data models (Entities) used throughout the system.

## Project Structure

```
.
├── CMakeLists.txt       # Build configuration
├── data/                # JSON data files (persistence storage)
│   ├── users.json
│   ├── flights.json
│   ├── ...
├── src/                 # Source code
│   ├── core/            # Domain entities (e.g., User, Flight)
│   ├── managers/        # Business logic services
│   ├── persistence/     # Data repositories and JSON handling
│   ├── ui/              # Console user interface
│   ├── utils/           # Utility classes (Hashing, Enums)
│   └── main.cpp         # Application entry point
└── README.md
```

## Prerequisites

- **C++ Compiler**: A compiler supporting C++17 (e.g., GCC 7+, Clang 5+, MSVC 19.14+).
- **CMake**: Version 3.16 or higher.
- **Build System**: Make or Ninja.
- **nlohmann_json**: This project depends on the `nlohmann/json` library.

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install nlohmann-json3-dev cmake build-essential
```

**macOS (Homebrew):**
```bash
brew install nlohmann-json cmake
```

## Installation

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/yourusername/AirlineReservationSystem.git
    cd AirlineReservationSystem
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure the project with CMake:**
    ```bash
    cmake ..
    ```

4.  **Build the application:**
    ```bash
    make
    ```

## Usage

1.  **Run the application:**
    From the `build` directory:
    ```bash
    ./airline_system
    ```

2.  **Log In:**
    The system comes pre-populated with default users in `data/users.json`. You can use the following credentials to test different roles:

    | Role            | Email (Username)       | Note |
    |-----------------|------------------------|------|
    | **Administrator** | `admin@airline.com`    | Has full access. |
    | **Booking Agent** | `jane@airline.com`     | Can manage bookings. |
    | **Passenger**     | `mike@example.com`     | Can search and book flights. |

    **Note on Passwords:**
    The passwords in `data/users.json` are hashed using FNV-1a 64-bit. As a security measure, default passwords are not hardcoded.

    To log in as the **Administrator**, you can reset the password to `password` by updating `data/users.json`:

    Replace the `passwordHash` for `admin@airline.com` with:
    ```
    "4b1a493507b3a318"
    ```
    *(This hash corresponds to the plaintext string "password")*

## Technical Stack

- **Language**: C++17
- **Build System**: CMake
- **Libraries**:
  - [nlohmann/json](https://github.com/nlohmann/json) - For JSON parsing and serialization.
- **Platform**: Cross-platform (Linux, macOS, Windows).

## Configuration

The application relies on JSON files located in the `data/` directory. These files act as the database for the system.
- Ensure the `data/` directory exists in the project root relative to where you run the executable.
- Key configuration files:
  - `users.json`: Stores user credentials and roles.
  - `flights.json`: Stores flight schedules and routes.
  - `aircraft.json`: Stores fleet information.

## Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a feature branch (`git checkout -b feature/AmazingFeature`).
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`).
4. Push to the branch (`git push origin feature/AmazingFeature`).
5. Open a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
