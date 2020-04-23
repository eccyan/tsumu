cmake_minimum_required(VERSION 3.1)

if(CMAKE_MAJOR_VERSION LESS 3)
    message(WARNING "Building with CMake 3.0 or higher is strongly suggested; current version is ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
endif()

project(tsumu
  VERSION ${PROJECT_VERSION}
  LANGUAGES CXX
)

set(PROJECT_VERSION "1.0.0")

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

set(TSUMU_SRC
  tsumu/main.cc
)

add_subdirectory(lib/fmt)
add_executable(${PROJECT_NAME} ${TSUMU_SRC})
