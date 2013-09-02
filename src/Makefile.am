include $(top_srcdir)/Makefile.common

sbin_PROGRAMS = rpl-ctl

manpages = rpl-ctl.8
rpl_ctl_DESC = "configure interfaces RPL behavior"

if MANPAGES
dist_man_MANS = $(manpages)
endif
EXTRA_DIST = $(manpages)

rpl_ctl_SOURCES = rpl-ctl.c rpl-ctl-common.c rpl-ctl-dag-conf.c rpl-ctl-dag-info.c rpl-ctl-dag-mng.c
noinst_HEADERS = rpl-ctl.h

rpl_ctl_CFLAGS = $(AM_CFLAGS) $(NL_CFLAGS) -D_GNU_SOURCE
rpl_ctl_LDADD = $(LDADD) $(NL_LIBS)

rpl-ctl.8: $(rpl_ctl_SOURCES) $(top_srcdir)/configure.ac
	-$(HELP2MAN) -o $@ -s 8 -N -n $(rpl_ctl_DESC) $(builddir)/rpl-ctl

install-data-hook:
	$(mkinstalldirs) $(DESTDIR)`dirname $(leasefile)`
	$(mkinstalldirs) $(DESTDIR)`dirname $(pidfile)`

