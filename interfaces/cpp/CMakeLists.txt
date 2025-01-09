cmake_minimum_required(VERSION 3.28)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

project(Sinfourmis)

add_library(Sinfourmis SHARED fourmis.cpp)

target_include_directories(Sinfourmis PRIVATE .)

string(APPEND CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -Werror -pedantic -Wpointer-arith -Wchar-subscripts -Wcomment -Wformat -Wwrite-strings -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function")

if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sinfourmis.h)
	message(SEND_ERROR "sinfourmis.h not found")
	message(FATAL_ERROR "Please copy the \"sinfourmis.h\" file to the current directory")
endif()
