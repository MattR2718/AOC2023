cmake_minimum_required (VERSION 3.8)

include(CheckLanguage)
check_language(CUDA)

set_source_files_properties(day5CUDA.cpp PROPERTIES LANGUAGE CUDA)

find_package(CUDA REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")

add_executable(day5CUDA "day5CUDA.cpp" kernel.cu)

add_custom_command(
	TARGET day5CUDA PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day5CUDA/input.txt
	${CMAKE_CURRENT_BINARY_DIR}/input.txt
)

add_custom_command(
	TARGET day5CUDA PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	${CMAKE_SOURCE_DIR}/day5CUDA/test.txt
	${CMAKE_CURRENT_BINARY_DIR}/test.txt
)