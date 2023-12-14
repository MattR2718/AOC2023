cmake_minimum_required (VERSION 3.8)

add_executable(day13 "day13.cpp")

add_custom_command(
	TARGET day13 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day13/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day13 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day13/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)