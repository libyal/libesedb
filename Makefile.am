ACLOCAL_AMFLAGS = -I m4

SUBDIRS = \
	include \
	common \
	libcstring \
	liberror \
	libnotify \
	libuna \
	libbfio \
	libfdata \
	libfdatetime \
	libfguid \
	libfvalue \
	libesedb \
	libfwnt \
	libfmapi \
	libsystem \
	esedbtools \
	po \
	manuals \
	macosx \
	msvscpp

REDHAT_FILES = \
	libesedb.spec \
	libesedb.spec.in

EXTRA_DIST = \
	config.rpath  \
	libesedb.pc.in \
        $(REDHAT_FILES) \
	README_mingw

MAINTAINERCLEANFILES = \
	Makefile.in

pkgconfigdir = $(libdir)/pkgconfig

pkgconfig_DATA = \
	libesedb.pc

libtool:
	@LIBTOOL_DEPS@
	cd $(srcdir) && $(SHELL) ./config.status --recheck

lib: library

library:
	(cd $(srcdir)/common && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libcstring && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdata && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfguid && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/libesedb && $(MAKE) $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))

distclean: clean
	/bin/rm -f Makefile
	/bin/rm -f config.status
	/bin/rm -f config.cache
	/bin/rm -f config.log
	/bin/rm -f libesedb.pc
	/bin/rm -f libesedb.spec
	@for dir in ${subdirs}; do \
		(cd $$dir && $(MAKE) distclean) \
		|| case "$(MFLAGS)" in *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"

splint:
	(cd $(srcdir)/libcstring && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/liberror && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libnotify && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libuna && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libbfio && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdata && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfdatetime && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfguid && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfvalue && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libesedb && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfwnt && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libfmapi && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/libsystem && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/esedbtools && $(MAKE) splint $(AM_MAKEFLAGS))
	(cd $(srcdir)/po && $(MAKE) $(AM_MAKEFLAGS))

