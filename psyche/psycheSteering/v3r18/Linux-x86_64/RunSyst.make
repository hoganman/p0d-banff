#-- start of make_header -----------------

#====================================
#  Application RunSyst
#
#   Generated Mon Oct  9 06:43:43 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunSyst_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunSyst_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunSyst

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSyst = /tmp/CMT_$(psycheSteering_tag)_RunSyst.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSyst = $(psycheSteering_tag)_RunSyst.make
cmt_local_tagfile_RunSyst = $(bin)$(psycheSteering_tag)_RunSyst.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSyst = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSyst = $(psycheSteering_tag).make
cmt_local_tagfile_RunSyst = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_RunSyst)

ifdef cmt_RunSyst_has_target_tag

ifdef READONLY
cmt_final_setup_RunSyst = /tmp/CMT_psycheSteering_RunSystsetup.make
cmt_local_RunSyst_makefile = /tmp/CMT_RunSyst$(cmt_lock_pid).make
else
cmt_final_setup_RunSyst = $(bin)psycheSteering_RunSystsetup.make
cmt_local_RunSyst_makefile = $(bin)RunSyst.make
endif

else

ifdef READONLY
cmt_final_setup_RunSyst = /tmp/CMT_psycheSteeringsetup.make
cmt_local_RunSyst_makefile = /tmp/CMT_RunSyst$(cmt_lock_pid).make
else
cmt_final_setup_RunSyst = $(bin)psycheSteeringsetup.make
cmt_local_RunSyst_makefile = $(bin)RunSyst.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

RunSyst ::


ifdef READONLY
RunSyst ::
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
	$(echo) 'RunSyst'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunSyst/
RunSyst::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunSyst :: dirs  $(bin)RunSyst${application_suffix}
	$(echo) "RunSyst ok"

#-- end of application_header
#-- start of application

$(bin)RunSyst${application_suffix} :: $(bin)RunSyst.o $(use_stamps) $(RunSyststamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunSyst.o $(cmt_installarea_linkopts) $(RunSyst_use_linkopts) $(RunSystlinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunSystinstallname = RunSyst${application_suffix}

RunSyst :: RunSystinstall

install :: RunSystinstall

RunSystinstall :: $(install_dir)/$(RunSystinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunSystinstallname) :: $(bin)$(RunSystinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSystinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunSystclean :: RunSystuninstall

uninstall :: RunSystuninstall

RunSystuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSystinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunSyst.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunSystclean)

#$(bin)RunSyst_dependencies.make :: dirs

ifndef QUICK
$(bin)RunSyst_dependencies.make : ../app/RunSyst.cxx $(use_requirements) $(cmt_final_setup_RunSyst)
	$(echo) "(RunSyst.make) Rebuilding $@"; \
	  $(build_dependencies) RunSyst -all_sources -out=$@ ../app/RunSyst.cxx
endif

#$(RunSyst_dependencies)

-include $(bin)RunSyst_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunSyst_dependencies.make : $(RunSyst_cxx_dependencies)

$(bin)$(binobj)RunSyst.o : $(RunSyst_cxx_dependencies)
	$(cpp_echo) ../app/RunSyst.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunSyst_pp_cppflags) $(app_RunSyst_pp_cppflags) $(RunSyst_pp_cppflags) $(use_cppflags) $(RunSyst_cppflags) $(app_RunSyst_cppflags) $(RunSyst_cppflags) $(RunSyst_cxx_cppflags) -I../app ../app/RunSyst.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunSystclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunSyst.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunSyst.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunSyst)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunSystclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunSyst${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunSyst${application_suffix}

#	@echo "------> (RunSyst.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunSyst.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunSyst_deps RunSyst_dependencies.make
#-- end of cleanup_objects ------
