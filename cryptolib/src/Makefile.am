bin_PROGRAMS = hugeTests

AM_CXXFLAGS = -I../include -std=c++14

hugeTests_SOURCES = main.cpp
hugeTests_LDADD = libtests.la \
	libhuge.la \
	libgtest.a \
	libgtest_main.a \
	-lpthread

lib_LTLIBRARIES = libhuge.la libtests.la

libhuge_la_SOURCES = HugeCore.cpp Huge.hpp
libhuge_la_LDFLAGS = -shared


libtests_la_SOURCES = HugeCoreTest.cpp HugeTest.cpp
libtests_la_LDFLAGS = -shared
