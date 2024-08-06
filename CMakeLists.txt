CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(kdtree)

set(CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

add_subdirectory("lib/googletest")
add_subdirectory("tests")
