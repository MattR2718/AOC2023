cmake_minimum_required (VERSION 3.8)

add_executable(day10 "day10.cpp")

add_custom_command(
	TARGET day10 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day10/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day10 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day10/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)