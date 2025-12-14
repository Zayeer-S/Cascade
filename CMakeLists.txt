cmake_minimum_required(VERSION 3.15)
project(CascadeEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build test suite" ON)
option(BUILD_GUI "Build SDL GUI (requires SDL2)" ON)
option(CASCADE_ENABLE_WARNINGS "Enable compiler warnings" ON)
option(CASCADE_ENABLE_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)

if(CASCADE_ENABLE_WARNINGS)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

if(CASCADE_ENABLE_SANITIZERS AND NOT MSVC)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

if(BUILD_GUI)
    find_package(SDL2 QUIET CONFIG)
    if(SDL2_FOUND)
        message(STATUS "SDL2 found - GUI will be built")
        set(GUI_ENABLED TRUE)
    else()
        message(STATUS "SDL2 not found - GUI will be skipped")
        set(GUI_ENABLED FALSE)
    endif()
else()
    set(GUI_ENABLED FALSE)
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
    src/suppression/agent_factory.cpp
    src/suppression/aggressive_agent.cpp
    src/suppression/defensive_agent.cpp
    src/suppression/suppression_agent.cpp
    src/utils/agent_config.cpp
    src/utils/config_builder.cpp
    src/utils/input_helper.cpp
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

if(EXISTS "${PROJECT_SOURCE_DIR}/src/main.cpp")
    message(STATUS "Found src/main.cpp - building cascade_app executable")
    
    add_executable(cascade_app src/main.cpp)
    
    target_link_libraries(cascade_app PRIVATE cascade)
    
    if(GUI_ENABLED AND WIN32)
        target_link_libraries(cascade_app PRIVATE SDL2::SDL2main)
    endif()
    
    set_target_properties(cascade_app PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/RelWithDebInfo
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/MinSizeRel
    )
    
    if(GUI_ENABLED AND WIN32)
        add_custom_command(TARGET cascade_app POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:cascade_app>
            COMMENT "Copying SDL2 DLL to output directory"
        )
    endif()
    
    message(STATUS "Executable will be built at: ${CMAKE_BINARY_DIR}/bin/$<CONFIG>/cascade_app.exe")
else()
    message(STATUS "src/main.cpp not found - skipping executable build")
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

message(STATUS "")
message(STATUS "Cascade Engine Configuration:")
message(STATUS "\tVersion: ${PROJECT_VERSION}")
message(STATUS "\tC++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "\tBuild Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "\tWarnings Enabled: ${CASCADE_ENABLE_WARNINGS}")
message(STATUS "\tSanitizers Enabled: ${CASCADE_ENABLE_SANITIZERS}")
message(STATUS "\tGUI Enabled: ${GUI_ENABLED}")
message(STATUS "\tBuild Tests: ${BUILD_TESTS}")
message(STATUS "")