cmake_minimum_required (VERSION 3.8)

add_executable(day7 "day7.cpp")

add_custom_command(
	TARGET day7 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day7/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day7 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day7/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)