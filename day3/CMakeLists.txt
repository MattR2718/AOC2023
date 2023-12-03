cmake_minimum_required (VERSION 3.8)

add_executable(day3 "day3.cpp")

add_custom_command(
	TARGET day3 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day3/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day3 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day3/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)