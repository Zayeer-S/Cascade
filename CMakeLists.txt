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
option (CASCADE_ENABLE_SANITIZERS "Enable sanitizers in Debug builds" ON)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if (CLANG_TIDY_EXE)
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
    message(WARNING "clang-tidy not found - skipping linting.")
endif()

# Include Folders
include_directories(include)

# Add src + modules
add_subdirectory(src)

# Add tests
enable_testing()
add_subdirectory(tests)
