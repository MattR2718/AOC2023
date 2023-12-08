cmake_minimum_required (VERSION 3.8)

add_executable(day8 "day8.cpp")

add_custom_command(
	TARGET day8 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day8/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day8 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day8/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)