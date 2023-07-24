# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SmartPtr")

enable_testing()

#add_subdirectory(Test)
include_directories("googletest/googletest/include")

add_subdirectory("googletest")
add_subdirectory ("src")
add_subdirectory ("tests")
