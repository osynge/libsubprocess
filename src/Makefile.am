
h_sources = \
	ptrptr.h \
	subprocess.h
	
	
c_sources = \
	config_local.h \
	ptrptr.c \
	subprocess.c


AM_CFLAGS =\
	 -Wall -Wimplicit -Wunused -Wmissing-prototypes\
	 -O1\
	 -g



lib_LTLIBRARIES = libsubprocess.la
libsubprocess_la_SOURCES= $(h_sources) $(c_sources) 
libsubprocess_la_LDFLAGS = -no-undefined -version-info $(LIBRARY_VERSION)
libsubprocess_la_CFLAGS=-fPIC $(AM_CFLAGS)
library_includedir=$(includedir)/yokel
library_include_HEADERS = $(h_sources)

libsubprocess_test_CFLAGS = $(AM_CFLAGS)
noinst_PROGRAMS = libsubprocess_test



libsubprocess_test_LDFLAGS = -lsubprocess

libsubprocess_test_SOURCES = \
	$(h_sources) \
	$(c_sources) \
	subprocess_test.c

