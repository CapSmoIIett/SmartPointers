enable_testing()

#include_directories("../googletest/googletest/include/")

add_executable(
  Tests 
  tests.cpp
)

target_link_libraries(Tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Tests)