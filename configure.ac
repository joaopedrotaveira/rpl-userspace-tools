#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([Userspace tools for RPL],
	[0.1],[joao.p.taveira@gmail.com],
	[rpl-tools], [http://github.com/joaopedrotaveira/rpl-userspace-tools])
m4_ifndef([AC_PACKAGE_URL],
	  [AC_SUBST([PACKAGE_URL], [http://github.com/joaopedrotaveira/rpl-userspace-tools])
	  AC_DEFINE_UNQUOTED([PACKAGE_URL], ["$PACKAGE_URL"],
			     [Define to the home page for this package.])])
AC_CONFIG_AUX_DIR([misc])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AM_MAINTAINER_MODE([enable])
AC_CONFIG_HEADER([config.h])

# Test for new silent rules and enable only if they are available
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_ARG_WITH(iproutedir, [AC_HELP_STRING([--with-iproutedir],
	    [directory where iproute2 plugins will be installed])],
	    [iproutedir=$withval],
	    [iproutedir='${libdir}/ip'])
AC_SUBST(iproutedir)

AC_ARG_ENABLE([manpages],[AS_HELP_STRING([--disable-manpages],
	      [disable manpages generation and installation])],
	[case "${enableval}" in
		yes | true) manpages=yes ;;
		no | fale) manpages=no ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --enable-manpages]) ;;
	esac], [manpages=yes])
AM_CONDITIONAL([MANPAGES], [test x$manpages = xyes])

AC_ARG_ENABLE([kernel-compat],[AS_HELP_STRING([--enable-kernel-compat],
	      [enable compatibility with older kernels])],
	[case "${enableval}" in
		yes | true) kernelcompat=yes ;;
		no | false) kernelcompat=no ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --enable-kernel-compat]) ;;
	esac], [kernelcompat=no])
if test "x$kernelcompat" == "xyes"
then
	AC_DEFINE([ENABLE_KERNEL_COMPAT], [1],
		  [Define to 1 if you want to enable compatibility with older kernels])
fi

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LIBTOOL
AM_PROG_LEX
AC_PROG_YACC
AC_SUBST(LIBTOOL_DEPS)
# Currently the scripts works with python2.x only
# Check for this at first
AC_PATH_PROG(PYTHON, python2 python2.7 python2.6 python2.5 python)
AM_PATH_PYTHON(,, [:])
py_CHECK_MAJOR_VERSION([PYTHON_VERSION], [2])
AM_MISSING_PROG(HELP2MAN, help2man, $missing_dir)

# Checks for libraries.
PKG_CHECK_MODULES([NL], [libnl-3.0 libnl-genl-3.0])

# Checks for header files.
AC_HEADER_STDC
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h libintl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h sys/ioctl.h sys/socket.h sys/time.h syslog.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([malloc realloc memset socket getopt_long select strdup strtol])

# Handle this last so -Werror doesn't get in the way of compile tests
AC_ARG_ENABLE(werror, [AC_HELP_STRING([--disable-werror],
	      [do not compile with -Werror])])
if test "x$enable_werror" != "xno" ; then
	CFLAGS="-Werror $CFLAGS"
fi

AC_CONFIG_FILES([
	Makefile
	lib/Makefile
	src/Makefile
])
AC_OUTPUT
