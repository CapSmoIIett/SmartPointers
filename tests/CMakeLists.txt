enable_testing()


add_executable(
  Tests 
  tests.cpp
)

include(GoogleTest)
gtest_discover_tests(Simple)