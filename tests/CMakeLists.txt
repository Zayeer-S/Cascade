include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE UNIT_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp")
file(GLOB_RECURSE INTEGRATION_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/integration/*.cpp")

add_executable(cascade_unit_tests ${UNIT_TESTS})
target_link_libraries(cascade_unit_tests PRIVATE cascade_lib gtest gtest_main)
add_test(NAME unit_tests COMMAND cascade_unit_tests)

add_executable(cascade_integration_tests ${INTEGRATION_TESTS})
target_link_libraries(cascade_integration_tests PRIVATE cascade_lib gtest gtest_main)
add_test(NAME integration_tests COMMAND cascade_integration_tests)
