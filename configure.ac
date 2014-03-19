dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta - will be overwritten
dnl If you don't want it to overwrite it,
dnl 	Please disable it in the Anjuta project configuration



VER_ARCHITECTURE=1
VER_MAJOR=0
VER_MINOR=0
VER_PATCH=0

AC_INIT(subprocess,0.0.7)
dnl AC_INIT(src/subprocess_test.c)

LT_INIT

AC_CONFIG_SRCDIR([src/subprocess_test.c])

dnl uncomment when tagging


# Only continue if this source file exists

dnl AC_CONFIG_AUX_DIR(config)

AM_CONFIG_HEADER(src/config.h)

dnl AM_INIT_AUTOMAKE(subprocess, $VER_ARCHITECTURE.$VER_MAJOR.$VER_MINOR,"os@yokel.org","subprocess","http://www.yokel.org/")
AM_INIT_AUTOMAKE
AC_SUBST(VER_ARCHITECTURE)
AC_SUBST(VER_MAJOR)
AC_SUBST(VER_MINOR)
AC_SUBST(VER_PATCH)

dnl LIBRARY_VERSION="$(VER_ARCHITECTURE):${VER_MAJOR}:${VER_MINOR}"
LIBRARY_VERSION=1:0:0

AC_SUBST(LIBRARY_VERSION)

AC_PROG_CC
AM_PROG_CC_C_O
AC_CHECK_FUNCS([wait])
AC_CHECK_FUNCS([fork])
AC_CHECK_FUNCS([waitpid])
AC_CHECK_FUNCS([malloc])
AC_CHECK_FUNCS([execv])
AC_CHECK_FUNCS([fork])
AC_CHECK_FUNCS([pipe])
#
# Wall
#
AC_CACHE_CHECK([for maximum warnings compiler flag],
  ac_cv_cwflag,
[case "${CC-cc}" in
  *gcc*) ac_cv_cwflag="-Wall -pedantic -pipe";;
  *)
  ;;
esac])
AC_SUBST([CWFLAG], [$ac_cv_cwflag])


AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

AC_CHECK_HEADERS([unistd.h sys/wait.h stdio.h stdlib.h string.h])
AM_PROG_LIBTOOL

dnl AM_INIT_AUTOMAKE(libsubprocess,$PACKAGE_VERSION)
#AM_CONFIG_HEADER(src/config.h)

AC_CONFIG_MACRO_DIR([m4])

dnl Checks for libraries.
dnl Checks for header files.
dnl Checks for typedefs, structures, and compiler characteristics.
dnl Checks for library functions.
dnl Checks for Additional stuffs.

dnl Languages which your application supports
dnl ALL_LINGUAS=""
dnl AM_GNU_GETTEXT


AC_SUBST(ac_aux_dir)
RPM_PREFIX="/usr"
AC_SUBST(RPM_PREFIX)

dnl =============
dnl Check RPM dir
dnl =============
RPMD=`pwd`
AC_MSG_CHECKING([for directory where to create rpms])
AC_ARG_WITH(rpm-dir,[  --with-rpm-dir=<dir> Default is \$RPMD/],
            rpm_dir="$withval", rpm_dir="$RPMD")
if test -d "$rpm_dir" ; then
    AC_MSG_RESULT([found $rpm_dir])
    RPM_DIR=$rpm_dir
else
    AC_MSG_RESULT([no such directory $rpm_dir])
    AC_MSG_RESULT([the directory $rpm_dir will be created for you if possible])
    RPM_DIR=$rpm_dir
fi

AC_SUBST(RPM_DIR)

dnl Set PACKAGE_LOCALE_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${ac_default_prefix}/${DATADIRNAME}/locale",[Defines the source code directory])
else
  AC_DEFINE_UNQUOTED(PACKAGE_LOCALE_DIR, "${prefix}/${DATADIRNAME}/locale",[Defines the source code directory])
fi


dnl Set PACKAGE SOURCE DIR in config.h.
packagesrcdir=`cd $srcdir && pwd`

