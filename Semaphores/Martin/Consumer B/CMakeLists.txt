#set(CMAKE_VERBOSE_MAKEFILE 1)

cmake_minimum_required(VERSION 3.5)
project(Consumer_B)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lrt")

set(SOURCE_FILES main.cpp)
add_executable(Consumer_B ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(Consumer_B rt)