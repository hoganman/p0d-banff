#-- start of make_header -----------------

#====================================
#  Application RunGammaAnalysis
#
#   Generated Mon Oct  9 06:42:19 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunGammaAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunGammaAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunGammaAnalysis

gammaAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunGammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag)_RunGammaAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunGammaAnalysis = $(gammaAnalysis_tag)_RunGammaAnalysis.make
cmt_local_tagfile_RunGammaAnalysis = $(bin)$(gammaAnalysis_tag)_RunGammaAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

gammaAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunGammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunGammaAnalysis = $(gammaAnalysis_tag).make
cmt_local_tagfile_RunGammaAnalysis = $(bin)$(gammaAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunGammaAnalysis)

ifdef cmt_RunGammaAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunGammaAnalysis = /tmp/CMT_gammaAnalysis_RunGammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = /tmp/CMT_RunGammaAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunGammaAnalysis = $(bin)gammaAnalysis_RunGammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = $(bin)RunGammaAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunGammaAnalysis = /tmp/CMT_gammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = /tmp/CMT_RunGammaAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunGammaAnalysis = $(bin)gammaAnalysissetup.make
cmt_local_RunGammaAnalysis_makefile = $(bin)RunGammaAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_gammaAnalysissetup.make
else
cmt_final_setup = $(bin)gammaAnalysissetup.make
endif

RunGammaAnalysis ::


ifdef READONLY
RunGammaAnalysis ::
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
	$(echo) 'RunGammaAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunGammaAnalysis/
RunGammaAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunGammaAnalysis :: dirs  $(bin)RunGammaAnalysis${application_suffix}
	$(echo) "RunGammaAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunGammaAnalysis${application_suffix} :: $(bin)RunGammaAnalysis.o $(use_stamps) $(RunGammaAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunGammaAnalysis.o $(cmt_installarea_linkopts) $(RunGammaAnalysis_use_linkopts) $(RunGammaAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunGammaAnalysisinstallname = RunGammaAnalysis${application_suffix}

RunGammaAnalysis :: RunGammaAnalysisinstall

install :: RunGammaAnalysisinstall

RunGammaAnalysisinstall :: $(install_dir)/$(RunGammaAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunGammaAnalysisinstallname) :: $(bin)$(RunGammaAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunGammaAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunGammaAnalysisclean :: RunGammaAnalysisuninstall

uninstall :: RunGammaAnalysisuninstall

RunGammaAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunGammaAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunGammaAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunGammaAnalysisclean)

#$(bin)RunGammaAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunGammaAnalysis_dependencies.make : ../app/RunGammaAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunGammaAnalysis)
	$(echo) "(RunGammaAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunGammaAnalysis -all_sources -out=$@ ../app/RunGammaAnalysis.cxx
endif

#$(RunGammaAnalysis_dependencies)

-include $(bin)RunGammaAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunGammaAnalysis_dependencies.make : $(RunGammaAnalysis_cxx_dependencies)

$(bin)$(binobj)RunGammaAnalysis.o : $(RunGammaAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunGammaAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunGammaAnalysis_pp_cppflags) $(app_RunGammaAnalysis_pp_cppflags) $(RunGammaAnalysis_pp_cppflags) $(use_cppflags) $(RunGammaAnalysis_cppflags) $(app_RunGammaAnalysis_cppflags) $(RunGammaAnalysis_cppflags) $(RunGammaAnalysis_cxx_cppflags) -I../app ../app/RunGammaAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunGammaAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunGammaAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunGammaAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunGammaAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunGammaAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunGammaAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunGammaAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunGammaAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunGammaAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunGammaAnalysis${application_suffix}

#	@echo "------> (RunGammaAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunGammaAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunGammaAnalysis_deps RunGammaAnalysis_dependencies.make
#-- end of cleanup_objects ------
