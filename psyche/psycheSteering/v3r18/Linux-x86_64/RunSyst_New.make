#-- start of make_header -----------------

#====================================
#  Application RunSyst_New
#
#   Generated Mon Oct  9 06:43:44 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunSyst_New_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunSyst_New_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunSyst_New

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSyst_New = /tmp/CMT_$(psycheSteering_tag)_RunSyst_New.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSyst_New = $(psycheSteering_tag)_RunSyst_New.make
cmt_local_tagfile_RunSyst_New = $(bin)$(psycheSteering_tag)_RunSyst_New.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheSteering_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunSyst_New = /tmp/CMT_$(psycheSteering_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunSyst_New = $(psycheSteering_tag).make
cmt_local_tagfile_RunSyst_New = $(bin)$(psycheSteering_tag).make
endif

endif

-include $(cmt_local_tagfile_RunSyst_New)

ifdef cmt_RunSyst_New_has_target_tag

ifdef READONLY
cmt_final_setup_RunSyst_New = /tmp/CMT_psycheSteering_RunSyst_Newsetup.make
cmt_local_RunSyst_New_makefile = /tmp/CMT_RunSyst_New$(cmt_lock_pid).make
else
cmt_final_setup_RunSyst_New = $(bin)psycheSteering_RunSyst_Newsetup.make
cmt_local_RunSyst_New_makefile = $(bin)RunSyst_New.make
endif

else

ifdef READONLY
cmt_final_setup_RunSyst_New = /tmp/CMT_psycheSteeringsetup.make
cmt_local_RunSyst_New_makefile = /tmp/CMT_RunSyst_New$(cmt_lock_pid).make
else
cmt_final_setup_RunSyst_New = $(bin)psycheSteeringsetup.make
cmt_local_RunSyst_New_makefile = $(bin)RunSyst_New.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheSteeringsetup.make
else
cmt_final_setup = $(bin)psycheSteeringsetup.make
endif

RunSyst_New ::


ifdef READONLY
RunSyst_New ::
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
	$(echo) 'RunSyst_New'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunSyst_New/
RunSyst_New::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunSyst_New :: dirs  $(bin)RunSyst_New${application_suffix}
	$(echo) "RunSyst_New ok"

#-- end of application_header
#-- start of application

$(bin)RunSyst_New${application_suffix} :: $(bin)RunSyst_New.o $(use_stamps) $(RunSyst_Newstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunSyst_New.o $(cmt_installarea_linkopts) $(RunSyst_New_use_linkopts) $(RunSyst_Newlinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunSyst_Newinstallname = RunSyst_New${application_suffix}

RunSyst_New :: RunSyst_Newinstall

install :: RunSyst_Newinstall

RunSyst_Newinstall :: $(install_dir)/$(RunSyst_Newinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunSyst_Newinstallname) :: $(bin)$(RunSyst_Newinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSyst_Newinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunSyst_Newclean :: RunSyst_Newuninstall

uninstall :: RunSyst_Newuninstall

RunSyst_Newuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunSyst_Newinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunSyst_New.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunSyst_Newclean)

#$(bin)RunSyst_New_dependencies.make :: dirs

ifndef QUICK
$(bin)RunSyst_New_dependencies.make : ../app/RunSyst_New.cxx $(use_requirements) $(cmt_final_setup_RunSyst_New)
	$(echo) "(RunSyst_New.make) Rebuilding $@"; \
	  $(build_dependencies) RunSyst_New -all_sources -out=$@ ../app/RunSyst_New.cxx
endif

#$(RunSyst_New_dependencies)

-include $(bin)RunSyst_New_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunSyst_New_dependencies.make : $(RunSyst_New_cxx_dependencies)

$(bin)$(binobj)RunSyst_New.o : $(RunSyst_New_cxx_dependencies)
	$(cpp_echo) ../app/RunSyst_New.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunSyst_New_pp_cppflags) $(app_RunSyst_New_pp_cppflags) $(RunSyst_New_pp_cppflags) $(use_cppflags) $(RunSyst_New_cppflags) $(app_RunSyst_New_cppflags) $(RunSyst_New_cppflags) $(RunSyst_New_cxx_cppflags) -I../app ../app/RunSyst_New.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunSyst_Newclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunSyst_New.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunSyst_New.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunSyst_New)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst_New.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst_New.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunSyst_New.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunSyst_Newclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunSyst_New${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunSyst_New${application_suffix}

#	@echo "------> (RunSyst_New.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunSyst_New.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunSyst_New_deps RunSyst_New_dependencies.make
#-- end of cleanup_objects ------
