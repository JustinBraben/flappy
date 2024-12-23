# Specify the minimum required CMake version
cmake_minimum_required(VERSION 3.21)

# Only set the cxx_standard if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# Set the variable projectName to "flappy"
set(projectName "flappy")

# Declare the project with the specified language (C++)
project(${projectName} LANGUAGES CXX)

# Set the output directory for executables to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define an option to build shared libraries (default is OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Include FetchContent module for managing external dependencies
include(FetchContent)

# Fetch and declare SFML as an external dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Fetch and declare EnTT as an external dependency
FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.14.0)  # Specify the desired tag or commit
FetchContent_MakeAvailable(EnTT)

# Debug info on where the binary directory is
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")

# Create an executable target named "flappy" with the source file src/main.cpp
add_executable(${projectName} src/main.cpp "src/core/app.cpp" "src/core/factories.cpp" "src/sys/pipe_behaviour.cpp")

# Copy assets to Binary directory
file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")

# Link the executable against SFML and EnTT libraries
target_link_libraries(${projectName} PRIVATE SFML::Graphics EnTT::EnTT)

# Set the C++ standard to C++17 for the "flappy" target
target_compile_features(${projectName} PRIVATE cxx_std_17)

# Install the "flappy" target
install(TARGETS ${projectName})