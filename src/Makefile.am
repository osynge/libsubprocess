
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

bin_PROGRAMS = edg-se-lib-launcher
edg_se_lib_launcher_CFLAGS = $(AM_CFLAGS)



edg_se_lib_launcher_SOURCES = \
	main.c\
	$(c_sources)



dnl edg_se_lib_launcher_LDFLAGS = -ldb 



lib_LTLIBRARIES = liblauncher.la
liblauncher_la_SOURCES= $(c_sources)


library_includedir=$(includedir)/se
library_include_HEADERS = $(h_sources)
