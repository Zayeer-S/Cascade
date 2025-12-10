cmake_minimum_required(VERSION 3.15)
project(CascadeEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build test suite" ON)
option(BUILD_GUI "Build SDL GUI (requires SDL2)" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

if(ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    endif()
endif()

if(ENABLE_SANITIZERS)
    if(NOT MSVC)
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
endif()

if(BUILD_GUI)
    find_package(SDL2 QUIET)
    if(SDL2_FOUND)
        message(STATUS "SDL2 found - building GUI")
        set(GUI_ENABLED TRUE)
    else()
        message(STATUS "SDL2 not found - skipping GUI")
        message(STATUS "\tTo install SDL2:")
        message(STATUS "\t\tWindows (vcpkg): vcpkg install sdl2:x64-windows")
        message(STATUS "\t\tLinux: sudo apt install libsdl2-dev")
        message(STATUS "\t\tmacOS: brew install sdl2")
        set(GUI_ENABLED FALSE)
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CASCADE_SOURCES
    src/core/time.cpp
    src/core/simulation.cpp
    src/utils/config.cpp
    src/grid/cell.cpp
    src/grid/grid.cpp
    src/fire/simple_fire_model.cpp
    src/fire/fire_model.cpp
    src/suppression/suppression_agent.cpp
    src/suppression/aggressive_agent.cpp
    src/suppression/defensive_agent.cpp
)

if(GUI_ENABLED)
    list(APPEND CASCADE_SOURCES
        src/gui/sdl_renderer.cpp
        src/gui/game_ui.cpp
    )
endif()

add_library(cascade STATIC ${CASCADE_SOURCES})

target_include_directories(cascade PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(GUI_ENABLED)
    target_link_libraries(cascade PUBLIC SDL2::SDL2)
    target_compile_definitions(cascade PUBLIC CASCADE_GUI_ENABLED)
endif()

set_target_properties(cascade PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)

if(BUILD_TESTS)
    enable_testing()
    
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        message(STATUS "Google Test found - building with GTest")
        include(GoogleTest)
        set(USE_GTEST TRUE)
    else()
        message(STATUS "Google Test not found, skipping tests. To install:")
        message(STATUS "\tWindows: vcpkg install gtest")
        message(STATUS "\tLinux: sudo apt install libgtest-dev")
        message(STATUS "\tOr set GTest_DIR to your Google Test installation")
        set(USE_GTEST FALSE)
    endif()
    
    add_subdirectory(tests)
endif()

message(STATUS "Cascade Engine Configuration:")
message(STATUS "\tVersion: ${PROJECT_VERSION}")
message(STATUS "\tC++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "\tBuild Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "\tWarnings Enabled: ${ENABLE_WARNINGS}")
message(STATUS "\tSanitizers Enabled: ${ENABLE_SANITIZERS}")
message(STATUS "\tBuild Tests: ${BUILD_TESTS}")