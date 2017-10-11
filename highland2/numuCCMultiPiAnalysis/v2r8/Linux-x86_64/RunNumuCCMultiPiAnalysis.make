#-- start of make_header -----------------

#====================================
#  Application RunNumuCCMultiPiAnalysis
#
#   Generated Mon Oct  9 06:43:20 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunNumuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNumuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNumuCCMultiPiAnalysis

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag)_RunNumuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag)_RunNumuCCMultiPiAnalysis.make
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(bin)$(numuCCMultiPiAnalysis_tag)_RunNumuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag).make
cmt_local_tagfile_RunNumuCCMultiPiAnalysis = $(bin)$(numuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNumuCCMultiPiAnalysis)

ifdef cmt_RunNumuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNumuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysis_RunNumuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysis_RunNumuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = $(bin)RunNumuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNumuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNumuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysissetup.make
cmt_local_RunNumuCCMultiPiAnalysis_makefile = $(bin)RunNumuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiPiAnalysissetup.make
endif

RunNumuCCMultiPiAnalysis ::


ifdef READONLY
RunNumuCCMultiPiAnalysis ::
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
	$(echo) 'RunNumuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNumuCCMultiPiAnalysis/
RunNumuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNumuCCMultiPiAnalysis :: dirs  $(bin)RunNumuCCMultiPiAnalysis${application_suffix}
	$(echo) "RunNumuCCMultiPiAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNumuCCMultiPiAnalysis${application_suffix} :: $(bin)RunNumuCCMultiPiAnalysis.o $(use_stamps) $(RunNumuCCMultiPiAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunNumuCCMultiPiAnalysis.o $(cmt_installarea_linkopts) $(RunNumuCCMultiPiAnalysis_use_linkopts) $(RunNumuCCMultiPiAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNumuCCMultiPiAnalysisinstallname = RunNumuCCMultiPiAnalysis${application_suffix}

RunNumuCCMultiPiAnalysis :: RunNumuCCMultiPiAnalysisinstall

install :: RunNumuCCMultiPiAnalysisinstall

RunNumuCCMultiPiAnalysisinstall :: $(install_dir)/$(RunNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunNumuCCMultiPiAnalysisinstallname) :: $(bin)$(RunNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunNumuCCMultiPiAnalysisclean :: RunNumuCCMultiPiAnalysisuninstall

uninstall :: RunNumuCCMultiPiAnalysisuninstall

RunNumuCCMultiPiAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunNumuCCMultiPiAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunNumuCCMultiPiAnalysisclean)

#$(bin)RunNumuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNumuCCMultiPiAnalysis_dependencies.make : ../app/RunNumuCCMultiPiAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunNumuCCMultiPiAnalysis)
	$(echo) "(RunNumuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunNumuCCMultiPiAnalysis -all_sources -out=$@ ../app/RunNumuCCMultiPiAnalysis.cxx
endif

#$(RunNumuCCMultiPiAnalysis_dependencies)

-include $(bin)RunNumuCCMultiPiAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunNumuCCMultiPiAnalysis_dependencies.make : $(RunNumuCCMultiPiAnalysis_cxx_dependencies)

$(bin)$(binobj)RunNumuCCMultiPiAnalysis.o : $(RunNumuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunNumuCCMultiPiAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunNumuCCMultiPiAnalysis_pp_cppflags) $(app_RunNumuCCMultiPiAnalysis_pp_cppflags) $(RunNumuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(RunNumuCCMultiPiAnalysis_cppflags) $(app_RunNumuCCMultiPiAnalysis_cppflags) $(RunNumuCCMultiPiAnalysis_cppflags) $(RunNumuCCMultiPiAnalysis_cxx_cppflags) -I../app ../app/RunNumuCCMultiPiAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNumuCCMultiPiAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunNumuCCMultiPiAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunNumuCCMultiPiAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunNumuCCMultiPiAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunNumuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunNumuCCMultiPiAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunNumuCCMultiPiAnalysis${application_suffix}

#	@echo "------> (RunNumuCCMultiPiAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunNumuCCMultiPiAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNumuCCMultiPiAnalysis_deps RunNumuCCMultiPiAnalysis_dependencies.make
#-- end of cleanup_objects ------
