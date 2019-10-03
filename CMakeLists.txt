cmake_minimum_required(VERSION 3.10)

project(Digestible VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wpedantic -Wshadow -fstack-protector-strong")

# Only do this stuff if we're the top-level project
# and not included via add_subdirectory()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  set(CMAKE_CXX_EXTENSIONS OFF)

  include(CTest)

  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()

endif()

add_subdirectory(utility)
