#-- start of make_header -----------------

#====================================
#  Document register
#
#   Generated Wed Oct  4 12:22:02 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_register_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_register_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_register

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_register = /tmp/CMT_$(BANFF_tag)_register.make$(cmt_lock_pid)
else
#cmt_local_tagfile_register = $(BANFF_tag)_register.make
cmt_local_tagfile_register = $(bin)$(BANFF_tag)_register.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_register = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_register = $(BANFF_tag).make
cmt_local_tagfile_register = $(bin)$(BANFF_tag).make
endif

endif

-include $(cmt_local_tagfile_register)

ifdef cmt_register_has_target_tag

ifdef READONLY
cmt_final_setup_register = /tmp/CMT_BANFF_registersetup.make
cmt_local_register_makefile = /tmp/CMT_register$(cmt_lock_pid).make
else
cmt_final_setup_register = $(bin)BANFF_registersetup.make
cmt_local_register_makefile = $(bin)register.make
endif

else

ifdef READONLY
cmt_final_setup_register = /tmp/CMT_BANFFsetup.make
cmt_local_register_makefile = /tmp/CMT_register$(cmt_lock_pid).make
else
cmt_final_setup_register = $(bin)BANFFsetup.make
cmt_local_register_makefile = $(bin)register.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
cmt_final_setup = $(bin)BANFFsetup.make
endif

register ::


ifdef READONLY
register ::
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
	$(echo) 'register'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = register/
register::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of document_header ------

register_output = $(doc)

register :: 
	@/bin/echo "------> register ok"

#-- end of document_header ------
#-- start of cleanup_header --------------

clean :: registerclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(register.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(register.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_register)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(register.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(register.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(register.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

registerclean ::
#-- end of cleanup_header ---------------
