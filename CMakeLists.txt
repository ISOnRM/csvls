cmake_minimum_required(VERSION 4.0)

project(csvls LANGUAGES CXX)

set(SRCS src)

add_library(
	cli
	${SRCS}/cli/ArgumentParser.cpp
)

add_library(
	checker
	${SRCS}/checker/ArgumentChecker.cpp
)

add_library(
	assembler
	${SRCS}/assembler/Assembler.cpp
)

add_library(
	csv_writer
	${SRCS}/csv_writer/CsvWriter.cpp
)

#fmt
include(FetchContent)
FetchContent_Declare(
	fmt
	GIT_REPOSITORY https://github.com/fmtlib/fmt.git
	GIT_TAG 11.2.0
)

#set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(fmt)

#cli
target_compile_features(
	cli
	PUBLIC cxx_std_20
)

target_compile_options(
	cli
	PRIVATE -Wall -Wextra -Werror -Wpedantic -O3
)
#checker
target_compile_features(
	checker
	PUBLIC cxx_std_20
)

target_compile_options(
	checker
	PRIVATE -Wall -Wextra -Werror -Wpedantic -O3
)
#assembler
target_compile_features(
	assembler
	PUBLIC cxx_std_20
)

target_compile_options(
	assembler
	PRIVATE -Wall -Wextra -Werror -Wpedantic -O3
)
#csv_writer
target_compile_features(
	csv_writer
	PUBLIC cxx_std_20
)

target_compile_options(
	csv_writer
	PRIVATE -Wall -Wextra -Werror -Wpedantic -O3
)

target_link_libraries(
	csv_writer
	PUBLIC fmt::fmt
)

#executable

add_executable(
	csvls
	${SRCS}/main.cpp
)

target_compile_options(
	csvls
	PRIVATE -Wall -Wextra -Werror -Wpedantic -O3
)

target_link_libraries(
	csvls
	PRIVATE cli checker assembler csv_writer
)

install(TARGETS csvls)
