cmake_minimum_required(VERSION 3.5)

project(crytests)

add_subdirectory(gtest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (SRC
    main.cpp
    ../include/internal/HugeCore.cpp
)

include_directories(../include)
include_directories(../include/internal)

include_directories(./gtest/include/gtest/)
include_directories(./gtest/include/gtest/internal)

add_executable(crytests ${SRC})

target_link_libraries(crytests gtest)