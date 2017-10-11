#-- start of make_header -----------------

#====================================
#  Application RunUseTutorialAnalysis
#
#   Generated Mon Oct  9 06:42:12 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunUseTutorialAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunUseTutorialAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunUseTutorialAnalysis

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunUseTutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag)_RunUseTutorialAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunUseTutorialAnalysis = $(tutorialAnalysis_tag)_RunUseTutorialAnalysis.make
cmt_local_tagfile_RunUseTutorialAnalysis = $(bin)$(tutorialAnalysis_tag)_RunUseTutorialAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunUseTutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunUseTutorialAnalysis = $(tutorialAnalysis_tag).make
cmt_local_tagfile_RunUseTutorialAnalysis = $(bin)$(tutorialAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunUseTutorialAnalysis)

ifdef cmt_RunUseTutorialAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunUseTutorialAnalysis = /tmp/CMT_tutorialAnalysis_RunUseTutorialAnalysissetup.make
cmt_local_RunUseTutorialAnalysis_makefile = /tmp/CMT_RunUseTutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunUseTutorialAnalysis = $(bin)tutorialAnalysis_RunUseTutorialAnalysissetup.make
cmt_local_RunUseTutorialAnalysis_makefile = $(bin)RunUseTutorialAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunUseTutorialAnalysis = /tmp/CMT_tutorialAnalysissetup.make
cmt_local_RunUseTutorialAnalysis_makefile = /tmp/CMT_RunUseTutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunUseTutorialAnalysis = $(bin)tutorialAnalysissetup.make
cmt_local_RunUseTutorialAnalysis_makefile = $(bin)RunUseTutorialAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_tutorialAnalysissetup.make
else
cmt_final_setup = $(bin)tutorialAnalysissetup.make
endif

RunUseTutorialAnalysis ::


ifdef READONLY
RunUseTutorialAnalysis ::
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
	$(echo) 'RunUseTutorialAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunUseTutorialAnalysis/
RunUseTutorialAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunUseTutorialAnalysis :: dirs  $(bin)RunUseTutorialAnalysis${application_suffix}
	$(echo) "RunUseTutorialAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunUseTutorialAnalysis${application_suffix} :: $(bin)RunUseTutorialAnalysis.o $(use_stamps) $(RunUseTutorialAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunUseTutorialAnalysis.o $(cmt_installarea_linkopts) $(RunUseTutorialAnalysis_use_linkopts) $(RunUseTutorialAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunUseTutorialAnalysisinstallname = RunUseTutorialAnalysis${application_suffix}

RunUseTutorialAnalysis :: RunUseTutorialAnalysisinstall

install :: RunUseTutorialAnalysisinstall

RunUseTutorialAnalysisinstall :: $(install_dir)/$(RunUseTutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunUseTutorialAnalysisinstallname) :: $(bin)$(RunUseTutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunUseTutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunUseTutorialAnalysisclean :: RunUseTutorialAnalysisuninstall

uninstall :: RunUseTutorialAnalysisuninstall

RunUseTutorialAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunUseTutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunUseTutorialAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunUseTutorialAnalysisclean)

#$(bin)RunUseTutorialAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunUseTutorialAnalysis_dependencies.make : ../app/RunUseTutorialAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunUseTutorialAnalysis)
	$(echo) "(RunUseTutorialAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunUseTutorialAnalysis -all_sources -out=$@ ../app/RunUseTutorialAnalysis.cxx
endif

#$(RunUseTutorialAnalysis_dependencies)

-include $(bin)RunUseTutorialAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunUseTutorialAnalysis_dependencies.make : $(RunUseTutorialAnalysis_cxx_dependencies)

$(bin)$(binobj)RunUseTutorialAnalysis.o : $(RunUseTutorialAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunUseTutorialAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunUseTutorialAnalysis_pp_cppflags) $(app_RunUseTutorialAnalysis_pp_cppflags) $(RunUseTutorialAnalysis_pp_cppflags) $(use_cppflags) $(RunUseTutorialAnalysis_cppflags) $(app_RunUseTutorialAnalysis_cppflags) $(RunUseTutorialAnalysis_cppflags) $(RunUseTutorialAnalysis_cxx_cppflags) -I../app ../app/RunUseTutorialAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunUseTutorialAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunUseTutorialAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunUseTutorialAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunUseTutorialAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunUseTutorialAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunUseTutorialAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunUseTutorialAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunUseTutorialAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunUseTutorialAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunUseTutorialAnalysis${application_suffix}

#	@echo "------> (RunUseTutorialAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunUseTutorialAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunUseTutorialAnalysis_deps RunUseTutorialAnalysis_dependencies.make
#-- end of cleanup_objects ------
