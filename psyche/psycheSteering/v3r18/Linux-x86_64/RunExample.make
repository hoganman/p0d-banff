#-- start of make_header -----------------

#====================================
#  Application RunExample
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

cmt_RunExample_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunExample_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunExample

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunExample = /tmp/CMT_$(psycheSteering_tag)_RunExample.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunExample = $(psycheSteering_tag)_RunExample.make
cmt_local_tagfile_RunExample = $(bin)$(psycheSteering_tag)_RunExample.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunExample = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunExample = $(psycheSteering_tag).make
cmt_local_tagfile_RunExample = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_RunExample)

ifdef cmt_RunExample_has_target_tag

ifdef READONLY
cmt_final_setup_RunExample = /tmp/CMT_psycheSteering_RunExamplesetup.make
cmt_local_RunExample_makefile = /tmp/CMT_RunExample$(cmt_lock_pid).make
else
cmt_final_setup_RunExample = $(bin)psycheSteering_RunExamplesetup.make
cmt_local_RunExample_makefile = $(bin)RunExample.make
endif

else

ifdef READONLY
cmt_final_setup_RunExample = /tmp/CMT_psycheSteeringsetup.make
cmt_local_RunExample_makefile = /tmp/CMT_RunExample$(cmt_lock_pid).make
else
cmt_final_setup_RunExample = $(bin)psycheSteeringsetup.make
cmt_local_RunExample_makefile = $(bin)RunExample.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

RunExample ::


ifdef READONLY
RunExample ::
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
	$(echo) 'RunExample'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunExample/
RunExample::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunExample :: dirs  $(bin)RunExample${application_suffix}
	$(echo) "RunExample ok"

#-- end of application_header
#-- start of application

$(bin)RunExample${application_suffix} :: $(bin)RunExample.o $(use_stamps) $(RunExamplestamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunExample.o $(cmt_installarea_linkopts) $(RunExample_use_linkopts) $(RunExamplelinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunExampleinstallname = RunExample${application_suffix}

RunExample :: RunExampleinstall

install :: RunExampleinstall

RunExampleinstall :: $(install_dir)/$(RunExampleinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunExampleinstallname) :: $(bin)$(RunExampleinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunExampleclean :: RunExampleuninstall

uninstall :: RunExampleuninstall

RunExampleuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunExampleinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunExample.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunExampleclean)

#$(bin)RunExample_dependencies.make :: dirs

ifndef QUICK
$(bin)RunExample_dependencies.make : ../app/RunExample.cxx $(use_requirements) $(cmt_final_setup_RunExample)
	$(echo) "(RunExample.make) Rebuilding $@"; \
	  $(build_dependencies) RunExample -all_sources -out=$@ ../app/RunExample.cxx
endif

#$(RunExample_dependencies)

-include $(bin)RunExample_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunExample_dependencies.make : $(RunExample_cxx_dependencies)

$(bin)$(binobj)RunExample.o : $(RunExample_cxx_dependencies)
	$(cpp_echo) ../app/RunExample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunExample_pp_cppflags) $(app_RunExample_pp_cppflags) $(RunExample_pp_cppflags) $(use_cppflags) $(RunExample_cppflags) $(app_RunExample_cppflags) $(RunExample_cppflags) $(RunExample_cxx_cppflags) -I../app ../app/RunExample.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunExampleclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunExample.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunExample.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunExample)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExample.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExample.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunExample.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunExampleclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunExample${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunExample${application_suffix}

#	@echo "------> (RunExample.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunExample.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunExample_deps RunExample_dependencies.make
#-- end of cleanup_objects ------
