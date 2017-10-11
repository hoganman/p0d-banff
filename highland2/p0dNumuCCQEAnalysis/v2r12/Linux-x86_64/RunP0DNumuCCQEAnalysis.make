#-- start of make_header -----------------

#====================================
#  Application RunP0DNumuCCQEAnalysis
#
#   Generated Mon Oct  9 06:42:57 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunP0DNumuCCQEAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunP0DNumuCCQEAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunP0DNumuCCQEAnalysis

p0dNumuCCQEAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCQEAnalysis = /tmp/CMT_$(p0dNumuCCQEAnalysis_tag)_RunP0DNumuCCQEAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DNumuCCQEAnalysis = $(p0dNumuCCQEAnalysis_tag)_RunP0DNumuCCQEAnalysis.make
cmt_local_tagfile_RunP0DNumuCCQEAnalysis = $(bin)$(p0dNumuCCQEAnalysis_tag)_RunP0DNumuCCQEAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

p0dNumuCCQEAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCQEAnalysis = /tmp/CMT_$(p0dNumuCCQEAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DNumuCCQEAnalysis = $(p0dNumuCCQEAnalysis_tag).make
cmt_local_tagfile_RunP0DNumuCCQEAnalysis = $(bin)$(p0dNumuCCQEAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunP0DNumuCCQEAnalysis)

ifdef cmt_RunP0DNumuCCQEAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunP0DNumuCCQEAnalysis = /tmp/CMT_p0dNumuCCQEAnalysis_RunP0DNumuCCQEAnalysissetup.make
cmt_local_RunP0DNumuCCQEAnalysis_makefile = /tmp/CMT_RunP0DNumuCCQEAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DNumuCCQEAnalysis = $(bin)p0dNumuCCQEAnalysis_RunP0DNumuCCQEAnalysissetup.make
cmt_local_RunP0DNumuCCQEAnalysis_makefile = $(bin)RunP0DNumuCCQEAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunP0DNumuCCQEAnalysis = /tmp/CMT_p0dNumuCCQEAnalysissetup.make
cmt_local_RunP0DNumuCCQEAnalysis_makefile = /tmp/CMT_RunP0DNumuCCQEAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DNumuCCQEAnalysis = $(bin)p0dNumuCCQEAnalysissetup.make
cmt_local_RunP0DNumuCCQEAnalysis_makefile = $(bin)RunP0DNumuCCQEAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_p0dNumuCCQEAnalysissetup.make
else
cmt_final_setup = $(bin)p0dNumuCCQEAnalysissetup.make
endif

RunP0DNumuCCQEAnalysis ::


ifdef READONLY
RunP0DNumuCCQEAnalysis ::
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
	$(echo) 'RunP0DNumuCCQEAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunP0DNumuCCQEAnalysis/
RunP0DNumuCCQEAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunP0DNumuCCQEAnalysis :: dirs  $(bin)RunP0DNumuCCQEAnalysis${application_suffix}
	$(echo) "RunP0DNumuCCQEAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunP0DNumuCCQEAnalysis${application_suffix} :: $(bin)RunP0DNumuCCQEAnalysis.o $(use_stamps) $(RunP0DNumuCCQEAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunP0DNumuCCQEAnalysis.o $(cmt_installarea_linkopts) $(RunP0DNumuCCQEAnalysis_use_linkopts) $(RunP0DNumuCCQEAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunP0DNumuCCQEAnalysisinstallname = RunP0DNumuCCQEAnalysis${application_suffix}

RunP0DNumuCCQEAnalysis :: RunP0DNumuCCQEAnalysisinstall

install :: RunP0DNumuCCQEAnalysisinstall

RunP0DNumuCCQEAnalysisinstall :: $(install_dir)/$(RunP0DNumuCCQEAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunP0DNumuCCQEAnalysisinstallname) :: $(bin)$(RunP0DNumuCCQEAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DNumuCCQEAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunP0DNumuCCQEAnalysisclean :: RunP0DNumuCCQEAnalysisuninstall

uninstall :: RunP0DNumuCCQEAnalysisuninstall

RunP0DNumuCCQEAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DNumuCCQEAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunP0DNumuCCQEAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunP0DNumuCCQEAnalysisclean)

#$(bin)RunP0DNumuCCQEAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunP0DNumuCCQEAnalysis_dependencies.make : ../app/RunP0DNumuCCQEAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunP0DNumuCCQEAnalysis)
	$(echo) "(RunP0DNumuCCQEAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunP0DNumuCCQEAnalysis -all_sources -out=$@ ../app/RunP0DNumuCCQEAnalysis.cxx
endif

#$(RunP0DNumuCCQEAnalysis_dependencies)

-include $(bin)RunP0DNumuCCQEAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunP0DNumuCCQEAnalysis_dependencies.make : $(RunP0DNumuCCQEAnalysis_cxx_dependencies)

$(bin)$(binobj)RunP0DNumuCCQEAnalysis.o : $(RunP0DNumuCCQEAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunP0DNumuCCQEAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunP0DNumuCCQEAnalysis_pp_cppflags) $(app_RunP0DNumuCCQEAnalysis_pp_cppflags) $(RunP0DNumuCCQEAnalysis_pp_cppflags) $(use_cppflags) $(RunP0DNumuCCQEAnalysis_cppflags) $(app_RunP0DNumuCCQEAnalysis_cppflags) $(RunP0DNumuCCQEAnalysis_cppflags) $(RunP0DNumuCCQEAnalysis_cxx_cppflags) -I../app ../app/RunP0DNumuCCQEAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunP0DNumuCCQEAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunP0DNumuCCQEAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunP0DNumuCCQEAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunP0DNumuCCQEAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunP0DNumuCCQEAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunP0DNumuCCQEAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunP0DNumuCCQEAnalysis${application_suffix}

#	@echo "------> (RunP0DNumuCCQEAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunP0DNumuCCQEAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunP0DNumuCCQEAnalysis_deps RunP0DNumuCCQEAnalysis_dependencies.make
#-- end of cleanup_objects ------
