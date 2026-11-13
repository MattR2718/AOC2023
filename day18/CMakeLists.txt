cmake_minimum_required (VERSION 3.8)

add_executable(day18 "day18.cpp")
add_executable(day18-2026 "day18-2026.cpp")
target_link_libraries(day18-2026 ctre::ctre)

add_custom_command(
 TARGET day18 PRE_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy
 ${CMAKE_SOURCE_DIR}/day18/input.txt
 ${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
 TARGET day18 PRE_BUILD
 COMMAND ${CMAKE_COMMAND} -E copy
 ${CMAKE_SOURCE_DIR}/day18/test.txt
 ${CMAKE_CURRENT_BINARY_DIR}/test.txt
)
