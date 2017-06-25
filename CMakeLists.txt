cmake_minimum_required(VERSION 3.7)
project(interval)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_TESTS "Disable unit tests for interval library." on)


include_directories(include)
file(COPY ${PROJECT_SOURCE_DIR}/include DESTINATION ${PROJECT_BINARY_DIR})


if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
