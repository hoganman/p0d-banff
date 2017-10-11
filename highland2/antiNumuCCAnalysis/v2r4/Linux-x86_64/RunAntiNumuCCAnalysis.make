#-- start of make_header -----------------

#====================================
#  Application RunAntiNumuCCAnalysis
#
#   Generated Mon Oct  9 06:43:05 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunAntiNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunAntiNumuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunAntiNumuCCAnalysis

antiNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCAnalysis = /tmp/CMT_$(antiNumuCCAnalysis_tag)_RunAntiNumuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCAnalysis = $(antiNumuCCAnalysis_tag)_RunAntiNumuCCAnalysis.make
cmt_local_tagfile_RunAntiNumuCCAnalysis = $(bin)$(antiNumuCCAnalysis_tag)_RunAntiNumuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNumuCCAnalysis = /tmp/CMT_$(antiNumuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNumuCCAnalysis = $(antiNumuCCAnalysis_tag).make
cmt_local_tagfile_RunAntiNumuCCAnalysis = $(bin)$(antiNumuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunAntiNumuCCAnalysis)

ifdef cmt_RunAntiNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunAntiNumuCCAnalysis = /tmp/CMT_antiNumuCCAnalysis_RunAntiNumuCCAnalysissetup.make
cmt_local_RunAntiNumuCCAnalysis_makefile = /tmp/CMT_RunAntiNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCAnalysis = $(bin)antiNumuCCAnalysis_RunAntiNumuCCAnalysissetup.make
cmt_local_RunAntiNumuCCAnalysis_makefile = $(bin)RunAntiNumuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunAntiNumuCCAnalysis = /tmp/CMT_antiNumuCCAnalysissetup.make
cmt_local_RunAntiNumuCCAnalysis_makefile = /tmp/CMT_RunAntiNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNumuCCAnalysis = $(bin)antiNumuCCAnalysissetup.make
cmt_local_RunAntiNumuCCAnalysis_makefile = $(bin)RunAntiNumuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCAnalysissetup.make
endif

RunAntiNumuCCAnalysis ::


ifdef READONLY
RunAntiNumuCCAnalysis ::
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
	$(echo) 'RunAntiNumuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunAntiNumuCCAnalysis/
RunAntiNumuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunAntiNumuCCAnalysis :: dirs  $(bin)RunAntiNumuCCAnalysis${application_suffix}
	$(echo) "RunAntiNumuCCAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunAntiNumuCCAnalysis${application_suffix} :: $(bin)RunAntiNumuCCAnalysis.o $(use_stamps) $(RunAntiNumuCCAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunAntiNumuCCAnalysis.o $(cmt_installarea_linkopts) $(RunAntiNumuCCAnalysis_use_linkopts) $(RunAntiNumuCCAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunAntiNumuCCAnalysisinstallname = RunAntiNumuCCAnalysis${application_suffix}

RunAntiNumuCCAnalysis :: RunAntiNumuCCAnalysisinstall

install :: RunAntiNumuCCAnalysisinstall

RunAntiNumuCCAnalysisinstall :: $(install_dir)/$(RunAntiNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunAntiNumuCCAnalysisinstallname) :: $(bin)$(RunAntiNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunAntiNumuCCAnalysisclean :: RunAntiNumuCCAnalysisuninstall

uninstall :: RunAntiNumuCCAnalysisuninstall

RunAntiNumuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunAntiNumuCCAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunAntiNumuCCAnalysisclean)

#$(bin)RunAntiNumuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunAntiNumuCCAnalysis_dependencies.make : ../app/RunAntiNumuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunAntiNumuCCAnalysis)
	$(echo) "(RunAntiNumuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunAntiNumuCCAnalysis -all_sources -out=$@ ../app/RunAntiNumuCCAnalysis.cxx
endif

#$(RunAntiNumuCCAnalysis_dependencies)

-include $(bin)RunAntiNumuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunAntiNumuCCAnalysis_dependencies.make : $(RunAntiNumuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)RunAntiNumuCCAnalysis.o : $(RunAntiNumuCCAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunAntiNumuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunAntiNumuCCAnalysis_pp_cppflags) $(app_RunAntiNumuCCAnalysis_pp_cppflags) $(RunAntiNumuCCAnalysis_pp_cppflags) $(use_cppflags) $(RunAntiNumuCCAnalysis_cppflags) $(app_RunAntiNumuCCAnalysis_cppflags) $(RunAntiNumuCCAnalysis_cppflags) $(RunAntiNumuCCAnalysis_cxx_cppflags) -I../app ../app/RunAntiNumuCCAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunAntiNumuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunAntiNumuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunAntiNumuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunAntiNumuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunAntiNumuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunAntiNumuCCAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunAntiNumuCCAnalysis${application_suffix}

#	@echo "------> (RunAntiNumuCCAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunAntiNumuCCAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunAntiNumuCCAnalysis_deps RunAntiNumuCCAnalysis_dependencies.make
#-- end of cleanup_objects ------
