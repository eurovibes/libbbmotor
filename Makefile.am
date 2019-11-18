SUBDIRS = po

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libbbmotor.pc

bin_PROGRAMS = stepper testservo

stepper_SOURCES = stepper.c stepper.1
stepper_CFLAGS = -O3 -Wall -Wextra -Werror -g -DLOCALEDIR='"$(localedir)"'
stepper_LDADD = libbbmotor.la

testservo_SOURCES = testservo.c testservo.1
testservo_CFLAGS = -O3 -Wall -Wextra -Werror -g -DLOCALEDIR='"$(localedir)"'
testservo_LDADD = libbbmotor.la

lib_LTLIBRARIES = libbbmotor.la
libbbmotor_la_SOURCES = core.c gpio.c i2c.c motor.c servo.c com.h internal.h
libbbmotor_la_CFLAGS = -O2 -Wall -Wextra @GPIO_CFLAGS@ -fvisibility=hidden
libbbmotor_la_LIBADD = @GPIO_LIBS@
include_HEADERS = bbmotor.h

# 1) If the library source code has changed at all since the last update, then
#    increment revision ("c:r:a" becomes "c:r+1:a").
# 2) If any interfaces have been added, removed, or changed since the last
#    update, increment current, and set revision to 0.
# 3) If any interfaces have been added since the last public release,
#    then increment age.
# 4) If any interfaces have been removed or changed since the last public
#    release, then set age to 0.

libbbmotor_la_LDFLAGS = -version-info 1:0:0

EXTRA_DIST = ChangeLog-from-git README.md libbbmotor.doxy.in libbbmotor.pc.in \
	libbbmotor-uninstalled.pc.in

html-local:
	doxygen libbbmotor.doxy

mostlyclean-local:
	-rm -rf doxydoc/

man_MANS = stepper.1 testservo.1

DISTCLEANFILES = *~ po/*~

MAINTAINERCLEANFILES = ABOUT-NLS ChangeLog INSTALL Makefile.in aclocal.m4 \
	compile config.guess config.h.in config.rpath config.sub configure \
	depcomp install-sh ltmain.sh m4/* missing po/Makefile.in.in \
	po/Makevars.template po/Rules-quot po/*.header po/*.sed po/*.sin

ACLOCAL_AMFLAGS = -I m4

dist-hook:
	$(top_srcdir)/ChangeLog-from-git $(top_srcdir)
