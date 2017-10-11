#-- start of make_header -----------------

#====================================
#  Application RunAntiNumuCCMultiPiAnalysis
#
#   Generated Mon Oct  9 06:43:12 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunAntiNumuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunAntiNumuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunAntiNumuCCMultiPiAnalysis

antiNumuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = /tmp/CMT_$(antiNumuCCMultiPiAnalysis_tag)_RunAntiNumuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = $(antiNumuCCMultiPiAnalysis_tag)_RunAntiNumuCCMultiPiAnalysis.make
cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = $(bin)$(antiNumuCCMultiPiAnalysis_tag)_RunAntiNumuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = /tmp/CMT_$(antiNumuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = $(antiNumuCCMultiPiAnalysis_tag).make
cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis = $(bin)$(antiNumuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunAntiNumuCCMultiPiAnalysis)

ifdef cmt_RunAntiNumuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunAntiNumuCCMultiPiAnalysis = /tmp/CMT_antiNumuCCMultiPiAnalysis_RunAntiNumuCCMultiPiAnalysissetup.make
cmt_local_RunAntiNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunAntiNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCMultiPiAnalysis = $(bin)antiNumuCCMultiPiAnalysis_RunAntiNumuCCMultiPiAnalysissetup.make
cmt_local_RunAntiNumuCCMultiPiAnalysis_makefile = $(bin)RunAntiNumuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunAntiNumuCCMultiPiAnalysis = /tmp/CMT_antiNumuCCMultiPiAnalysissetup.make
cmt_local_RunAntiNumuCCMultiPiAnalysis_makefile = /tmp/CMT_RunAntiNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCMultiPiAnalysis = $(bin)antiNumuCCMultiPiAnalysissetup.make
cmt_local_RunAntiNumuCCMultiPiAnalysis_makefile = $(bin)RunAntiNumuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCMultiPiAnalysissetup.make
endif

RunAntiNumuCCMultiPiAnalysis ::


ifdef READONLY
RunAntiNumuCCMultiPiAnalysis ::
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
	$(echo) 'RunAntiNumuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunAntiNumuCCMultiPiAnalysis/
RunAntiNumuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunAntiNumuCCMultiPiAnalysis :: dirs  $(bin)RunAntiNumuCCMultiPiAnalysis${application_suffix}
	$(echo) "RunAntiNumuCCMultiPiAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunAntiNumuCCMultiPiAnalysis${application_suffix} :: $(bin)RunAntiNumuCCMultiPiAnalysis.o $(use_stamps) $(RunAntiNumuCCMultiPiAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunAntiNumuCCMultiPiAnalysis.o $(cmt_installarea_linkopts) $(RunAntiNumuCCMultiPiAnalysis_use_linkopts) $(RunAntiNumuCCMultiPiAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunAntiNumuCCMultiPiAnalysisinstallname = RunAntiNumuCCMultiPiAnalysis${application_suffix}

RunAntiNumuCCMultiPiAnalysis :: RunAntiNumuCCMultiPiAnalysisinstall

install :: RunAntiNumuCCMultiPiAnalysisinstall

RunAntiNumuCCMultiPiAnalysisinstall :: $(install_dir)/$(RunAntiNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunAntiNumuCCMultiPiAnalysisinstallname) :: $(bin)$(RunAntiNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunAntiNumuCCMultiPiAnalysisclean :: RunAntiNumuCCMultiPiAnalysisuninstall

uninstall :: RunAntiNumuCCMultiPiAnalysisuninstall

RunAntiNumuCCMultiPiAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunAntiNumuCCMultiPiAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunAntiNumuCCMultiPiAnalysisclean)

#$(bin)RunAntiNumuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunAntiNumuCCMultiPiAnalysis_dependencies.make : ../app/RunAntiNumuCCMultiPiAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunAntiNumuCCMultiPiAnalysis)
	$(echo) "(RunAntiNumuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunAntiNumuCCMultiPiAnalysis -all_sources -out=$@ ../app/RunAntiNumuCCMultiPiAnalysis.cxx
endif

#$(RunAntiNumuCCMultiPiAnalysis_dependencies)

-include $(bin)RunAntiNumuCCMultiPiAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunAntiNumuCCMultiPiAnalysis_dependencies.make : $(RunAntiNumuCCMultiPiAnalysis_cxx_dependencies)

$(bin)$(binobj)RunAntiNumuCCMultiPiAnalysis.o : $(RunAntiNumuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunAntiNumuCCMultiPiAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunAntiNumuCCMultiPiAnalysis_pp_cppflags) $(app_RunAntiNumuCCMultiPiAnalysis_pp_cppflags) $(RunAntiNumuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(RunAntiNumuCCMultiPiAnalysis_cppflags) $(app_RunAntiNumuCCMultiPiAnalysis_cppflags) $(RunAntiNumuCCMultiPiAnalysis_cppflags) $(RunAntiNumuCCMultiPiAnalysis_cxx_cppflags) -I../app ../app/RunAntiNumuCCMultiPiAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunAntiNumuCCMultiPiAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunAntiNumuCCMultiPiAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunAntiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunAntiNumuCCMultiPiAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunAntiNumuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunAntiNumuCCMultiPiAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunAntiNumuCCMultiPiAnalysis${application_suffix}

#	@echo "------> (RunAntiNumuCCMultiPiAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunAntiNumuCCMultiPiAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunAntiNumuCCMultiPiAnalysis_deps RunAntiNumuCCMultiPiAnalysis_dependencies.make
#-- end of cleanup_objects ------
