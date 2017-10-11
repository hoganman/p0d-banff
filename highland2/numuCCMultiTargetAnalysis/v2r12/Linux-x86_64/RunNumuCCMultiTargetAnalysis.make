#-- start of make_header -----------------

#====================================
#  Application RunNumuCCMultiTargetAnalysis
#
#   Generated Mon Oct  9 06:43:38 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunNumuCCMultiTargetAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuCCMultiTargetAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNumuCCMultiTargetAnalysis

numuCCMultiTargetAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = /tmp/CMT_$(numuCCMultiTargetAnalysis_tag)_RunNumuCCMultiTargetAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = $(numuCCMultiTargetAnalysis_tag)_RunNumuCCMultiTargetAnalysis.make
cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = $(bin)$(numuCCMultiTargetAnalysis_tag)_RunNumuCCMultiTargetAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiTargetAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = /tmp/CMT_$(numuCCMultiTargetAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = $(numuCCMultiTargetAnalysis_tag).make
cmt_local_tagfile_RunNumuCCMultiTargetAnalysis = $(bin)$(numuCCMultiTargetAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNumuCCMultiTargetAnalysis)

ifdef cmt_RunNumuCCMultiTargetAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNumuCCMultiTargetAnalysis = /tmp/CMT_numuCCMultiTargetAnalysis_RunNumuCCMultiTargetAnalysissetup.make
cmt_local_RunNumuCCMultiTargetAnalysis_makefile = /tmp/CMT_RunNumuCCMultiTargetAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiTargetAnalysis = $(bin)numuCCMultiTargetAnalysis_RunNumuCCMultiTargetAnalysissetup.make
cmt_local_RunNumuCCMultiTargetAnalysis_makefile = $(bin)RunNumuCCMultiTargetAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNumuCCMultiTargetAnalysis = /tmp/CMT_numuCCMultiTargetAnalysissetup.make
cmt_local_RunNumuCCMultiTargetAnalysis_makefile = /tmp/CMT_RunNumuCCMultiTargetAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiTargetAnalysis = $(bin)numuCCMultiTargetAnalysissetup.make
cmt_local_RunNumuCCMultiTargetAnalysis_makefile = $(bin)RunNumuCCMultiTargetAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiTargetAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiTargetAnalysissetup.make
endif

RunNumuCCMultiTargetAnalysis ::


ifdef READONLY
RunNumuCCMultiTargetAnalysis ::
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
	$(echo) 'RunNumuCCMultiTargetAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNumuCCMultiTargetAnalysis/
RunNumuCCMultiTargetAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNumuCCMultiTargetAnalysis :: dirs  $(bin)RunNumuCCMultiTargetAnalysis${application_suffix}
	$(echo) "RunNumuCCMultiTargetAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNumuCCMultiTargetAnalysis${application_suffix} :: $(bin)RunNumuCCMultiTargetAnalysis.o $(use_stamps) $(RunNumuCCMultiTargetAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunNumuCCMultiTargetAnalysis.o $(cmt_installarea_linkopts) $(RunNumuCCMultiTargetAnalysis_use_linkopts) $(RunNumuCCMultiTargetAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNumuCCMultiTargetAnalysisinstallname = RunNumuCCMultiTargetAnalysis${application_suffix}

RunNumuCCMultiTargetAnalysis :: RunNumuCCMultiTargetAnalysisinstall

install :: RunNumuCCMultiTargetAnalysisinstall

RunNumuCCMultiTargetAnalysisinstall :: $(install_dir)/$(RunNumuCCMultiTargetAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunNumuCCMultiTargetAnalysisinstallname) :: $(bin)$(RunNumuCCMultiTargetAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiTargetAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunNumuCCMultiTargetAnalysisclean :: RunNumuCCMultiTargetAnalysisuninstall

uninstall :: RunNumuCCMultiTargetAnalysisuninstall

RunNumuCCMultiTargetAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiTargetAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunNumuCCMultiTargetAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunNumuCCMultiTargetAnalysisclean)

#$(bin)RunNumuCCMultiTargetAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNumuCCMultiTargetAnalysis_dependencies.make : ../app/RunNumuCCMultiTargetAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunNumuCCMultiTargetAnalysis)
	$(echo) "(RunNumuCCMultiTargetAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunNumuCCMultiTargetAnalysis -all_sources -out=$@ ../app/RunNumuCCMultiTargetAnalysis.cxx
endif

#$(RunNumuCCMultiTargetAnalysis_dependencies)

-include $(bin)RunNumuCCMultiTargetAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunNumuCCMultiTargetAnalysis_dependencies.make : $(RunNumuCCMultiTargetAnalysis_cxx_dependencies)

$(bin)$(binobj)RunNumuCCMultiTargetAnalysis.o : $(RunNumuCCMultiTargetAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunNumuCCMultiTargetAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunNumuCCMultiTargetAnalysis_pp_cppflags) $(app_RunNumuCCMultiTargetAnalysis_pp_cppflags) $(RunNumuCCMultiTargetAnalysis_pp_cppflags) $(use_cppflags) $(RunNumuCCMultiTargetAnalysis_cppflags) $(app_RunNumuCCMultiTargetAnalysis_cppflags) $(RunNumuCCMultiTargetAnalysis_cppflags) $(RunNumuCCMultiTargetAnalysis_cxx_cppflags) -I../app ../app/RunNumuCCMultiTargetAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNumuCCMultiTargetAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunNumuCCMultiTargetAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunNumuCCMultiTargetAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunNumuCCMultiTargetAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunNumuCCMultiTargetAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunNumuCCMultiTargetAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunNumuCCMultiTargetAnalysis${application_suffix}

#	@echo "------> (RunNumuCCMultiTargetAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunNumuCCMultiTargetAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNumuCCMultiTargetAnalysis_deps RunNumuCCMultiTargetAnalysis_dependencies.make
#-- end of cleanup_objects ------
