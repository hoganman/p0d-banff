#-- start of make_header -----------------

#====================================
#  Library tutorialAnalysis
#
#   Generated Mon Oct  9 06:41:59 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_tutorialAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_tutorialAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_tutorialAnalysis

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_tutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag)_tutorialAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_tutorialAnalysis = $(tutorialAnalysis_tag)_tutorialAnalysis.make
cmt_local_tagfile_tutorialAnalysis = $(bin)$(tutorialAnalysis_tag)_tutorialAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

tutorialAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_tutorialAnalysis = /tmp/CMT_$(tutorialAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_tutorialAnalysis = $(tutorialAnalysis_tag).make
cmt_local_tagfile_tutorialAnalysis = $(bin)$(tutorialAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_tutorialAnalysis)

ifdef cmt_tutorialAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_tutorialAnalysis = /tmp/CMT_tutorialAnalysis_tutorialAnalysissetup.make
cmt_local_tutorialAnalysis_makefile = /tmp/CMT_tutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_tutorialAnalysis = $(bin)tutorialAnalysis_tutorialAnalysissetup.make
cmt_local_tutorialAnalysis_makefile = $(bin)tutorialAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_tutorialAnalysis = /tmp/CMT_tutorialAnalysissetup.make
cmt_local_tutorialAnalysis_makefile = /tmp/CMT_tutorialAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_tutorialAnalysis = $(bin)tutorialAnalysissetup.make
cmt_local_tutorialAnalysis_makefile = $(bin)tutorialAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_tutorialAnalysissetup.make
else
cmt_final_setup = $(bin)tutorialAnalysissetup.make
endif

tutorialAnalysis ::


ifdef READONLY
tutorialAnalysis ::
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
	$(echo) 'tutorialAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = tutorialAnalysis/
tutorialAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

tutorialAnalysislibname   = $(bin)$(library_prefix)tutorialAnalysis$(library_suffix)
tutorialAnalysislib       = $(tutorialAnalysislibname).a
tutorialAnalysisstamp     = $(bin)tutorialAnalysis.stamp
tutorialAnalysisshstamp   = $(bin)tutorialAnalysis.shstamp

tutorialAnalysis :: dirs  tutorialAnalysisLIB
	$(echo) "tutorialAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

tutorialAnalysisLIB :: $(tutorialAnalysislib) $(tutorialAnalysisshstamp)
	$(echo) "tutorialAnalysis : library ok"

$(tutorialAnalysislib) :: $(bin)tutorialAnalysis.o $(bin)tutorialBranchesSelection.o $(bin)tutorialCorrection.o $(bin)tutorialSelection.o $(bin)tutorialVariationSystematics.o $(bin)tutorialWeightSystematics.o $(bin)useTutorialAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(tutorialAnalysislib) $?
	$(lib_silent) $(ranlib) $(tutorialAnalysislib)
	$(lib_silent) cat /dev/null >$(tutorialAnalysisstamp)

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

$(tutorialAnalysislibname).$(shlibsuffix) :: $(tutorialAnalysislib) requirements $(use_requirements) $(tutorialAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" tutorialAnalysis $(tutorialAnalysis_shlibflags)

$(tutorialAnalysisshstamp) :: $(tutorialAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(tutorialAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(tutorialAnalysisshstamp) ; fi

tutorialAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)tutorialAnalysis.o $(bin)tutorialBranchesSelection.o $(bin)tutorialCorrection.o $(bin)tutorialSelection.o $(bin)tutorialVariationSystematics.o $(bin)tutorialWeightSystematics.o $(bin)useTutorialAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf tutorialAnalysis_deps tutorialAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
tutorialAnalysisinstallname = $(library_prefix)tutorialAnalysis$(library_suffix).$(shlibsuffix)

tutorialAnalysis :: tutorialAnalysisinstall

install :: tutorialAnalysisinstall

tutorialAnalysisinstall :: $(install_dir)/$(tutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(tutorialAnalysisinstallname) :: $(bin)$(tutorialAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(tutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##tutorialAnalysisclean :: tutorialAnalysisuninstall

uninstall :: tutorialAnalysisuninstall

tutorialAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(tutorialAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),tutorialAnalysisclean)

#$(bin)tutorialAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)tutorialAnalysis_dependencies.make : ../src/tutorialAnalysis.cxx ../src/tutorialBranchesSelection.cxx ../src/tutorialCorrection.cxx ../src/tutorialSelection.cxx ../src/tutorialVariationSystematics.cxx ../src/tutorialWeightSystematics.cxx ../src/useTutorialAnalysis.cxx $(use_requirements) $(cmt_final_setup_tutorialAnalysis)
	$(echo) "(tutorialAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) tutorialAnalysis -all_sources -out=$@ ../src/tutorialAnalysis.cxx ../src/tutorialBranchesSelection.cxx ../src/tutorialCorrection.cxx ../src/tutorialSelection.cxx ../src/tutorialVariationSystematics.cxx ../src/tutorialWeightSystematics.cxx ../src/useTutorialAnalysis.cxx
endif

#$(tutorialAnalysis_dependencies)

-include $(bin)tutorialAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialAnalysis_cxx_dependencies)

$(bin)$(binobj)tutorialAnalysis.o : $(tutorialAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)tutorialAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialAnalysis_cppflags) $(tutorialAnalysis_cxx_cppflags)  $(src)tutorialAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialBranchesSelection_cxx_dependencies)

$(bin)$(binobj)tutorialBranchesSelection.o : $(tutorialBranchesSelection_cxx_dependencies)
	$(cpp_echo) $(src)tutorialBranchesSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialBranchesSelection_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialBranchesSelection_cppflags) $(tutorialBranchesSelection_cxx_cppflags)  $(src)tutorialBranchesSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialCorrection_cxx_dependencies)

