cmake_minimum_required(VERSION 3.26)
project(assignment2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(assignment2
        interpreter.cc
        interpreter.h
        main.cc
        parser.cc
        parser.h)
