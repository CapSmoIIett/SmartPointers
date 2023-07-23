# CMakeList.txt : CMake project for SmartPtrImplementation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (SmartPtr "main.cpp" "header.h" "unique_ptr.h" "weak_ptr.h" "Widget.h" "defaultmemoryfunctions.h")

