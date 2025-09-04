
enable_testing()

# List all test sources
set(TEST_SOURCES
    mainTest.cpp
    physics/world/testWorld.cpp
    registry/registryTest.cpp
)

# Create the test executable
add_executable(Game_tests ${TEST_SOURCES})

# Output directory
set_target_properties(Game_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Link Google Test and your project libraries
target_link_libraries(Game_tests
    GTest::gtest_main
    game_run_lib   # or whatever library contains World/Entity
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(Game_tests
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)