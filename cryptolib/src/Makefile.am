bin_PROGRAMS = huge_main
lib_LTLIBRARIES = libhuge.la

AM_CPPFLAGS = -I../include 

huge_main_SOURCES = main.cpp
huge_main_LDADD = libhuge.la \
	libgtest.a \
	libgtest_main.a \
	-lpthread

#huge_main_LDFLAGS = -lpthread
huge_main_CXXFLAGS = -std=c++11

libhuge_la_SOURCES = huge.cpp
libhuge_la_LDFLAGS = -shared
libhuge_la_CXXFLAGS = -std=c++11
