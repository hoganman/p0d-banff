#-- start of make_header -----------------

#====================================
#  Application RunP0DExampleAnalysis
#
#   Generated Mon Oct  9 06:41:26 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunP0DExampleAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunP0DExampleAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunP0DExampleAnalysis

baseP0DAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DExampleAnalysis = /tmp/CMT_$(baseP0DAnalysis_tag)_RunP0DExampleAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DExampleAnalysis = $(baseP0DAnalysis_tag)_RunP0DExampleAnalysis.make
cmt_local_tagfile_RunP0DExampleAnalysis = $(bin)$(baseP0DAnalysis_tag)_RunP0DExampleAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseP0DAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DExampleAnalysis = /tmp/CMT_$(baseP0DAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DExampleAnalysis = $(baseP0DAnalysis_tag).make
cmt_local_tagfile_RunP0DExampleAnalysis = $(bin)$(baseP0DAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunP0DExampleAnalysis)

ifdef cmt_RunP0DExampleAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunP0DExampleAnalysis = /tmp/CMT_baseP0DAnalysis_RunP0DExampleAnalysissetup.make
cmt_local_RunP0DExampleAnalysis_makefile = /tmp/CMT_RunP0DExampleAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DExampleAnalysis = $(bin)baseP0DAnalysis_RunP0DExampleAnalysissetup.make
cmt_local_RunP0DExampleAnalysis_makefile = $(bin)RunP0DExampleAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunP0DExampleAnalysis = /tmp/CMT_baseP0DAnalysissetup.make
cmt_local_RunP0DExampleAnalysis_makefile = /tmp/CMT_RunP0DExampleAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DExampleAnalysis = $(bin)baseP0DAnalysissetup.make
cmt_local_RunP0DExampleAnalysis_makefile = $(bin)RunP0DExampleAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseP0DAnalysissetup.make
else
cmt_final_setup = $(bin)baseP0DAnalysissetup.make
endif

RunP0DExampleAnalysis ::


ifdef READONLY
RunP0DExampleAnalysis ::
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
	$(echo) 'RunP0DExampleAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunP0DExampleAnalysis/
RunP0DExampleAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunP0DExampleAnalysis :: dirs  $(bin)RunP0DExampleAnalysis${application_suffix}
	$(echo) "RunP0DExampleAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunP0DExampleAnalysis${application_suffix} :: $(bin)RunP0DExampleAnalysis.o $(use_stamps) $(RunP0DExampleAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunP0DExampleAnalysis.o $(cmt_installarea_linkopts) $(RunP0DExampleAnalysis_use_linkopts) $(RunP0DExampleAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunP0DExampleAnalysisinstallname = RunP0DExampleAnalysis${application_suffix}

RunP0DExampleAnalysis :: RunP0DExampleAnalysisinstall

install :: RunP0DExampleAnalysisinstall

RunP0DExampleAnalysisinstall :: $(install_dir)/$(RunP0DExampleAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunP0DExampleAnalysisinstallname) :: $(bin)$(RunP0DExampleAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DExampleAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunP0DExampleAnalysisclean :: RunP0DExampleAnalysisuninstall

uninstall :: RunP0DExampleAnalysisuninstall

RunP0DExampleAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DExampleAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunP0DExampleAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunP0DExampleAnalysisclean)

#$(bin)RunP0DExampleAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunP0DExampleAnalysis_dependencies.make : ../app/RunP0DExampleAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunP0DExampleAnalysis)
	$(echo) "(RunP0DExampleAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunP0DExampleAnalysis -all_sources -out=$@ ../app/RunP0DExampleAnalysis.cxx
endif

#$(RunP0DExampleAnalysis_dependencies)

-include $(bin)RunP0DExampleAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunP0DExampleAnalysis_dependencies.make : $(RunP0DExampleAnalysis_cxx_dependencies)

$(bin)$(binobj)RunP0DExampleAnalysis.o : $(RunP0DExampleAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunP0DExampleAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunP0DExampleAnalysis_pp_cppflags) $(app_RunP0DExampleAnalysis_pp_cppflags) $(RunP0DExampleAnalysis_pp_cppflags) $(use_cppflags) $(RunP0DExampleAnalysis_cppflags) $(app_RunP0DExampleAnalysis_cppflags) $(RunP0DExampleAnalysis_cppflags) $(RunP0DExampleAnalysis_cxx_cppflags) -I../app ../app/RunP0DExampleAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunP0DExampleAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunP0DExampleAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunP0DExampleAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunP0DExampleAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DExampleAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DExampleAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DExampleAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunP0DExampleAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunP0DExampleAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunP0DExampleAnalysis${application_suffix}

#	@echo "------> (RunP0DExampleAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunP0DExampleAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunP0DExampleAnalysis_deps RunP0DExampleAnalysis_dependencies.make
#-- end of cleanup_objects ------
