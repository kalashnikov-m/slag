cmake_minimum_required(VERSION 3.5)

project(gtest)

include_directories(./include/)
include_directories(./include/gtest/)
include_directories(./include/gtest/internal/)
include_directories(./)

set (SRC
    ./src/gtest-all.cc
    ./src/gtest-filepath.cc
    ./src/gtest-port.cc
    ./src/gtest-test-part.cc
    ./src/gtest.cc
    ./src/gtest-death-test.cc
    ./src/gtest-internal-inl.h
    ./src/gtest-printers.cc
    ./src/gtest-typed-test.cc
    ./src/gtest_main.cc
)

add_library(gtest ${SRC})

