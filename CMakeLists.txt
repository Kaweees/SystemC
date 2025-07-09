cmake_minimum_required(VERSION 3.14 FATAL_ERROR) # Set the minimum required
                                                 # version of CMake

project(
  kiwimodel
  VERSION 1.0.0
  LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Library target
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
  ${PROJECT_NAME}
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>)

# Find SystemC using SYSTEMC_HOME environment variable
set(SYSTEMC_HOME $ENV{SYSTEMC_HOME})
if(NOT SYSTEMC_HOME)
  message(
    FATAL_ERROR
      "SYSTEMC_HOME environment variable not set. Please set it to the SystemC installation directory."
  )
endif()

# Find SystemC library
find_library(
  SYSTEMC_LIBRARY
  NAMES systemc
  PATHS ${SYSTEMC_HOME}/lib
  NO_DEFAULT_PATH)

# Find SystemC headers
find_path(
  SYSTEMC_INCLUDE_DIR
  NAMES systemc.h
  PATHS ${SYSTEMC_HOME}/include
  NO_DEFAULT_PATH)

if(NOT SYSTEMC_LIBRARY OR NOT SYSTEMC_INCLUDE_DIR)
  message(FATAL_ERROR "SystemC not found at ${SYSTEMC_HOME}")
endif()

# Link SystemC to the interface library
target_link_libraries(${PROJECT_NAME} INTERFACE ${SYSTEMC_LIBRARY})
target_include_directories(${PROJECT_NAME} INTERFACE ${SYSTEMC_INCLUDE_DIR})

# Add SystemC compile definitions
target_compile_definitions(${PROJECT_NAME}
                           INTERFACE SC_INCLUDE_DYNAMIC_PROCESSES)

include(CheckLanguage)
include(FetchContent)

# Optional builds
option(BUILD_TB "Build testbenches" ON)

# Examples
if(BUILD_TB)
  add_subdirectory(tb)
endif()
