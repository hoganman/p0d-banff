#-- start of make_header -----------------

#====================================
#  Library highlandValidation
#
#   Generated Mon Oct  9 06:43:48 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_highlandValidation_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_highlandValidation_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_highlandValidation

highlandValidation_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandValidation = /tmp/CMT_$(highlandValidation_tag)_highlandValidation.make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandValidation = $(highlandValidation_tag)_highlandValidation.make
cmt_local_tagfile_highlandValidation = $(bin)$(highlandValidation_tag)_highlandValidation.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

highlandValidation_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_highlandValidation = /tmp/CMT_$(highlandValidation_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_highlandValidation = $(highlandValidation_tag).make
cmt_local_tagfile_highlandValidation = $(bin)$(highlandValidation_tag).make
endif

endif

-include $(cmt_local_tagfile_highlandValidation)

ifdef cmt_highlandValidation_has_target_tag

ifdef READONLY
cmt_final_setup_highlandValidation = /tmp/CMT_highlandValidation_highlandValidationsetup.make
cmt_local_highlandValidation_makefile = /tmp/CMT_highlandValidation$(cmt_lock_pid).make
else
cmt_final_setup_highlandValidation = $(bin)highlandValidation_highlandValidationsetup.make
cmt_local_highlandValidation_makefile = $(bin)highlandValidation.make
endif

else

ifdef READONLY
cmt_final_setup_highlandValidation = /tmp/CMT_highlandValidationsetup.make
cmt_local_highlandValidation_makefile = /tmp/CMT_highlandValidation$(cmt_lock_pid).make
else
cmt_final_setup_highlandValidation = $(bin)highlandValidationsetup.make
cmt_local_highlandValidation_makefile = $(bin)highlandValidation.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_highlandValidationsetup.make
else
cmt_final_setup = $(bin)highlandValidationsetup.make
endif

highlandValidation ::


ifdef READONLY
highlandValidation ::
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
	$(echo) 'highlandValidation'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = highlandValidation/
highlandValidation::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

highlandValidationlibname   = $(bin)$(library_prefix)highlandValidation$(library_suffix)
highlandValidationlib       = $(highlandValidationlibname).a
highlandValidationstamp     = $(bin)highlandValidation.stamp
highlandValidationshstamp   = $(bin)highlandValidation.shstamp

highlandValidation :: dirs  highlandValidationLIB
	$(echo) "highlandValidation ok"

#-- end of libary_header ----------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),highlandValidationclean)

#$(bin)highlandValidation_dependencies.make :: dirs

ifndef QUICK
$(bin)highlandValidation_dependencies.make :  $(use_requirements) $(cmt_final_setup_highlandValidation)
	$(echo) "(highlandValidation.make) Rebuilding $@"; \
	  $(build_dependencies) highlandValidation -all_sources -out=$@ 
endif

#$(highlandValidation_dependencies)

-include $(bin)highlandValidation_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cleanup_header --------------

clean :: highlandValidationclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(highlandValidation.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(highlandValidation.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_highlandValidation)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandValidation.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandValidation.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(highlandValidation.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

highlandValidationclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)highlandValidation$(library_suffix).a $(binobj)$(library_prefix)highlandValidation$(library_suffix).s? $(binobj)highlandValidation.stamp $(binobj)highlandValidation.shstamp
#-- end of cleanup_library ---------------

