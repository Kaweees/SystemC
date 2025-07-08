enable_testing()

add_executable(${PROJECT_NAME}_tests test_library.cpp)
target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        ${PROJECT_NAME}
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)

# Setting our output directory
set_target_properties(${PROJECT_NAME}_tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/target/release/"
  OUTPUT_NAME ${PROJECT_NAME}_tests
)
