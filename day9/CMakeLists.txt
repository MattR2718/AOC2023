cmake_minimum_required (VERSION 3.8)

add_executable(day9 "day9.cpp")

target_link_libraries(day9 PRIVATE ctre::ctre)

add_custom_command(
	TARGET day9 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day9/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day9 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day9/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)