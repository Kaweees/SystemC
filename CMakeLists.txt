cmake_minimum_required(VERSION 3.14 FATAL_ERROR) # Set the minimum required
                                                 # version of CMake

project(
  kiwicpp
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

include(CheckLanguage)
include(FetchContent)

# Metal Support (macOS/iOS)
if(APPLE)
  find_library(METAL_FRAMEWORK Metal)
  find_library(METALKIT_FRAMEWORK MetalKit)
  find_library(FOUNDATION_FRAMEWORK Foundation)

  if(METAL_FRAMEWORK AND METALKIT_FRAMEWORK)
    message(STATUS "Apple Metal found. Building with Metal support")
    enable_language(OBJCXX)
    set(CMAKE_OBJCXX_STANDARD 20)
    set(CMAKE_OBJCXX_STANDARD_REQUIRED ON)
    # Fetch Metal-cpp
    FetchContent_Declare(
      metal-cpp
      GIT_REPOSITORY https://github.com/rizajin/metal-cpp.git
      GIT_TAG "main"
      GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(metal-cpp)
    target_link_libraries(
      ${PROJECT_NAME} INTERFACE "-framework Metal" "-framework Foundation"
                                "-framework QuartzCore" "-framework MetalKit")
    add_compile_definitions("__METAL__")
  else()
    message(STATUS "Apple Metal not found. Building without Metal support")
  endif()
else()
  if(WIN32)
    # Forcing MSVC to use utf-8 encoding
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  endif(WIN32)

  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    message(STATUS "CUDA found. Building with CUDA support")
    enable_language(CUDA)
    set(CMAKE_CUDA_STANDARD 14)
    find_package(CUDA REQUIRED)
    include_directories(${CUDA_INCLUDE_DIRS})
    add_compile_definitions("__CUDA__")
  else()
    message(STATUS "CUDA not found. Building without CUDA support")
  endif()
endif()

# Optional builds
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_TESTS "Build tests" ON)
option(BENCHMARK "Build with benchmark mode" OFF)

# Benchmark mode
if(BENCHMARK)
  target_compile_definitions(${PROJECT_NAME} PRIVATE BENCHMARK_MODE)
endif()

# Examples
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
  # Fetch GoogleTest
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "v1.15.2"
    GIT_PROGRESS TRUE)
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  add_subdirectory(tests)
endif()
