include $(top_srcdir)/Makefile.common

sbin_PROGRAMS = rpl-ctl

manpages = rpl-ctl.8
rpl-ctl_DESC = "configure interfaces RPL behavior"

if MANPAGES
dist_man_MANS = $(manpages)
endif
EXTRA_DIST = $(manpages)

rpl-ctl_SOURCES = rpl-ctl.c rpl-ctl-common.c rpl-ctl-dag.c
noinst_HEADERS = rpl-ctl.h

rpl-ctl_CFLAGS = $(AM_CFLAGS) $(NL_CFLAGS) -D_GNU_SOURCE
rpl-ctl_LDADD = $(LDADD) $(NL_LIBS)

rpl-ctl.8: $(rpl-ctl_SOURCES) $(top_srcdir)/configure.ac
	-$(HELP2MAN) -o $@ -s 8 -N -n $(rpl-ctl_DESC) $(builddir)/rpl-ctl

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)`dirname $(leasefile)`
	$(mkinstalldirs) $(DESTDIR)`dirname $(pidfile)`