dnl Set PACKAGE PREFIX
if test "x${prefix}" = "xNONE"; then
  packageprefix=${ac_default_prefix}
else
  packageprefix=${prefix}
fi

dnl Set PACKAGE DATA & DOC DIR
packagedatadir=share/${PACKAGE}
packagedocdir=doc/${PACKAGE}

dnl Set PACKAGE DIRS in config.h.
packagepixmapsdir=${packagedatadir}/pixmaps
packagehelpdir=${packagedatadir}/help
packagemenudir=${packagedatadir}

dnl Subst PACKAGE_DATA_DIR.
NO_PREFIX_PACKAGE_DATA_DIR="${packagedatadir}"
AC_SUBST(NO_PREFIX_PACKAGE_DATA_DIR)
PACKAGE_DATA_DIR="${packageprefix}/${packagedatadir}"
AC_SUBST(PACKAGE_DATA_DIR)

dnl Subst PACKAGE_DOC_DIR.
NO_PREFIX_PACKAGE_DOC_DIR="${packagedocdir}"
AC_SUBST(NO_PREFIX_PACKAGE_DOC_DIR)
PACKAGE_DOC_DIR="${packageprefix}/${packagedocdir}"
AC_SUBST(PACKAGE_DOC_DIR)

dnl Subst PACKAGE_PIXMAPS_DIR.
NO_PREFIX_PACKAGE_PIXMAPS_DIR="${packagepixmapsdir}"
AC_SUBST(NO_PREFIX_PACKAGE_PIXMAPS_DIR)
PACKAGE_PIXMAPS_DIR="${packageprefix}/${packagepixmapsdir}"
AC_SUBST(PACKAGE_PIXMAPS_DIR)

dnl Subst PACKAGE_HELP_DIR.
NO_PREFIX_PACKAGE_HELP_DIR="${packagehelpdir}"
AC_SUBST(NO_PREFIX_PACKAGE_HELP_DIR)
PACKAGE_HELP_DIR="${packageprefix}/${packagehelpdir}"
AC_SUBST(PACKAGE_HELP_DIR)

dnl Subst PACKAGE_MENU_DIR.
NO_PREFIX_PACKAGE_MENU_DIR="${packagemenudir}"
AC_SUBST(NO_PREFIX_PACKAGE_MENU_DIR)
PACKAGE_MENU_DIR="${packageprefix}/${packagemenudir}"
AC_SUBST(PACKAGE_MENU_DIR)

AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${packageprefix}/${packagedatadir}",[Defines the source code directory])
AC_DEFINE_UNQUOTED(PACKAGE_DOC_DIR, "${packageprefix}/${packagedocdir}",[Defines the source code directory])
AC_DEFINE_UNQUOTED(PACKAGE_PIXMAPS_DIR, "${packageprefix}/${packagepixmapsdir}",[Defines the source code directory])
AC_DEFINE_UNQUOTED(PACKAGE_HELP_DIR, "${packageprefix}/${packagehelpdir}",[Defines the source code directory])
AC_DEFINE_UNQUOTED(PACKAGE_MENU_DIR, "${packageprefix}/${packagemenudir}",[Defines the source code directory])
AC_DEFINE_UNQUOTED(PACKAGE_SOURCE_DIR, "${packagesrcdir}",[Defines the source code directory])


DOCD=${packageprefix}//share/doc/${PACKAGE}-${PACKAGE_VERSION}
AC_MSG_CHECKING([for directory where to install docs])
AC_ARG_WITH(doc-dir,[  --with-doc-dir=<dir> Default is $DOCD/],
            doc_dir="$withval", doc_dir="$DOCD")
if test -d "$doc_dir" ; then
    AC_MSG_RESULT([found $doc_dir])
    DOC_DIR=$doc_dir
else
    AC_MSG_RESULT([no such directory $doc_dir])
    AC_MSG_RESULT([the directory $doc_dir will be created for you if possible])
    DOC_DIR=$doc_dir
fi

AC_SUBST(DOC_DIR)

AC_SUBST(PACKAGE_VERSION)



AC_OUTPUT([
Makefile
src/Makefile
libsubprocess.spec
])

