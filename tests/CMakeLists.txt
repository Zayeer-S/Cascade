find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(STATUS "Google Test not found - tests will not be built")
    message(STATUS "\tTo install Google Test:")
    message(STATUS "\t\tWindows (vcpkg): vcpkg install gtest:x64-windows")
    message(STATUS "\t\tLinux: sudo apt install libgtest-dev")
    message(STATUS "\t\tmacOS: brew install googletest")
    message(STATUS "\t\tOr set GTest_DIR to your Google Test installation")
    return()
endif()

message(STATUS "Google Test found - building test suite")

enable_testing()
include(GoogleTest)

file(GLOB_RECURSE UNIT_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp"
)

file(GLOB INTEGRATION_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.cpp"
)

if(UNIT_TEST_SOURCES)
    add_executable(unit_tests ${UNIT_TEST_SOURCES})
    target_link_libraries(unit_tests PRIVATE 
        cascade 
        GTest::gtest 
        GTest::gtest_main
    )
    
    set_target_properties(unit_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )
    
    gtest_discover_tests(unit_tests)
    
    message(STATUS "Unit tests executable: ${CMAKE_BINARY_DIR}/bin/tests/unit_tests")
else()
    message(STATUS "No unit test sources found in tests/unit/")
endif()

if(INTEGRATION_TEST_SOURCES)
    add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})
    target_link_libraries(integration_tests PRIVATE 
        cascade 
        GTest::gtest 
        GTest::gtest_main
    )
    
    set_target_properties(integration_tests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )
    
    gtest_discover_tests(integration_tests)
    
    message(STATUS "Integration tests executable: ${CMAKE_BINARY_DIR}/bin/tests/integration_tests")
else()
    message(STATUS "No integration test sources found in tests/integration/")
endif()