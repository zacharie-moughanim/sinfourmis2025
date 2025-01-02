cmake_minimum_required(VERSION 3.28)

project(Sinfourmis)

add_executable(Sinfourmis 
	src/map/map.cpp
	src/map/node.cpp
	src/map/edge.cpp
	src/game/game.cpp
	src/game/ant.cpp
	src/game/queen.cpp
	src/interfaces/dummy.cpp
	src/main.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -Wall -Wextra -Werror -pedantic -Wpointer-arith -Wchar-subscripts -Wcomment -Wformat -Wwrite-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")

# json library
include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(Sinfourmis PRIVATE nlohmann_json::nlohmann_json)

# argparser library
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

target_link_libraries(Sinfourmis PRIVATE argparse)

# clang-format
find_program(CLANG_FORMAT_EXE NAMES "clang-format" DOC "Path to clang-format executable")
if(CLANG_FORMAT_EXE)
	add_custom_target(
		format
		COMMAND ${CLANG_FORMAT_EXE} -i -style=file 
			${PROJECT_SOURCE_DIR}/src/*.cpp 
			${PROJECT_SOURCE_DIR}/src/*/*.cpp 
			${PROJECT_SOURCE_DIR}/include/*.h
			${PROJECT_SOURCE_DIR}/include/*.hpp
			${PROJECT_SOURCE_DIR}/include/*/*.hpp
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
else()
	message(STATUS "clang-format not found.")
endif()