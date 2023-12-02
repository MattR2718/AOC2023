cmake_minimum_required (VERSION 3.8)

add_executable(day2 "day2.cpp")

add_custom_command(
	TARGET day2 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day2/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day2 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day2/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)