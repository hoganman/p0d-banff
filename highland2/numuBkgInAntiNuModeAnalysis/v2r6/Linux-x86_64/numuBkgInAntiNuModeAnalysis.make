#-- start of make_header -----------------

#====================================
#  Library numuBkgInAntiNuModeAnalysis
#
#   Generated Mon Oct  9 06:43:25 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_numuBkgInAntiNuModeAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuBkgInAntiNuModeAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_numuBkgInAntiNuModeAnalysis

numuBkgInAntiNuModeAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag)_numuBkgInAntiNuModeAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuBkgInAntiNuModeAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = /tmp/CMT_$(numuBkgInAntiNuModeAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(numuBkgInAntiNuModeAnalysis_tag).make
cmt_local_tagfile_numuBkgInAntiNuModeAnalysis = $(bin)$(numuBkgInAntiNuModeAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_numuBkgInAntiNuModeAnalysis)

ifdef cmt_numuBkgInAntiNuModeAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_numuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysis_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_numuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysis_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = $(bin)numuBkgInAntiNuModeAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_numuBkgInAntiNuModeAnalysis = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = /tmp/CMT_numuBkgInAntiNuModeAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuBkgInAntiNuModeAnalysis = $(bin)numuBkgInAntiNuModeAnalysissetup.make
cmt_local_numuBkgInAntiNuModeAnalysis_makefile = $(bin)numuBkgInAntiNuModeAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuBkgInAntiNuModeAnalysissetup.make
else
cmt_final_setup = $(bin)numuBkgInAntiNuModeAnalysissetup.make
endif

numuBkgInAntiNuModeAnalysis ::


ifdef READONLY
numuBkgInAntiNuModeAnalysis ::
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
	$(echo) 'numuBkgInAntiNuModeAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = numuBkgInAntiNuModeAnalysis/
numuBkgInAntiNuModeAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

numuBkgInAntiNuModeAnalysislibname   = $(bin)$(library_prefix)numuBkgInAntiNuModeAnalysis$(library_suffix)
numuBkgInAntiNuModeAnalysislib       = $(numuBkgInAntiNuModeAnalysislibname).a
numuBkgInAntiNuModeAnalysisstamp     = $(bin)numuBkgInAntiNuModeAnalysis.stamp
numuBkgInAntiNuModeAnalysisshstamp   = $(bin)numuBkgInAntiNuModeAnalysis.shstamp

numuBkgInAntiNuModeAnalysis :: dirs  numuBkgInAntiNuModeAnalysisLIB
	$(echo) "numuBkgInAntiNuModeAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

numuBkgInAntiNuModeAnalysisLIB :: $(numuBkgInAntiNuModeAnalysislib) $(numuBkgInAntiNuModeAnalysisshstamp)
	$(echo) "numuBkgInAntiNuModeAnalysis : library ok"

$(numuBkgInAntiNuModeAnalysislib) :: $(bin)numuBkgInAntiNuModeAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(numuBkgInAntiNuModeAnalysislib) $?
	$(lib_silent) $(ranlib) $(numuBkgInAntiNuModeAnalysislib)
	$(lib_silent) cat /dev/null >$(numuBkgInAntiNuModeAnalysisstamp)

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

$(numuBkgInAntiNuModeAnalysislibname).$(shlibsuffix) :: $(numuBkgInAntiNuModeAnalysislib) requirements $(use_requirements) $(numuBkgInAntiNuModeAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" numuBkgInAntiNuModeAnalysis $(numuBkgInAntiNuModeAnalysis_shlibflags)

$(numuBkgInAntiNuModeAnalysisshstamp) :: $(numuBkgInAntiNuModeAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(numuBkgInAntiNuModeAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(numuBkgInAntiNuModeAnalysisshstamp) ; fi

numuBkgInAntiNuModeAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)numuBkgInAntiNuModeAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf numuBkgInAntiNuModeAnalysis_deps numuBkgInAntiNuModeAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
numuBkgInAntiNuModeAnalysisinstallname = $(library_prefix)numuBkgInAntiNuModeAnalysis$(library_suffix).$(shlibsuffix)

numuBkgInAntiNuModeAnalysis :: numuBkgInAntiNuModeAnalysisinstall

install :: numuBkgInAntiNuModeAnalysisinstall

numuBkgInAntiNuModeAnalysisinstall :: $(install_dir)/$(numuBkgInAntiNuModeAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(numuBkgInAntiNuModeAnalysisinstallname) :: $(bin)$(numuBkgInAntiNuModeAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuBkgInAntiNuModeAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##numuBkgInAntiNuModeAnalysisclean :: numuBkgInAntiNuModeAnalysisuninstall

uninstall :: numuBkgInAntiNuModeAnalysisuninstall

numuBkgInAntiNuModeAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuBkgInAntiNuModeAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),numuBkgInAntiNuModeAnalysisclean)

#$(bin)numuBkgInAntiNuModeAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)numuBkgInAntiNuModeAnalysis_dependencies.make : ../src/numuBkgInAntiNuModeAnalysis.cxx $(use_requirements) $(cmt_final_setup_numuBkgInAntiNuModeAnalysis)
	$(echo) "(numuBkgInAntiNuModeAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) numuBkgInAntiNuModeAnalysis -all_sources -out=$@ ../src/numuBkgInAntiNuModeAnalysis.cxx
endif

#$(numuBkgInAntiNuModeAnalysis_dependencies)

-include $(bin)numuBkgInAntiNuModeAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)numuBkgInAntiNuModeAnalysis_dependencies.make : $(numuBkgInAntiNuModeAnalysis_cxx_dependencies)

$(bin)$(binobj)numuBkgInAntiNuModeAnalysis.o : $(numuBkgInAntiNuModeAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)numuBkgInAntiNuModeAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuBkgInAntiNuModeAnalysis_pp_cppflags) $(lib_numuBkgInAntiNuModeAnalysis_pp_cppflags) $(numuBkgInAntiNuModeAnalysis_pp_cppflags) $(use_cppflags) $(numuBkgInAntiNuModeAnalysis_cppflags) $(lib_numuBkgInAntiNuModeAnalysis_cppflags) $(numuBkgInAntiNuModeAnalysis_cppflags) $(numuBkgInAntiNuModeAnalysis_cxx_cppflags)  $(src)numuBkgInAntiNuModeAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: numuBkgInAntiNuModeAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(numuBkgInAntiNuModeAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(numuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_numuBkgInAntiNuModeAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuBkgInAntiNuModeAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

numuBkgInAntiNuModeAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)numuBkgInAntiNuModeAnalysis$(library_suffix).a $(binobj)$(library_prefix)numuBkgInAntiNuModeAnalysis$(library_suffix).s? $(binobj)numuBkgInAntiNuModeAnalysis.stamp $(binobj)numuBkgInAntiNuModeAnalysis.shstamp
#-- end of cleanup_library ---------------

