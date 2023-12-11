cmake_minimum_required (VERSION 3.8)

add_executable(day11 "day11.cpp")
target_link_libraries(day11 Boost::iostreams)

add_custom_command(
	TARGET day11 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day11/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day11 PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day11/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)