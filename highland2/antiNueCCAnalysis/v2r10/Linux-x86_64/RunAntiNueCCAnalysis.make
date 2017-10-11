#-- start of make_header -----------------

#====================================
#  Application RunAntiNueCCAnalysis
#
#   Generated Mon Oct  9 06:42:27 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_RunAntiNueCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_RunAntiNueCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_RunAntiNueCCAnalysis

antiNueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNueCCAnalysis = /tmp/CMT_$(antiNueCCAnalysis_tag)_RunAntiNueCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNueCCAnalysis = $(antiNueCCAnalysis_tag)_RunAntiNueCCAnalysis.make
cmt_local_tagfile_RunAntiNueCCAnalysis = $(bin)$(antiNueCCAnalysis_tag)_RunAntiNueCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_RunAntiNueCCAnalysis = /tmp/CMT_$(antiNueCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_RunAntiNueCCAnalysis = $(antiNueCCAnalysis_tag).make
cmt_local_tagfile_RunAntiNueCCAnalysis = $(bin)$(antiNueCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_RunAntiNueCCAnalysis)

ifdef cmt_RunAntiNueCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_RunAntiNueCCAnalysis = /tmp/CMT_antiNueCCAnalysis_RunAntiNueCCAnalysissetup.make
cmt_local_RunAntiNueCCAnalysis_makefile = /tmp/CMT_RunAntiNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNueCCAnalysis = $(bin)antiNueCCAnalysis_RunAntiNueCCAnalysissetup.make
cmt_local_RunAntiNueCCAnalysis_makefile = $(bin)RunAntiNueCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_RunAntiNueCCAnalysis = /tmp/CMT_antiNueCCAnalysissetup.make
cmt_local_RunAntiNueCCAnalysis_makefile = /tmp/CMT_RunAntiNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_RunAntiNueCCAnalysis = $(bin)antiNueCCAnalysissetup.make
cmt_local_RunAntiNueCCAnalysis_makefile = $(bin)RunAntiNueCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNueCCAnalysissetup.make
else
cmt_final_setup = $(bin)antiNueCCAnalysissetup.make
endif

RunAntiNueCCAnalysis ::


ifdef READONLY
RunAntiNueCCAnalysis ::
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
	$(echo) 'RunAntiNueCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = RunAntiNueCCAnalysis/
RunAntiNueCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of application_header

RunAntiNueCCAnalysis :: dirs  $(bin)RunAntiNueCCAnalysis${application_suffix}
	$(echo) "RunAntiNueCCAnalysis ok"

#-- end of application_header
#-- start of application

$(bin)RunAntiNueCCAnalysis${application_suffix} :: $(bin)RunAntiNueCCAnalysis.o $(use_stamps) $(RunAntiNueCCAnalysisstamps) requirements $(use_requirements)
	$(link_echo) "application $@"
	$(link_silent) $(cpplink) -o $(@).new $(bin)RunAntiNueCCAnalysis.o $(cmt_installarea_linkopts) $(RunAntiNueCCAnalysis_use_linkopts) $(RunAntiNueCCAnalysislinkopts) && mv -f $(@).new $(@)

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/bin
RunAntiNueCCAnalysisinstallname = RunAntiNueCCAnalysis${application_suffix}

RunAntiNueCCAnalysis :: RunAntiNueCCAnalysisinstall

install :: RunAntiNueCCAnalysisinstall

RunAntiNueCCAnalysisinstall :: $(install_dir)/$(RunAntiNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(RunAntiNueCCAnalysisinstallname) :: $(bin)$(RunAntiNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##RunAntiNueCCAnalysisclean :: RunAntiNueCCAnalysisuninstall

uninstall :: RunAntiNueCCAnalysisuninstall

RunAntiNueCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(RunAntiNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#	@echo "------> (RunAntiNueCCAnalysis.make) Removing installed files"
#-- end of application
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),RunAntiNueCCAnalysisclean)

#$(bin)RunAntiNueCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)RunAntiNueCCAnalysis_dependencies.make : ../app/RunAntiNueCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_RunAntiNueCCAnalysis)
	$(echo) "(RunAntiNueCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) RunAntiNueCCAnalysis -all_sources -out=$@ ../app/RunAntiNueCCAnalysis.cxx
endif

#$(RunAntiNueCCAnalysis_dependencies)

-include $(bin)RunAntiNueCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp ------

$(bin)RunAntiNueCCAnalysis_dependencies.make : $(RunAntiNueCCAnalysis_cxx_dependencies)

$(bin)$(binobj)RunAntiNueCCAnalysis.o : $(RunAntiNueCCAnalysis_cxx_dependencies)
	$(cpp_echo) ../app/RunAntiNueCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(RunAntiNueCCAnalysis_pp_cppflags) $(app_RunAntiNueCCAnalysis_pp_cppflags) $(RunAntiNueCCAnalysis_pp_cppflags) $(use_cppflags) $(RunAntiNueCCAnalysis_cppflags) $(app_RunAntiNueCCAnalysis_cppflags) $(RunAntiNueCCAnalysis_cppflags) $(RunAntiNueCCAnalysis_cxx_cppflags) -I../app ../app/RunAntiNueCCAnalysis.cxx

#-- end of cpp ------
#-- start of cleanup_header --------------

clean :: RunAntiNueCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(RunAntiNueCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(RunAntiNueCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_RunAntiNueCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNueCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNueCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(RunAntiNueCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

RunAntiNueCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_application ------
	$(cleanup_echo) RunAntiNueCCAnalysis${application_suffix}
	-$(cleanup_silent) cd $(bin); /bin/rm -f RunAntiNueCCAnalysis${application_suffix}

#	@echo "------> (RunAntiNueCCAnalysis.make) Removing application files"
#-- end of cleanup_application ------
#-- start of cleanup_objects ------
	$(cleanup_echo) objects
	-$(cleanup_silent) /bin/rm -f $(bin)RunAntiNueCCAnalysis.o
	-$(cleanup_silent) cd $(bin); /bin/rm -rf RunAntiNueCCAnalysis_deps RunAntiNueCCAnalysis_dependencies.make
#-- end of cleanup_objects ------
