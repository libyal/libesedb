include_HEADERS = \
	libesedb.h

pkginclude_HEADERS = \
	libesedb/codepage.h \
	libesedb/definitions.h \
	libesedb/error.h \
	libesedb/extern.h \
	libesedb/features.h \
	libesedb/types.h

EXTRA_DIST = \
	libesedb.h.in \
	libesedb/definitions.h.in \
	libesedb/features.h.in \
	libesedb/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libesedb.h
	/bin/rm -f libesedb/definitions.h
	/bin/rm -f libesedb/features.h
	/bin/rm -f libesedb/types.h
	/bin/rm -f Makefile

