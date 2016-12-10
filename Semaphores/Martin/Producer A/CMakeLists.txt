#set(CMAKE_VERBOSE_MAKEFILE 1)

cmake_minimum_required(VERSION 3.5)
project(Producer_A)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lrt")

set(SOURCE_FILES main.cpp)
add_executable(Producer_A ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Producer_A rt)