#-- start of make_header -----------------

#====================================
#  Application RunP0DNumuCCAnalysis
#
#   Generated Mon Oct  9 06:42:46 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunP0DNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunP0DNumuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunP0DNumuCCAnalysis

p0dNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DNumuCCAnalysis = $(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make
cmt_local_tagfile_RunP0DNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag)_RunP0DNumuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

p0dNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunP0DNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunP0DNumuCCAnalysis = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_RunP0DNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunP0DNumuCCAnalysis)

ifdef cmt_RunP0DNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunP0DNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysis_RunP0DNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = /tmp/CMT_RunP0DNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DNumuCCAnalysis = $(bin)p0dNumuCCAnalysis_RunP0DNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = $(bin)RunP0DNumuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunP0DNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = /tmp/CMT_RunP0DNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunP0DNumuCCAnalysis = $(bin)p0dNumuCCAnalysissetup.make
cmt_local_RunP0DNumuCCAnalysis_makefile = $(bin)RunP0DNumuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_p0dNumuCCAnalysissetup.make
else
cmt_final_setup = $(bin)p0dNumuCCAnalysissetup.make
endif

RunP0DNumuCCAnalysis ::


ifdef READONLY
RunP0DNumuCCAnalysis ::
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
	$(echo) 'RunP0DNumuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunP0DNumuCCAnalysis/
RunP0DNumuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunP0DNumuCCAnalysis :: dirs  $(bin)RunP0DNumuCCAnalysis${application_suffix}
	$(echo) "RunP0DNumuCCAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunP0DNumuCCAnalysis${application_suffix} :: $(bin)RunP0DNumuCCAnalysis.o $(use_stamps) $(RunP0DNumuCCAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunP0DNumuCCAnalysis.o $(cmt_installarea_linkopts) $(RunP0DNumuCCAnalysis_use_linkopts) $(RunP0DNumuCCAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunP0DNumuCCAnalysisinstallname = RunP0DNumuCCAnalysis${application_suffix}

RunP0DNumuCCAnalysis :: RunP0DNumuCCAnalysisinstall

install :: RunP0DNumuCCAnalysisinstall

RunP0DNumuCCAnalysisinstall :: $(install_dir)/$(RunP0DNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunP0DNumuCCAnalysisinstallname) :: $(bin)$(RunP0DNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunP0DNumuCCAnalysisclean :: RunP0DNumuCCAnalysisuninstall

uninstall :: RunP0DNumuCCAnalysisuninstall

RunP0DNumuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunP0DNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunP0DNumuCCAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunP0DNumuCCAnalysisclean)

#$(bin)RunP0DNumuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunP0DNumuCCAnalysis_dependencies.make : ../app/RunP0DNumuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunP0DNumuCCAnalysis)
	$(echo) "(RunP0DNumuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunP0DNumuCCAnalysis -all_sources -out=$@ ../app/RunP0DNumuCCAnalysis.cxx
endif

#$(RunP0DNumuCCAnalysis_dependencies)

-include $(bin)RunP0DNumuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunP0DNumuCCAnalysis_dependencies.make : $(RunP0DNumuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)RunP0DNumuCCAnalysis.o : $(RunP0DNumuCCAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunP0DNumuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunP0DNumuCCAnalysis_pp_cppflags) $(app_RunP0DNumuCCAnalysis_pp_cppflags) $(RunP0DNumuCCAnalysis_pp_cppflags) $(use_cppflags) $(RunP0DNumuCCAnalysis_cppflags) $(app_RunP0DNumuCCAnalysis_cppflags) $(RunP0DNumuCCAnalysis_cppflags) $(RunP0DNumuCCAnalysis_cxx_cppflags) -I../app ../app/RunP0DNumuCCAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunP0DNumuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunP0DNumuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunP0DNumuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunP0DNumuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunP0DNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunP0DNumuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunP0DNumuCCAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunP0DNumuCCAnalysis${application_suffix}

#	@echo "------> (RunP0DNumuCCAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunP0DNumuCCAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunP0DNumuCCAnalysis_deps RunP0DNumuCCAnalysis_dependencies.make
#-- end of cleanup_objects ------
