set(BINARY kdtree_test)

enable_testing()

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("../src")
file(GLOB_RECURSE src "test.cpp")

add_executable(${BINARY} ${src})
add_test(NAME ${BINARY} COMMAND ${BINARY})
set_property(TARGET ${BINARY} PROPERTY CXX_STANDARD ${CXX_STANDARD})

target_link_libraries(${BINARY} gtest_main gtest gmock)
