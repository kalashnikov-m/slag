cmake_minimum_required(VERSION 3.5)
project(crytests)

add_subdirectory(googletest)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (SOURCES
    main.cpp
    Test_CryCore.cpp
    Test_Bigint.cpp
    Test_Rsa.cpp
)

include_directories(../include)
include_directories(../include/internal)
include_directories(./googletest/include)

add_executable(crytests ${HEADERS} ${SOURCES})

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    target_link_libraries(crytests gtest -lpthread)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    target_link_libraries(crytests gtest)
endif()
