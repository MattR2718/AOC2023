cmake_minimum_required (VERSION 3.8)

add_executable(day6 "day6.cpp")

add_custom_command(
	TARGET day6 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day6/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day6 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day6/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)