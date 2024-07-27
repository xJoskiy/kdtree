set(BINARY kdtree_test)

enable_testing()

include_directories("../src")
file(GLOB_RECURSE src "test.cpp")

add_executable(${BINARY} ${src})
add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} gtest_main gtest gmock)
