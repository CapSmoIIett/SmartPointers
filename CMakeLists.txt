# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SmartPtrImplementation")

#add_subdirectory(Test)
add_subdirectory(googletest)

# Include sub-projects.
add_subdirectory ("src")
#add_subdirectory ("tests")
