#-- start of make_header -----------------

#====================================
#  Document oaAnalysisReader
#
#   Generated Mon Oct  9 06:36:55 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_oaAnalysisReader_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_oaAnalysisReader_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_oaAnalysisReader

oaAnalysisReader_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_oaAnalysisReader = /tmp/CMT_$(oaAnalysisReader_tag)_oaAnalysisReader.make$(cmt_lock_pid)
else
#cmt_local_tagfile_oaAnalysisReader = $(oaAnalysisReader_tag)_oaAnalysisReader.make
cmt_local_tagfile_oaAnalysisReader = $(bin)$(oaAnalysisReader_tag)_oaAnalysisReader.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

oaAnalysisReader_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_oaAnalysisReader = /tmp/CMT_$(oaAnalysisReader_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_oaAnalysisReader = $(oaAnalysisReader_tag).make
cmt_local_tagfile_oaAnalysisReader = $(bin)$(oaAnalysisReader_tag).make
endif

endif

-include $(cmt_local_tagfile_oaAnalysisReader)

ifdef cmt_oaAnalysisReader_has_target_tag

ifdef READONLY
cmt_final_setup_oaAnalysisReader = /tmp/CMT_oaAnalysisReader_oaAnalysisReadersetup.make
cmt_local_oaAnalysisReader_makefile = /tmp/CMT_oaAnalysisReader$(cmt_lock_pid).make
else
cmt_final_setup_oaAnalysisReader = $(bin)oaAnalysisReader_oaAnalysisReadersetup.make
cmt_local_oaAnalysisReader_makefile = $(bin)oaAnalysisReader.make
endif

else

ifdef READONLY
cmt_final_setup_oaAnalysisReader = /tmp/CMT_oaAnalysisReadersetup.make
cmt_local_oaAnalysisReader_makefile = /tmp/CMT_oaAnalysisReader$(cmt_lock_pid).make
else
cmt_final_setup_oaAnalysisReader = $(bin)oaAnalysisReadersetup.make
cmt_local_oaAnalysisReader_makefile = $(bin)oaAnalysisReader.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_oaAnalysisReadersetup.make
else
cmt_final_setup = $(bin)oaAnalysisReadersetup.make
endif

oaAnalysisReader ::


ifdef READONLY
oaAnalysisReader ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'oaAnalysisReader'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = oaAnalysisReader/
oaAnalysisReader::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

########################### Start External Header ######################### 
# -*- makefile -*-
# 
# A fragment used by CMT to build the an external library.  This executes a
# compilation script that is found in the src directory.  See the
# "external" fragment for the full documentation.
#

oaAnalysisReader_output = ../$(tag)

ifndef oaAnalysisReader_stamps
  ifdef $(package)_stamps
     oaAnalysisReader_stamps=$($(package)_stamps)
  else
     oaAnalysisReader_stamps=$(oaAnalysisReader_output)/oaAnalysisReader.stamp
  endif
endif

oaAnalysisReader :: $(oaAnalysisReader_stamps)
	@echo "oaAnalysisReader OK... " 

########################### End External Header ########################
compile_reader__dependencies = ../src/prod6B/compile_reader
# -*- makefile -*-
# 
# A makefile fragment used by CMT to build an EXTERNAL library.

# This fragment is intended to run a user script which in turn will
# run a normal "configure;make;make install" on some external library
# (e.g. CLHEP or GEANT4).  

# The fragment will execute a shell script found in the ../src/
# directory.  The script should compile the library when it is run "sh
# <scriptname> compile <package> <tag>" where <package> is the current
# package root directory, and <tag> is the current architecture tag.
# When the script is run as "sh <scriptname> clean <package> <tag>",
# the imported source should be cleaned.  This is usually done with
# "make distclean"

# An example of use in the requirements file is 
#    "document external CLHEP compile_script"

$(oaAnalysisReader_stamps) :: $(src)prod6B/compile_reader
	@echo "Compiling oaAnalysisReader" 
	bash $(src)prod6B/compile_reader compile $(PACKAGE_ROOT) $(tag)
	touch $(oaAnalysisReader_stamps)

oaAnalysisReaderclean :: 
	bash $(src)prod6B/compile_reader clean $(PACKAGE_ROOT) $(tag)
	rm $(oaAnalysisReader_stamps) || true

clean :: oaAnalysisReaderclean
#-- start of cleanup_header --------------

clean :: oaAnalysisReaderclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(oaAnalysisReader.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(oaAnalysisReader.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_oaAnalysisReader)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(oaAnalysisReader.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(oaAnalysisReader.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(oaAnalysisReader.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

oaAnalysisReaderclean ::
#-- end of cleanup_header ---------------
