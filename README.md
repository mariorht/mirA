# FILE: /mir-compositor/mir-compositor/README.md

# Mir Compositor

This project is a simple compositor based on the MIR framework from Canonical. It serves as an example of how to create a basic window management system using MIR.

## Project Structure

```
mir-compositor
├── src
│   ├── main.cpp          # Entry point of the application
│   └── compositor.cpp     # Implementation of the Compositor class
├── include
│   └── compositor.h       # Declaration of the Compositor class
├── CMakeLists.txt        # CMake configuration file
├── .gitignore            # Git ignore file
└── README.md             # Project documentation
```

## Requirements

- CMake (version 3.10 or higher)
- MIR library

## Setup Instructions

1. Clone the repository:
   ```
   git clone <repository-url>
   cd mir-compositor
   ```

2. Create a build directory:
   ```
   mkdir build
   cd build
   ```

3. Configure the project using CMake:
   ```
   cmake ..
   ```

4. Build the project:
   ```
   make
   ```

## Usage

After building the project, you can run the compositor with the following command:
```
./mir-compositor
```
