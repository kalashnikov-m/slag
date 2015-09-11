bin_PROGRAMS = huge_main

AM_CPPFLAGS = -I../include 

huge_main_SOURCES = main.cpp
huge_main_LDADD = libhugecore.la \
	libgtest.a \
	libgtest_main.a \
	-lpthread

huge_main_CXXFLAGS = -std=c++11



lib_LTLIBRARIES = libhugecore.la

libhugecore_la_SOURCES = hugecore.cpp \
	hugecore_test.cpp \
	huge_test.cpp
	
libhugecore_la_LDFLAGS = -shared
libhugecore_la_CXXFLAGS = -std=c++11