$(bin)$(binobj)tutorialCorrection.o : $(tutorialCorrection_cxx_dependencies)
	$(cpp_echo) $(src)tutorialCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialCorrection_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialCorrection_cppflags) $(tutorialCorrection_cxx_cppflags)  $(src)tutorialCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialSelection_cxx_dependencies)

$(bin)$(binobj)tutorialSelection.o : $(tutorialSelection_cxx_dependencies)
	$(cpp_echo) $(src)tutorialSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialSelection_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialSelection_cppflags) $(tutorialSelection_cxx_cppflags)  $(src)tutorialSelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialVariationSystematics_cxx_dependencies)

$(bin)$(binobj)tutorialVariationSystematics.o : $(tutorialVariationSystematics_cxx_dependencies)
	$(cpp_echo) $(src)tutorialVariationSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialVariationSystematics_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialVariationSystematics_cppflags) $(tutorialVariationSystematics_cxx_cppflags)  $(src)tutorialVariationSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(tutorialWeightSystematics_cxx_dependencies)

$(bin)$(binobj)tutorialWeightSystematics.o : $(tutorialWeightSystematics_cxx_dependencies)
	$(cpp_echo) $(src)tutorialWeightSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(tutorialWeightSystematics_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(tutorialWeightSystematics_cppflags) $(tutorialWeightSystematics_cxx_cppflags)  $(src)tutorialWeightSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)tutorialAnalysis_dependencies.make : $(useTutorialAnalysis_cxx_dependencies)

$(bin)$(binobj)useTutorialAnalysis.o : $(useTutorialAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)useTutorialAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(tutorialAnalysis_pp_cppflags) $(lib_tutorialAnalysis_pp_cppflags) $(useTutorialAnalysis_pp_cppflags) $(use_cppflags) $(tutorialAnalysis_cppflags) $(lib_tutorialAnalysis_cppflags) $(useTutorialAnalysis_cppflags) $(useTutorialAnalysis_cxx_cppflags)  $(src)useTutorialAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: tutorialAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(tutorialAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(tutorialAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_tutorialAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(tutorialAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(tutorialAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(tutorialAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

tutorialAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)tutorialAnalysis$(library_suffix).a $(binobj)$(library_prefix)tutorialAnalysis$(library_suffix).s? $(binobj)tutorialAnalysis.stamp $(binobj)tutorialAnalysis.shstamp
#-- end of cleanup_library ---------------

