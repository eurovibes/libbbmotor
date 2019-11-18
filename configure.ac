dnl Every other copy of the package version number gets its value from here
AC_INIT(libbbmotor, 0.1.2, b.spranger@linutronix.de)
AM_INIT_AUTOMAKE

dnl create a config.h file (Automake will add -DHAVE_CONFIG_H)
AM_CONFIG_HEADER(config.h)

AC_SUBST(VERSION)

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST
AC_CANONICAL_BUILD
AC_USE_SYSTEM_EXTENSIONS

dnl Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADER(argp.h,,AC_MSG_ERROR(Cannot continue: argp.h not found))

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

dnl pkg-config section
PKG_CHECK_MODULES(GPIO, libgpiod,,
  AC_MSG_ERROR([libgpiod not found.])
)

dnl i18n makros.
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.17])

AC_OUTPUT([Makefile
	libbbmotor.doxy
	libbbmotor-uninstalled.pc
	libbbmotor.pc
	po/Makefile.in])
