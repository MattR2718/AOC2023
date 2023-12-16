cmake_minimum_required (VERSION 3.8)

add_executable(day16 "day16.cpp")

add_custom_command(
	TARGET day16 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day16/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day16 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day16/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)