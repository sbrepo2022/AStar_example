cmake_minimum_required(VERSION 3.10)
project(A_star)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
add_definitions(-g)

add_executable(
    ${PROJECT_NAME}

    "src/a_star.h"
    "src/state.h"
    "src/helpers.h"

    "src/a_star.cpp"
    "src/state.cpp"
    "src/main.cpp"
)

target_include_directories(${PROJECT_NAME} PRIVATE "./libs")
target_include_directories(${PROJECT_NAME} PRIVATE "./src")