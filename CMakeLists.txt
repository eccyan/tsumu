cmake_minimum_required(VERSION 3.1)

if(CMAKE_MAJOR_VERSION LESS 3)
    message(WARNING "Building with CMake 3.0 or higher is strongly suggested; current version is ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
endif()

set(PROJECT_VERSION "1.0.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall")

project(tsumu VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_executable(${PROJECT_NAME} tsumu/main.cc)

include_directories(lib/fmt/include)
add_subdirectory(lib/fmt)
