cmake_minimum_required (VERSION 3.8)

add_executable(day1 "day1.cpp")

add_custom_command(
	TARGET day1 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day1/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)