#-- start of make_header -----------------

#====================================
#  Application RunNueCCAnalysis
#
#   Generated Mon Oct  9 06:41:55 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunNueCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunNueCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunNueCCAnalysis

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNueCCAnalysis = /tmp/CMT_$(nueCCAnalysis_tag)_RunNueCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNueCCAnalysis = $(nueCCAnalysis_tag)_RunNueCCAnalysis.make
cmt_local_tagfile_RunNueCCAnalysis = $(bin)$(nueCCAnalysis_tag)_RunNueCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunNueCCAnalysis = /tmp/CMT_$(nueCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunNueCCAnalysis = $(nueCCAnalysis_tag).make
cmt_local_tagfile_RunNueCCAnalysis = $(bin)$(nueCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunNueCCAnalysis)

ifdef cmt_RunNueCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunNueCCAnalysis = /tmp/CMT_nueCCAnalysis_RunNueCCAnalysissetup.make
cmt_local_RunNueCCAnalysis_makefile = /tmp/CMT_RunNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNueCCAnalysis = $(bin)nueCCAnalysis_RunNueCCAnalysissetup.make
cmt_local_RunNueCCAnalysis_makefile = $(bin)RunNueCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunNueCCAnalysis = /tmp/CMT_nueCCAnalysissetup.make
cmt_local_RunNueCCAnalysis_makefile = /tmp/CMT_RunNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunNueCCAnalysis = $(bin)nueCCAnalysissetup.make
cmt_local_RunNueCCAnalysis_makefile = $(bin)RunNueCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_nueCCAnalysissetup.make
else
cmt_final_setup = $(bin)nueCCAnalysissetup.make
endif

RunNueCCAnalysis ::


ifdef READONLY
RunNueCCAnalysis ::
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
	$(echo) 'RunNueCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunNueCCAnalysis/
RunNueCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunNueCCAnalysis :: dirs  $(bin)RunNueCCAnalysis${application_suffix}
	$(echo) "RunNueCCAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunNueCCAnalysis${application_suffix} :: $(bin)RunNueCCAnalysis.o $(use_stamps) $(RunNueCCAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunNueCCAnalysis.o $(cmt_installarea_linkopts) $(RunNueCCAnalysis_use_linkopts) $(RunNueCCAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunNueCCAnalysisinstallname = RunNueCCAnalysis${application_suffix}

RunNueCCAnalysis :: RunNueCCAnalysisinstall

install :: RunNueCCAnalysisinstall

RunNueCCAnalysisinstall :: $(install_dir)/$(RunNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunNueCCAnalysisinstallname) :: $(bin)$(RunNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunNueCCAnalysisclean :: RunNueCCAnalysisuninstall

uninstall :: RunNueCCAnalysisuninstall

RunNueCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunNueCCAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunNueCCAnalysisclean)

#$(bin)RunNueCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunNueCCAnalysis_dependencies.make : ../app/RunNueCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunNueCCAnalysis)
	$(echo) "(RunNueCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunNueCCAnalysis -all_sources -out=$@ ../app/RunNueCCAnalysis.cxx
endif

#$(RunNueCCAnalysis_dependencies)

-include $(bin)RunNueCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunNueCCAnalysis_dependencies.make : $(RunNueCCAnalysis_cxx_dependencies)

$(bin)$(binobj)RunNueCCAnalysis.o : $(RunNueCCAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunNueCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunNueCCAnalysis_pp_cppflags) $(app_RunNueCCAnalysis_pp_cppflags) $(RunNueCCAnalysis_pp_cppflags) $(use_cppflags) $(RunNueCCAnalysis_cppflags) $(app_RunNueCCAnalysis_cppflags) $(RunNueCCAnalysis_cppflags) $(RunNueCCAnalysis_cxx_cppflags) -I../app ../app/RunNueCCAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunNueCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunNueCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunNueCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunNueCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNueCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNueCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunNueCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunNueCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunNueCCAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunNueCCAnalysis${application_suffix}

#	@echo "------> (RunNueCCAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunNueCCAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunNueCCAnalysis_deps RunNueCCAnalysis_dependencies.make
#-- end of cleanup_objects ------
