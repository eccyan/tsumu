cmake_minimum_required(VERSION 3.1)

if(CMAKE_MAJOR_VERSION LESS 3)
    message(WARNING "Building with CMake 3.0 or higher is strongly suggested; current version is ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
endif()

set(PROJECT_VERSION "1.0.0")
project(tsumu
  VERSION ${PROJECT_VERSION}
  LANGUAGES CXX
)

include(ExternalProject)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Update compiler waring flags
#
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()

set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)

# fmt
ExternalProject_Add(
  fmt_dep
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  PREFIX ${EXTERNAL_DIR}/fmt
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_DIR}/build
)

add_library(fmt STATIC IMPORTED)
set_property(
  TARGET fmt PROPERTY IMPORTED_LOCATION
  ${EXTERNAL_DIR}/build/lib/libfmt.a
)
include_directories(
  ${EXTERNAL_DIR}/build/include
)

# main
set(TSUMU_SRC
  src/main.cc
)

add_executable(${PROJECT_NAME} ${TSUMU_SRC})
add_dependencies(${PROJECT_NAME} fmt_dep)
target_link_libraries(
  ${PROJECT_NAME}
  fmt
)
