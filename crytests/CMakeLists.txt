cmake_minimum_required(VERSION 3.5)

project(crytests)

add_subdirectory(googletest)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (HEADERS
	../include/basic_int.hpp
    ../include/rsa/rsa.hpp
)

set (SOURCES
    main.cpp
    HugeCoreTest.cpp
    HugeTest.cpp	
    ../include/internal/HugeCore.cpp
)

include_directories(../include)
include_directories(../include/internal)
include_directories(./googletest/include)

add_executable(crytests ${HEADERS} ${SOURCES})

if(UNIX)
    target_link_libraries(crytests gtest -lpthread)
elseif(WIN32)
    target_link_libraries(crytests gtest)
endif()
