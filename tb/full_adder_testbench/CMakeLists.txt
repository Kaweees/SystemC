set(EXECUTABLE_NAME full_adder_testbench)

file(GLOB_RECURSE PROJECT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE PROJECT_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_executable(${EXECUTABLE_NAME} ${PROJECT_SRCS})
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${PROJECT_INCLUDES})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${PROJECT_NAME})

# Setting our output directory
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/target/release/"
  OUTPUT_NAME ${EXECUTABLE_NAME}
)
