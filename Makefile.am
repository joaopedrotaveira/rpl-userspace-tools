include $(top_srcdir)/Makefile.common

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = lib src

include_HEADERS = include/ieee802154.h include/nl802154.h
noinst_HEADERS = include/libcommon.h include/logging.h

EXTRA_DIST = $(srcdir)/include/linux

libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

#resync:
#	if [ -z "$(KDIR)" ] ; then echo "please set KDIR variable" ; exit 1 ; fi
#	cp $(KDIR)/include/linux/nl802154.h include/
#	patch include/nl802154.h include/nl802154.h.patch
#	cp $(KDIR)/net/ieee802154/nl_policy.c lib/
#	patch lib/nl_policy.c lib/nl_policy.c.patch
#	[ ! -z "$(NOCOMMIT)" ] || git commit -s include/nl802154.h include/nl802154.h.patch lib/nl_policy.c -m "Resync with kernel"
