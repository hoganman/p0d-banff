#-- start of make_header -----------------

#====================================
#  Application RunTutorialAnalysis
#
#   Generated Mon Oct  9 06:42:09 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunTutorialAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunTutorialAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunTutorialAnalysis

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunTutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag)_RunTutorialAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunTutorialAnalysis = $(tutorialAnalysis_tag)_RunTutorialAnalysis.make
cmt_local_tagfile_RunTutorialAnalysis = $(bin)$(tutorialAnalysis_tag)_RunTutorialAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunTutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunTutorialAnalysis = $(tutorialAnalysis_tag).make
cmt_local_tagfile_RunTutorialAnalysis = $(bin)$(tutorialAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunTutorialAnalysis)

ifdef cmt_RunTutorialAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunTutorialAnalysis = /tmp/CMT_tutorialAnalysis_RunTutorialAnalysissetup.make
cmt_local_RunTutorialAnalysis_makefile = /tmp/CMT_RunTutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunTutorialAnalysis = $(bin)tutorialAnalysis_RunTutorialAnalysissetup.make
cmt_local_RunTutorialAnalysis_makefile = $(bin)RunTutorialAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunTutorialAnalysis = /tmp/CMT_tutorialAnalysissetup.make
cmt_local_RunTutorialAnalysis_makefile = /tmp/CMT_RunTutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunTutorialAnalysis = $(bin)tutorialAnalysissetup.make
cmt_local_RunTutorialAnalysis_makefile = $(bin)RunTutorialAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_tutorialAnalysissetup.make
else
cmt_final_setup = $(bin)tutorialAnalysissetup.make
endif

RunTutorialAnalysis ::


ifdef READONLY
RunTutorialAnalysis ::
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
	$(echo) 'RunTutorialAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunTutorialAnalysis/
RunTutorialAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunTutorialAnalysis :: dirs  $(bin)RunTutorialAnalysis${application_suffix}
	$(echo) "RunTutorialAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunTutorialAnalysis${application_suffix} :: $(bin)RunTutorialAnalysis.o $(use_stamps) $(RunTutorialAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunTutorialAnalysis.o $(cmt_installarea_linkopts) $(RunTutorialAnalysis_use_linkopts) $(RunTutorialAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunTutorialAnalysisinstallname = RunTutorialAnalysis${application_suffix}

RunTutorialAnalysis :: RunTutorialAnalysisinstall

install :: RunTutorialAnalysisinstall

RunTutorialAnalysisinstall :: $(install_dir)/$(RunTutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunTutorialAnalysisinstallname) :: $(bin)$(RunTutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunTutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunTutorialAnalysisclean :: RunTutorialAnalysisuninstall

uninstall :: RunTutorialAnalysisuninstall

RunTutorialAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunTutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunTutorialAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunTutorialAnalysisclean)

#$(bin)RunTutorialAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunTutorialAnalysis_dependencies.make : ../app/RunTutorialAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunTutorialAnalysis)
	$(echo) "(RunTutorialAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunTutorialAnalysis -all_sources -out=$@ ../app/RunTutorialAnalysis.cxx
endif

#$(RunTutorialAnalysis_dependencies)

-include $(bin)RunTutorialAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunTutorialAnalysis_dependencies.make : $(RunTutorialAnalysis_cxx_dependencies)

$(bin)$(binobj)RunTutorialAnalysis.o : $(RunTutorialAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunTutorialAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunTutorialAnalysis_pp_cppflags) $(app_RunTutorialAnalysis_pp_cppflags) $(RunTutorialAnalysis_pp_cppflags) $(use_cppflags) $(RunTutorialAnalysis_cppflags) $(app_RunTutorialAnalysis_cppflags) $(RunTutorialAnalysis_cppflags) $(RunTutorialAnalysis_cxx_cppflags) -I../app ../app/RunTutorialAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunTutorialAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunTutorialAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunTutorialAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunTutorialAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunTutorialAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunTutorialAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunTutorialAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunTutorialAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunTutorialAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunTutorialAnalysis${application_suffix}

#	@echo "------> (RunTutorialAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunTutorialAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunTutorialAnalysis_deps RunTutorialAnalysis_dependencies.make
#-- end of cleanup_objects ------
