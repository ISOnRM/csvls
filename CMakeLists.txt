cmake_minimum_required(VERSION 4.0)

project(csvls LANGUAGES CXX)

add_executable(
	csvls
	src/main.cpp
	src/cli/ArgumentParser.cpp
	src/checker/ArgumentChecker.cpp
	src/assembler/Assembler.cpp
	src/csv_writer/CsvWriter.cpp
)

target_compile_features(
	csvls PRIVATE
	cxx_std_20
)

target_compile_options(
	csvls PRIVATE
	-Wall -Wextra -Werror -Wpedantic -O3
)

set_target_properties(
	csvls PROPERTIES
	CXX_EXTENSIONS OFF
)

target_include_directories(
	csvls PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
)


install(TARGETS csvls)
