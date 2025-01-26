cmake_minimum_required(VERSION 3.4.1)
project(OpAwsLc)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 14)

add_library(op-aws-lc            SHARED
            ../cpp/op-aws-lc.cpp
            cpp-adapter.cpp
)

# Specifies a path to native header files.
include_directories(
            ../cpp
)
