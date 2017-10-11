#-- start of make_header -----------------

#====================================
#  Application RunNumuBkgInAntiNuModeAnalysis
#
#   Generated Mon Oct  9 06:43:28 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunNumuBkgInAntiNuModeAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuBkgInAntiNuModeAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNumuBkgInAntiNuModeAnalysis

numuBkgInAntiNuModeAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_RunNumuBkgInAntiNuModeAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuBkgInAntiNuModeAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNumuBkgInAntiNuModeAnalysis)

ifdef cmt_RunNumuBkgInAntiNuModeAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysis_RunNumuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_RunNumuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysis_RunNumuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = $(bin)RunNumuBkgInAntiNuModeAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_RunNumuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysissetup.make
cmt_local_RunNumuBkgInAntiNuModeAnalysis_makefile = $(bin)RunNumuBkgInAntiNuModeAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
else
cmt_final_setup = $(bin)numuBkgInAntiNuModeAnalysissetup.make
endif

RunNumuBkgInAntiNuModeAnalysis ::


ifdef READONLY
RunNumuBkgInAntiNuModeAnalysis ::
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
	$(echo) 'RunNumuBkgInAntiNuModeAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNumuBkgInAntiNuModeAnalysis/
RunNumuBkgInAntiNuModeAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNumuBkgInAntiNuModeAnalysis :: dirs  $(bin)RunNumuBkgInAntiNuModeAnalysis${application_suffix}
	$(echo) "RunNumuBkgInAntiNuModeAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNumuBkgInAntiNuModeAnalysis${application_suffix} :: $(bin)RunNumuBkgInAntiNuModeAnalysis.o $(use_stamps) $(RunNumuBkgInAntiNuModeAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunNumuBkgInAntiNuModeAnalysis.o $(cmt_installarea_linkopts) $(RunNumuBkgInAntiNuModeAnalysis_use_linkopts) $(RunNumuBkgInAntiNuModeAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNumuBkgInAntiNuModeAnalysisinstallname = RunNumuBkgInAntiNuModeAnalysis${application_suffix}

RunNumuBkgInAntiNuModeAnalysis :: RunNumuBkgInAntiNuModeAnalysisinstall

install :: RunNumuBkgInAntiNuModeAnalysisinstall

RunNumuBkgInAntiNuModeAnalysisinstall :: $(install_dir)/$(RunNumuBkgInAntiNuModeAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunNumuBkgInAntiNuModeAnalysisinstallname) :: $(bin)$(RunNumuBkgInAntiNuModeAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuBkgInAntiNuModeAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunNumuBkgInAntiNuModeAnalysisclean :: RunNumuBkgInAntiNuModeAnalysisuninstall

uninstall :: RunNumuBkgInAntiNuModeAnalysisuninstall

RunNumuBkgInAntiNuModeAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuBkgInAntiNuModeAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunNumuBkgInAntiNuModeAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunNumuBkgInAntiNuModeAnalysisclean)

#$(bin)RunNumuBkgInAntiNuModeAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNumuBkgInAntiNuModeAnalysis_dependencies.make : ../app/RunNumuBkgInAntiNuModeAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis)
	$(echo) "(RunNumuBkgInAntiNuModeAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunNumuBkgInAntiNuModeAnalysis -all_sources -out=$@ ../app/RunNumuBkgInAntiNuModeAnalysis.cxx
endif

#$(RunNumuBkgInAntiNuModeAnalysis_dependencies)

-include $(bin)RunNumuBkgInAntiNuModeAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunNumuBkgInAntiNuModeAnalysis_dependencies.make : $(RunNumuBkgInAntiNuModeAnalysis_cxx_dependencies)

$(bin)$(binobj)RunNumuBkgInAntiNuModeAnalysis.o : $(RunNumuBkgInAntiNuModeAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunNumuBkgInAntiNuModeAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunNumuBkgInAntiNuModeAnalysis_pp_cppflags) $(app_RunNumuBkgInAntiNuModeAnalysis_pp_cppflags) $(RunNumuBkgInAntiNuModeAnalysis_pp_cppflags) $(use_cppflags) $(RunNumuBkgInAntiNuModeAnalysis_cppflags) $(app_RunNumuBkgInAntiNuModeAnalysis_cppflags) $(RunNumuBkgInAntiNuModeAnalysis_cppflags) $(RunNumuBkgInAntiNuModeAnalysis_cxx_cppflags) -I../app ../app/RunNumuBkgInAntiNuModeAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNumuBkgInAntiNuModeAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunNumuBkgInAntiNuModeAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunNumuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunNumuBkgInAntiNuModeAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunNumuBkgInAntiNuModeAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunNumuBkgInAntiNuModeAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunNumuBkgInAntiNuModeAnalysis${application_suffix}

#	@echo "------> (RunNumuBkgInAntiNuModeAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunNumuBkgInAntiNuModeAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNumuBkgInAntiNuModeAnalysis_deps RunNumuBkgInAntiNuModeAnalysis_dependencies.make
#-- end of cleanup_objects ------
