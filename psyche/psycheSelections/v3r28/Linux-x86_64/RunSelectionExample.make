#-- start of make_header -----------------

#====================================
#  Application RunSelectionExample
#
#   Generated Mon Oct  9 06:40:08 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunSelectionExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunSelectionExample_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunSelectionExample

psycheSelections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSelectionExample = /tmp/CMT_$(psycheSelections_tag)_RunSelectionExample.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSelectionExample = $(psycheSelections_tag)_RunSelectionExample.make
cmt_local_tagfile_RunSelectionExample = $(bin)$(psycheSelections_tag)_RunSelectionExample.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSelections_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSelectionExample = /tmp/CMT_$(psycheSelections_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSelectionExample = $(psycheSelections_tag).make
cmt_local_tagfile_RunSelectionExample = $(bin)$(psycheSelections_tag).make
endif

endif

-include $(cmt_local_tagfile_RunSelectionExample)

ifdef cmt_RunSelectionExample_has_target_tag

ifdef READONLY
cmt_final_setup_RunSelectionExample = /tmp/CMT_psycheSelections_RunSelectionExamplesetup.make
cmt_local_RunSelectionExample_makefile = /tmp/CMT_RunSelectionExample$(cmt_lock_pid).make
else
cmt_final_setup_RunSelectionExample = $(bin)psycheSelections_RunSelectionExamplesetup.make
cmt_local_RunSelectionExample_makefile = $(bin)RunSelectionExample.make
endif

else

ifdef READONLY
cmt_final_setup_RunSelectionExample = /tmp/CMT_psycheSelectionssetup.make
cmt_local_RunSelectionExample_makefile = /tmp/CMT_RunSelectionExample$(cmt_lock_pid).make
else
cmt_final_setup_RunSelectionExample = $(bin)psycheSelectionssetup.make
cmt_local_RunSelectionExample_makefile = $(bin)RunSelectionExample.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSelectionssetup.make
else
cmt_final_setup = $(bin)psycheSelectionssetup.make
endif

RunSelectionExample ::


ifdef READONLY
RunSelectionExample ::
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
	$(echo) 'RunSelectionExample'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunSelectionExample/
RunSelectionExample::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunSelectionExample :: dirs  $(bin)RunSelectionExample${application_suffix}
	$(echo) "RunSelectionExample ok"

#-- end of application_header
#-- start of application

$(bin)RunSelectionExample${application_suffix} :: $(bin)RunSelectionExample.o $(use_stamps) $(RunSelectionExamplestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunSelectionExample.o $(cmt_installarea_linkopts) $(RunSelectionExample_use_linkopts) $(RunSelectionExamplelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunSelectionExampleinstallname = RunSelectionExample${application_suffix}

RunSelectionExample :: RunSelectionExampleinstall

install :: RunSelectionExampleinstall

RunSelectionExampleinstall :: $(install_dir)/$(RunSelectionExampleinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunSelectionExampleinstallname) :: $(bin)$(RunSelectionExampleinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSelectionExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunSelectionExampleclean :: RunSelectionExampleuninstall

uninstall :: RunSelectionExampleuninstall

RunSelectionExampleuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSelectionExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunSelectionExample.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunSelectionExampleclean)

#$(bin)RunSelectionExample_dependencies.make :: dirs

ifndef QUICK
$(bin)RunSelectionExample_dependencies.make : ../app/RunSelectionExample.cxx $(use_requirements) $(cmt_final_setup_RunSelectionExample)
	$(echo) "(RunSelectionExample.make) Rebuilding $@"; \
	  $(build_dependencies) RunSelectionExample -all_sources -out=$@ ../app/RunSelectionExample.cxx
endif

#$(RunSelectionExample_dependencies)

-include $(bin)RunSelectionExample_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunSelectionExample_dependencies.make : $(RunSelectionExample_cxx_dependencies)

$(bin)$(binobj)RunSelectionExample.o : $(RunSelectionExample_cxx_dependencies)
	$(cpp_echo) ../app/RunSelectionExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunSelectionExample_pp_cppflags) $(app_RunSelectionExample_pp_cppflags) $(RunSelectionExample_pp_cppflags) $(use_cppflags) $(RunSelectionExample_cppflags) $(app_RunSelectionExample_cppflags) $(RunSelectionExample_cppflags) $(RunSelectionExample_cxx_cppflags) -I../app ../app/RunSelectionExample.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunSelectionExampleclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunSelectionExample.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunSelectionExample.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunSelectionExample)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSelectionExample.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSelectionExample.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSelectionExample.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunSelectionExampleclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunSelectionExample${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunSelectionExample${application_suffix}

#	@echo "------> (RunSelectionExample.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunSelectionExample.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunSelectionExample_deps RunSelectionExample_dependencies.make
#-- end of cleanup_objects ------
