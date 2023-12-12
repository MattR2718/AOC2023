cmake_minimum_required (VERSION 3.8)

add_executable(day12 "day12.cpp")

target_link_libraries(day12 PRIVATE ctre::ctre)

add_custom_command(
	TARGET day12 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day12/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day12 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day12/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)