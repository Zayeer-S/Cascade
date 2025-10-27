cmake_minimum_required(VERSION 3.20)

project(Cascade
    VERSION 0.1.0
    LANGUAGES CXX
)

# Global C++ Config
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CASCADE_ENABLE_WARNINGS "Enable strict warnings" ON)
option (CASCADE_ENABLE_SANITIZERS "Enable sanitizersi in Debug builds" ON)

# Include Folders
include_directories(include)

# Add src + modules
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)
