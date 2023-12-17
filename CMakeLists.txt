# CMakeList.txt : CMake project for AOC2023, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set C++ standard to 20
set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("AOC2023")

include(FetchContent)

#set(Boost_DEBUG on)

# Check for cuda for day5 implementation
find_package(CUDA)

find_package(Boost 1.83.0 REQUIRED COMPONENTS iostreams)
include_directories(${Boost_INCLUDE_DIRS})

# Get ctre for faster regex
FetchContent_Declare( ctre 
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions
    GIT_TAG origin/main
    )
FetchContent_MakeAvailable(ctre)


# Add source to this project's executable.
add_executable (AOC2023 "AOC2023.cpp" "AOC2023.h")

# Add all subdirectories for days
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
if(CUDA_FOUND)
	enable_language(CUDA)
	set(CMAKE_CUDA_COMPILER nvcc)
  add_subdirectory(day5CUDA)
endif(CUDA_FOUND)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)
add_subdirectory(day15)
add_subdirectory(day16)

# if (CMAKE_VERSION VERSION_GREATER 3.12)
#   set_property(TARGET AOC2023 PROPERTY CXX_STANDARD 20)
# endif()

# TODO: Add tests and install targets if needed.
