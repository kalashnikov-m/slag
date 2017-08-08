cmake_minimum_required(VERSION 3.5)

project(crytests)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (SRC
    main.cpp
    HugeCoreTest.cpp
    HugeTest.cpp	
    ../include/internal/HugeCore.cpp
    ../include/basic_int.hpp
    ../include/rsa/rsa.hpp
)

include_directories(../include)
include_directories(../include/internal)
include_directories(./googletest/include)

add_executable(crytests ${SRC})

if(UNIX)
    target_link_libraries(crytests gtest -lpthread)
elseif(WIN32)
    target_link_libraries(crytests gtest)
endif()
