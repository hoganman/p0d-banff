#-- start of make_header -----------------

#====================================
#  Application RunExpPreloadExample
#
#   Generated Mon Oct  9 06:43:46 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunExpPreloadExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunExpPreloadExample_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunExpPreloadExample

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunExpPreloadExample = /tmp/CMT_$(psycheSteering_tag)_RunExpPreloadExample.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunExpPreloadExample = $(psycheSteering_tag)_RunExpPreloadExample.make
cmt_local_tagfile_RunExpPreloadExample = $(bin)$(psycheSteering_tag)_RunExpPreloadExample.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunExpPreloadExample = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunExpPreloadExample = $(psycheSteering_tag).make
cmt_local_tagfile_RunExpPreloadExample = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_RunExpPreloadExample)

ifdef cmt_RunExpPreloadExample_has_target_tag

ifdef READONLY
cmt_final_setup_RunExpPreloadExample = /tmp/CMT_psycheSteering_RunExpPreloadExamplesetup.make
cmt_local_RunExpPreloadExample_makefile = /tmp/CMT_RunExpPreloadExample$(cmt_lock_pid).make
else
cmt_final_setup_RunExpPreloadExample = $(bin)psycheSteering_RunExpPreloadExamplesetup.make
cmt_local_RunExpPreloadExample_makefile = $(bin)RunExpPreloadExample.make
endif

else

ifdef READONLY
cmt_final_setup_RunExpPreloadExample = /tmp/CMT_psycheSteeringsetup.make
cmt_local_RunExpPreloadExample_makefile = /tmp/CMT_RunExpPreloadExample$(cmt_lock_pid).make
else
cmt_final_setup_RunExpPreloadExample = $(bin)psycheSteeringsetup.make
cmt_local_RunExpPreloadExample_makefile = $(bin)RunExpPreloadExample.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

RunExpPreloadExample ::


ifdef READONLY
RunExpPreloadExample ::
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
	$(echo) 'RunExpPreloadExample'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunExpPreloadExample/
RunExpPreloadExample::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunExpPreloadExample :: dirs  $(bin)RunExpPreloadExample${application_suffix}
	$(echo) "RunExpPreloadExample ok"

#-- end of application_header
#-- start of application

$(bin)RunExpPreloadExample${application_suffix} :: $(bin)RunExpPreloadExample.o $(use_stamps) $(RunExpPreloadExamplestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunExpPreloadExample.o $(cmt_installarea_linkopts) $(RunExpPreloadExample_use_linkopts) $(RunExpPreloadExamplelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunExpPreloadExampleinstallname = RunExpPreloadExample${application_suffix}

RunExpPreloadExample :: RunExpPreloadExampleinstall

install :: RunExpPreloadExampleinstall

RunExpPreloadExampleinstall :: $(install_dir)/$(RunExpPreloadExampleinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunExpPreloadExampleinstallname) :: $(bin)$(RunExpPreloadExampleinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunExpPreloadExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunExpPreloadExampleclean :: RunExpPreloadExampleuninstall

uninstall :: RunExpPreloadExampleuninstall

RunExpPreloadExampleuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunExpPreloadExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunExpPreloadExample.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunExpPreloadExampleclean)

#$(bin)RunExpPreloadExample_dependencies.make :: dirs

ifndef QUICK
$(bin)RunExpPreloadExample_dependencies.make : ../app/RunExpPreloadExample.cxx $(use_requirements) $(cmt_final_setup_RunExpPreloadExample)
	$(echo) "(RunExpPreloadExample.make) Rebuilding $@"; \
	  $(build_dependencies) RunExpPreloadExample -all_sources -out=$@ ../app/RunExpPreloadExample.cxx
endif

#$(RunExpPreloadExample_dependencies)

-include $(bin)RunExpPreloadExample_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunExpPreloadExample_dependencies.make : $(RunExpPreloadExample_cxx_dependencies)

$(bin)$(binobj)RunExpPreloadExample.o : $(RunExpPreloadExample_cxx_dependencies)
	$(cpp_echo) ../app/RunExpPreloadExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunExpPreloadExample_pp_cppflags) $(app_RunExpPreloadExample_pp_cppflags) $(RunExpPreloadExample_pp_cppflags) $(use_cppflags) $(RunExpPreloadExample_cppflags) $(app_RunExpPreloadExample_cppflags) $(RunExpPreloadExample_cppflags) $(RunExpPreloadExample_cxx_cppflags) -I../app ../app/RunExpPreloadExample.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunExpPreloadExampleclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunExpPreloadExample.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunExpPreloadExample.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunExpPreloadExample)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExpPreloadExample.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExpPreloadExample.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExpPreloadExample.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunExpPreloadExampleclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunExpPreloadExample${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunExpPreloadExample${application_suffix}

#	@echo "------> (RunExpPreloadExample.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunExpPreloadExample.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunExpPreloadExample_deps RunExpPreloadExample_dependencies.make
#-- end of cleanup_objects ------
