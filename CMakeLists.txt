# CMakeList.txt : CMake project for AOC2023, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.25)

project ("AOC2023")
# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


include(FetchContent)

#set(Boost_DEBUG on)

# Check for cuda for day5 implementation
include(CheckLanguage)
check_language(CUDA)

# Fetch Boost with only the components needed
set(BOOST_INCLUDE_LIBRARIES iostreams icl range CACHE STRING "" FORCE)
set(BOOST_ENABLE_CMAKE ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG        boost-1.85.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(Boost)

# Get ctre for faster regex
FetchContent_Declare( ctre
    GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions
    GIT_TAG v3.9.0
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
if(CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
  add_subdirectory(day5CUDA)
endif()
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
add_subdirectory(day17)
add_subdirectory(day18)

# if (CMAKE_VERSION VERSION_GREATER 3.12)
#   set_property(TARGET AOC2023 PROPERTY CXX_STANDARD 20)
# endif()

# TODO: Add tests and install targets if needed.
