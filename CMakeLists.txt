cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)

project(ac)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# VARIABLES
set(SRC_DIR ${CMAKE_SOURCE_DIR}/AC/src)
set(CMAKE_C_FLAGS_RELEASE "-O3")

file(GLOB SOURCES ${SRC_DIR}/*.c)

include_directories(ac ${CMAKE_SOURCE_DIR})

add_executable(ac ${CMAKE_SOURCE_DIR}/AC/main.c ${SOURCES})
