cmake_minimum_required (VERSION 3.8)

add_executable(day5 "day5.cpp")

add_custom_command(
	TARGET day5 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day5/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day5 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day5/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)