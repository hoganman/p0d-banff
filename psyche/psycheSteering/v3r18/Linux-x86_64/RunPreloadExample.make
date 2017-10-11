#-- start of make_header -----------------

#====================================
#  Application RunPreloadExample
#
#   Generated Mon Oct  9 06:43:45 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunPreloadExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunPreloadExample_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunPreloadExample

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunPreloadExample = /tmp/CMT_$(psycheSteering_tag)_RunPreloadExample.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunPreloadExample = $(psycheSteering_tag)_RunPreloadExample.make
cmt_local_tagfile_RunPreloadExample = $(bin)$(psycheSteering_tag)_RunPreloadExample.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunPreloadExample = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunPreloadExample = $(psycheSteering_tag).make
cmt_local_tagfile_RunPreloadExample = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_RunPreloadExample)

ifdef cmt_RunPreloadExample_has_target_tag

ifdef READONLY
cmt_final_setup_RunPreloadExample = /tmp/CMT_psycheSteering_RunPreloadExamplesetup.make
cmt_local_RunPreloadExample_makefile = /tmp/CMT_RunPreloadExample$(cmt_lock_pid).make
else
cmt_final_setup_RunPreloadExample = $(bin)psycheSteering_RunPreloadExamplesetup.make
cmt_local_RunPreloadExample_makefile = $(bin)RunPreloadExample.make
endif

else

ifdef READONLY
cmt_final_setup_RunPreloadExample = /tmp/CMT_psycheSteeringsetup.make
cmt_local_RunPreloadExample_makefile = /tmp/CMT_RunPreloadExample$(cmt_lock_pid).make
else
cmt_final_setup_RunPreloadExample = $(bin)psycheSteeringsetup.make
cmt_local_RunPreloadExample_makefile = $(bin)RunPreloadExample.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

RunPreloadExample ::


ifdef READONLY
RunPreloadExample ::
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
	$(echo) 'RunPreloadExample'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunPreloadExample/
RunPreloadExample::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunPreloadExample :: dirs  $(bin)RunPreloadExample${application_suffix}
	$(echo) "RunPreloadExample ok"

#-- end of application_header
#-- start of application

$(bin)RunPreloadExample${application_suffix} :: $(bin)RunPreloadExample.o $(use_stamps) $(RunPreloadExamplestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunPreloadExample.o $(cmt_installarea_linkopts) $(RunPreloadExample_use_linkopts) $(RunPreloadExamplelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunPreloadExampleinstallname = RunPreloadExample${application_suffix}

RunPreloadExample :: RunPreloadExampleinstall

install :: RunPreloadExampleinstall

RunPreloadExampleinstall :: $(install_dir)/$(RunPreloadExampleinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunPreloadExampleinstallname) :: $(bin)$(RunPreloadExampleinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunPreloadExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunPreloadExampleclean :: RunPreloadExampleuninstall

uninstall :: RunPreloadExampleuninstall

RunPreloadExampleuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunPreloadExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunPreloadExample.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunPreloadExampleclean)

#$(bin)RunPreloadExample_dependencies.make :: dirs

ifndef QUICK
$(bin)RunPreloadExample_dependencies.make : ../app/RunPreloadExample.cxx $(use_requirements) $(cmt_final_setup_RunPreloadExample)
	$(echo) "(RunPreloadExample.make) Rebuilding $@"; \
	  $(build_dependencies) RunPreloadExample -all_sources -out=$@ ../app/RunPreloadExample.cxx
endif

#$(RunPreloadExample_dependencies)

-include $(bin)RunPreloadExample_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunPreloadExample_dependencies.make : $(RunPreloadExample_cxx_dependencies)

$(bin)$(binobj)RunPreloadExample.o : $(RunPreloadExample_cxx_dependencies)
	$(cpp_echo) ../app/RunPreloadExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunPreloadExample_pp_cppflags) $(app_RunPreloadExample_pp_cppflags) $(RunPreloadExample_pp_cppflags) $(use_cppflags) $(RunPreloadExample_cppflags) $(app_RunPreloadExample_cppflags) $(RunPreloadExample_cppflags) $(RunPreloadExample_cxx_cppflags) -I../app ../app/RunPreloadExample.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunPreloadExampleclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunPreloadExample.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunPreloadExample.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunPreloadExample)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunPreloadExample.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunPreloadExample.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunPreloadExample.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunPreloadExampleclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunPreloadExample${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunPreloadExample${application_suffix}

#	@echo "------> (RunPreloadExample.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunPreloadExample.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunPreloadExample_deps RunPreloadExample_dependencies.make
#-- end of cleanup_objects ------
