#-- start of make_header -----------------

#====================================
#  Library numuCCMultiPiAnalysis
#
#   Generated Mon Oct  9 06:43:17 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_numuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_numuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_numuCCMultiPiAnalysis

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag)_numuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag)_numuCCMultiPiAnalysis.make
cmt_local_tagfile_numuCCMultiPiAnalysis = $(bin)$(numuCCMultiPiAnalysis_tag)_numuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

numuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_numuCCMultiPiAnalysis = /tmp/CMT_$(numuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_numuCCMultiPiAnalysis = $(numuCCMultiPiAnalysis_tag).make
cmt_local_tagfile_numuCCMultiPiAnalysis = $(bin)$(numuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_numuCCMultiPiAnalysis)

ifdef cmt_numuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_numuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysis_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = /tmp/CMT_numuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysis_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = $(bin)numuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_numuCCMultiPiAnalysis = /tmp/CMT_numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = /tmp/CMT_numuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_numuCCMultiPiAnalysis = $(bin)numuCCMultiPiAnalysissetup.make
cmt_local_numuCCMultiPiAnalysis_makefile = $(bin)numuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_numuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)numuCCMultiPiAnalysissetup.make
endif

numuCCMultiPiAnalysis ::


ifdef READONLY
numuCCMultiPiAnalysis ::
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
	$(echo) 'numuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = numuCCMultiPiAnalysis/
numuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

numuCCMultiPiAnalysislibname   = $(bin)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix)
numuCCMultiPiAnalysislib       = $(numuCCMultiPiAnalysislibname).a
numuCCMultiPiAnalysisstamp     = $(bin)numuCCMultiPiAnalysis.stamp
numuCCMultiPiAnalysisshstamp   = $(bin)numuCCMultiPiAnalysis.shstamp

numuCCMultiPiAnalysis :: dirs  numuCCMultiPiAnalysisLIB
	$(echo) "numuCCMultiPiAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

numuCCMultiPiAnalysisLIB :: $(numuCCMultiPiAnalysislib) $(numuCCMultiPiAnalysisshstamp)
	$(echo) "numuCCMultiPiAnalysis : library ok"

$(numuCCMultiPiAnalysislib) :: $(bin)numuCCMultiPiAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(numuCCMultiPiAnalysislib) $?
	$(lib_silent) $(ranlib) $(numuCCMultiPiAnalysislib)
	$(lib_silent) cat /dev/null >$(numuCCMultiPiAnalysisstamp)

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

$(numuCCMultiPiAnalysislibname).$(shlibsuffix) :: $(numuCCMultiPiAnalysislib) requirements $(use_requirements) $(numuCCMultiPiAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" numuCCMultiPiAnalysis $(numuCCMultiPiAnalysis_shlibflags)

$(numuCCMultiPiAnalysisshstamp) :: $(numuCCMultiPiAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(numuCCMultiPiAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(numuCCMultiPiAnalysisshstamp) ; fi

numuCCMultiPiAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)numuCCMultiPiAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf numuCCMultiPiAnalysis_deps numuCCMultiPiAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
numuCCMultiPiAnalysisinstallname = $(library_prefix)numuCCMultiPiAnalysis$(library_suffix).$(shlibsuffix)

numuCCMultiPiAnalysis :: numuCCMultiPiAnalysisinstall

install :: numuCCMultiPiAnalysisinstall

numuCCMultiPiAnalysisinstall :: $(install_dir)/$(numuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(numuCCMultiPiAnalysisinstallname) :: $(bin)$(numuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##numuCCMultiPiAnalysisclean :: numuCCMultiPiAnalysisuninstall

uninstall :: numuCCMultiPiAnalysisuninstall

numuCCMultiPiAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(numuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),numuCCMultiPiAnalysisclean)

#$(bin)numuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)numuCCMultiPiAnalysis_dependencies.make : ../src/numuCCMultiPiAnalysis.cxx $(use_requirements) $(cmt_final_setup_numuCCMultiPiAnalysis)
	$(echo) "(numuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) numuCCMultiPiAnalysis -all_sources -out=$@ ../src/numuCCMultiPiAnalysis.cxx
endif

#$(numuCCMultiPiAnalysis_dependencies)

-include $(bin)numuCCMultiPiAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)numuCCMultiPiAnalysis_dependencies.make : $(numuCCMultiPiAnalysis_cxx_dependencies)

$(bin)$(binobj)numuCCMultiPiAnalysis.o : $(numuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)numuCCMultiPiAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(numuCCMultiPiAnalysis_pp_cppflags) $(lib_numuCCMultiPiAnalysis_pp_cppflags) $(numuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(numuCCMultiPiAnalysis_cppflags) $(lib_numuCCMultiPiAnalysis_cppflags) $(numuCCMultiPiAnalysis_cppflags) $(numuCCMultiPiAnalysis_cxx_cppflags)  $(src)numuCCMultiPiAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: numuCCMultiPiAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(numuCCMultiPiAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(numuCCMultiPiAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_numuCCMultiPiAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(numuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

numuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix).a $(binobj)$(library_prefix)numuCCMultiPiAnalysis$(library_suffix).s? $(binobj)numuCCMultiPiAnalysis.stamp $(binobj)numuCCMultiPiAnalysis.shstamp
#-- end of cleanup_library ---------------

