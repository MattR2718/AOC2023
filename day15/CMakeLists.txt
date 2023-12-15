cmake_minimum_required (VERSION 3.8)

add_executable(day15 "day15.cpp")
target_link_libraries(day15 PRIVATE ctre::ctre)

add_custom_command(
	TARGET day15 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day15/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day15 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day15/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)