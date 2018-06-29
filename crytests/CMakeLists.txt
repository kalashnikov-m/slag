cmake_minimum_required(VERSION 3.5)
project(crytests)

add_subdirectory(googletest)

set(EXECUTABLE_OUTPUT_PATH "bin")

set (HEADERS
	../include/rsa/rsa.hpp
	../include/rsa/rsassa_pss.hpp
	../include/rsa/rsassa_pkcs1.hpp
	../include/rsa/rsaes_oaep.hpp
	../include/rsa/rsaes_pkcs1.hpp
    	../include/rsa/emsa_pkcs1.hpp
	../include/rsa/emsa_pss.hpp
	../include/rsa/eme_pkcs1.hpp
	../include/rsa/eme_oaep.hpp	
    	../include/algorithm.hpp
	../include/basic_integer.hpp
	../include/rsa/mgf.hpp
	../include/oid.hpp	
	../include/digest/sha1.hpp
	../include/digest/sha224.hpp
	../include/digest/sha256.hpp
	../include/digest/sha384.hpp
	../include/digest/sha512.hpp
	../utility/encoders.hpp
	../utility/os2ip.hpp
	../include/internal/cry_engine.hpp
)

set (SOURCES
    main.cpp
    Test_CryCore.cpp
    Test_Bigint.cpp
    Test_Rsa.cpp
)

include_directories(../utility)
include_directories(../include)
include_directories(../include/internal)
include_directories(../include/experimental)
include_directories(./googletest/include)

add_executable(crytests ${HEADERS} ${SOURCES})

if(UNIX)
    target_link_libraries(crytests gtest -lpthread)
elseif(WIN32)
    target_link_libraries(crytests gtest)
endif()
