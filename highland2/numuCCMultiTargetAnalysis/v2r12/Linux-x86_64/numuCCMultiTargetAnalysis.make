#-- start of make_header -----------------

#====================================
#  Library numuCCMultiTargetAnalysis
#
#   Generated Mon Oct  9 06:43:32 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_numuCCMultiTargetAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuCCMultiTargetAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_numuCCMultiTargetAnalysis

numuCCMultiTargetAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiTargetAnalysis = /tmp/CMT_$(numuCCMultiTargetAnalysis_tag)_numuCCMultiTargetAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCMultiTargetAnalysis = $(numuCCMultiTargetAnalysis_tag)_numuCCMultiTargetAnalysis.make
cmt_local_tagfile_numuCCMultiTargetAnalysis = $(bin)$(numuCCMultiTargetAnalysis_tag)_numuCCMultiTargetAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiTargetAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiTargetAnalysis = /tmp/CMT_$(numuCCMultiTargetAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCMultiTargetAnalysis = $(numuCCMultiTargetAnalysis_tag).make
cmt_local_tagfile_numuCCMultiTargetAnalysis = $(bin)$(numuCCMultiTargetAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_numuCCMultiTargetAnalysis)

ifdef cmt_numuCCMultiTargetAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_numuCCMultiTargetAnalysis = /tmp/CMT_numuCCMultiTargetAnalysis_numuCCMultiTargetAnalysissetup.make
cmt_local_numuCCMultiTargetAnalysis_makefile = /tmp/CMT_numuCCMultiTargetAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiTargetAnalysis = $(bin)numuCCMultiTargetAnalysis_numuCCMultiTargetAnalysissetup.make
cmt_local_numuCCMultiTargetAnalysis_makefile = $(bin)numuCCMultiTargetAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_numuCCMultiTargetAnalysis = /tmp/CMT_numuCCMultiTargetAnalysissetup.make
cmt_local_numuCCMultiTargetAnalysis_makefile = /tmp/CMT_numuCCMultiTargetAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiTargetAnalysis = $(bin)numuCCMultiTargetAnalysissetup.make
cmt_local_numuCCMultiTargetAnalysis_makefile = $(bin)numuCCMultiTargetAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiTargetAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiTargetAnalysissetup.make
endif

numuCCMultiTargetAnalysis ::


ifdef READONLY
numuCCMultiTargetAnalysis ::
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
	$(echo) 'numuCCMultiTargetAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = numuCCMultiTargetAnalysis/
numuCCMultiTargetAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

numuCCMultiTargetAnalysislibname   = $(bin)$(library_prefix)numuCCMultiTargetAnalysis$(library_suffix)
numuCCMultiTargetAnalysislib       = $(numuCCMultiTargetAnalysislibname).a
numuCCMultiTargetAnalysisstamp     = $(bin)numuCCMultiTargetAnalysis.stamp
numuCCMultiTargetAnalysisshstamp   = $(bin)numuCCMultiTargetAnalysis.shstamp

numuCCMultiTargetAnalysis :: dirs  numuCCMultiTargetAnalysisLIB
	$(echo) "numuCCMultiTargetAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

numuCCMultiTargetAnalysisLIB :: $(numuCCMultiTargetAnalysislib) $(numuCCMultiTargetAnalysisshstamp)
	$(echo) "numuCCMultiTargetAnalysis : library ok"

$(numuCCMultiTargetAnalysislib) :: $(bin)numuCCMultiTargetAnalysis.o $(bin)numuCCMultiTargetSelection.o $(bin)numuCCMultiTargetUtils.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(numuCCMultiTargetAnalysislib) $?
	$(lib_silent) $(ranlib) $(numuCCMultiTargetAnalysislib)
	$(lib_silent) cat /dev/null >$(numuCCMultiTargetAnalysisstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(numuCCMultiTargetAnalysislibname).$(shlibsuffix) :: $(numuCCMultiTargetAnalysislib) requirements $(use_requirements) $(numuCCMultiTargetAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" numuCCMultiTargetAnalysis $(numuCCMultiTargetAnalysis_shlibflags)

$(numuCCMultiTargetAnalysisshstamp) :: $(numuCCMultiTargetAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(numuCCMultiTargetAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(numuCCMultiTargetAnalysisshstamp) ; fi

numuCCMultiTargetAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)numuCCMultiTargetAnalysis.o $(bin)numuCCMultiTargetSelection.o $(bin)numuCCMultiTargetUtils.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf numuCCMultiTargetAnalysis_deps numuCCMultiTargetAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
numuCCMultiTargetAnalysisinstallname = $(library_prefix)numuCCMultiTargetAnalysis$(library_suffix).$(shlibsuffix)

numuCCMultiTargetAnalysis :: numuCCMultiTargetAnalysisinstall

install :: numuCCMultiTargetAnalysisinstall

numuCCMultiTargetAnalysisinstall :: $(install_dir)/$(numuCCMultiTargetAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(numuCCMultiTargetAnalysisinstallname) :: $(bin)$(numuCCMultiTargetAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiTargetAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##numuCCMultiTargetAnalysisclean :: numuCCMultiTargetAnalysisuninstall

uninstall :: numuCCMultiTargetAnalysisuninstall

numuCCMultiTargetAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiTargetAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),numuCCMultiTargetAnalysisclean)

#$(bin)numuCCMultiTargetAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)numuCCMultiTargetAnalysis_dependencies.make : ../src/numuCCMultiTargetAnalysis.cxx ../src/numuCCMultiTargetSelection.cxx ../src/numuCCMultiTargetUtils.cxx $(use_requirements) $(cmt_final_setup_numuCCMultiTargetAnalysis)
	$(echo) "(numuCCMultiTargetAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) numuCCMultiTargetAnalysis -all_sources -out=$@ ../src/numuCCMultiTargetAnalysis.cxx ../src/numuCCMultiTargetSelection.cxx ../src/numuCCMultiTargetUtils.cxx
endif

#$(numuCCMultiTargetAnalysis_dependencies)

-include $(bin)numuCCMultiTargetAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)numuCCMultiTargetAnalysis_dependencies.make : $(numuCCMultiTargetAnalysis_cxx_dependencies)

$(bin)$(binobj)numuCCMultiTargetAnalysis.o : $(numuCCMultiTargetAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiTargetAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuCCMultiTargetAnalysis_pp_cppflags) $(lib_numuCCMultiTargetAnalysis_pp_cppflags) $(numuCCMultiTargetAnalysis_pp_cppflags) $(use_cppflags) $(numuCCMultiTargetAnalysis_cppflags) $(lib_numuCCMultiTargetAnalysis_cppflags) $(numuCCMultiTargetAnalysis_cppflags) $(numuCCMultiTargetAnalysis_cxx_cppflags)  $(src)numuCCMultiTargetAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)numuCCMultiTargetAnalysis_dependencies.make : $(numuCCMultiTargetSelection_cxx_dependencies)

$(bin)$(binobj)numuCCMultiTargetSelection.o : $(numuCCMultiTargetSelection_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiTargetSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuCCMultiTargetAnalysis_pp_cppflags) $(lib_numuCCMultiTargetAnalysis_pp_cppflags) $(numuCCMultiTargetSelection_pp_cppflags) $(use_cppflags) $(numuCCMultiTargetAnalysis_cppflags) $(lib_numuCCMultiTargetAnalysis_cppflags) $(numuCCMultiTargetSelection_cppflags) $(numuCCMultiTargetSelection_cxx_cppflags)  $(src)numuCCMultiTargetSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)numuCCMultiTargetAnalysis_dependencies.make : $(numuCCMultiTargetUtils_cxx_dependencies)

$(bin)$(binobj)numuCCMultiTargetUtils.o : $(numuCCMultiTargetUtils_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiTargetUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuCCMultiTargetAnalysis_pp_cppflags) $(lib_numuCCMultiTargetAnalysis_pp_cppflags) $(numuCCMultiTargetUtils_pp_cppflags) $(use_cppflags) $(numuCCMultiTargetAnalysis_cppflags) $(lib_numuCCMultiTargetAnalysis_cppflags) $(numuCCMultiTargetUtils_cppflags) $(numuCCMultiTargetUtils_cxx_cppflags)  $(src)numuCCMultiTargetUtils.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: numuCCMultiTargetAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(numuCCMultiTargetAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(numuCCMultiTargetAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_numuCCMultiTargetAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiTargetAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

numuCCMultiTargetAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)numuCCMultiTargetAnalysis$(library_suffix).a $(binobj)$(library_prefix)numuCCMultiTargetAnalysis$(library_suffix).s? $(binobj)numuCCMultiTargetAnalysis.stamp $(binobj)numuCCMultiTargetAnalysis.shstamp
#-- end of cleanup_library ---------------

