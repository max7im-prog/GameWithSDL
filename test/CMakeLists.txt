add_executable(Game_tests mainTest.cpp)

# Put test executables in a predictable place
set_target_properties(Game_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(Game_tests GTest::gtest_main)

gtest_discover_tests(Game_tests
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
