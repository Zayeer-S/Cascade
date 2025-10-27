# Collect .cpp files, all of em, recursively
file(GLOB_RECURSE CASCADE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# Build as lib
add_library(cascade_lib ${CASCADE_SOURCES})

target_include_directories(cascade_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Warnings
if (CASCADE_ENABLE_WARNINGS)
    if (MSVC)
        target_compile_options(cascade_lib PRIVATE /W4)
    else ()
        target_compile_options(cascade_lib PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

# Sanitizers (Debug only)
if (CASCADE_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(cascade_lib PRIVATE -fsanitize=address,undefined)
    target_link_libraries(cascade_lib PRIVATE -fsanitize=address,undefined)
endif()

if (EXISTS "${PROJECT_SOURCE_DIR}/src/main.cpp")
    add_executable(cascade src/main.cpp)
    target_link_libraries(cascade PRIVATE cascade_lib)
endif()