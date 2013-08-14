
h_sources = \
	edg_se_launcher.h \
	edg_se_ptrptr.h
	
	
c_sources = \
	config_local.h \
	edg_se_launcher.c \
	edg_se_ptrptr.c



INCLUDES =

AM_CFLAGS =\
	 -Wall -Wimplicit -Wunused -Wmissing-prototypes\
	 -O1\
	 -g


libsubprocess_test_CFLAGS = $(AM_CFLAGS)
noinst_PROGRAMS = libsubprocess_test





libsubprocess_test_SOURCES = \
	main.c\
	$(c_sources)



dnl libsubprocess_test_LDFLAGS = -ldb 



lib_LTLIBRARIES = libsubprocess.la
libsubprocess_la_SOURCES= $(c_sources)


library_includedir=$(includedir)/yokel
library_include_HEADERS = $(h_sources)
