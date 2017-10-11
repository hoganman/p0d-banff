#
# Author: Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk>
#

SHELL    = /bin/sh
NAME     = all
MAKEFILE = Makefile

# Include machine specific flags and locations (inc. files & libs)
#
include $(T2KREWEIGHT)/make/Make.include

PACKAGE           = ReWeight
DICTIONARY        = $(T2KREWEIGHT_DICT)
LIBNAME           = $(T2KREWEIGHT_LIB)
EXTRA_EXT_LIBS    = 

all     : rootcint lib lib-link
install : install-inc install-lib

# Include standard package makefile targets
#
include $(T2KREWEIGHT)/make/Make.std-package-targets

FORCE:

