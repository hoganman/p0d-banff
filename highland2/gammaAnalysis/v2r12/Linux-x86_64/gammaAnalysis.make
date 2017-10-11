#-- start of make_header -----------------

#====================================
#  Library gammaAnalysis
#
#   Generated Mon Oct  9 06:42:16 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_gammaAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_gammaAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_gammaAnalysis

gammaAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_gammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag)_gammaAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_gammaAnalysis = $(gammaAnalysis_tag)_gammaAnalysis.make
cmt_local_tagfile_gammaAnalysis = $(bin)$(gammaAnalysis_tag)_gammaAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

gammaAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_gammaAnalysis = /tmp/CMT_$(gammaAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_gammaAnalysis = $(gammaAnalysis_tag).make
cmt_local_tagfile_gammaAnalysis = $(bin)$(gammaAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_gammaAnalysis)

ifdef cmt_gammaAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_gammaAnalysis = /tmp/CMT_gammaAnalysis_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = /tmp/CMT_gammaAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_gammaAnalysis = $(bin)gammaAnalysis_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = $(bin)gammaAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_gammaAnalysis = /tmp/CMT_gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = /tmp/CMT_gammaAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_gammaAnalysis = $(bin)gammaAnalysissetup.make
cmt_local_gammaAnalysis_makefile = $(bin)gammaAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_gammaAnalysissetup.make
else
cmt_final_setup = $(bin)gammaAnalysissetup.make
endif

gammaAnalysis ::


ifdef READONLY
gammaAnalysis ::
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
	$(echo) 'gammaAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = gammaAnalysis/
gammaAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

gammaAnalysislibname   = $(bin)$(library_prefix)gammaAnalysis$(library_suffix)
gammaAnalysislib       = $(gammaAnalysislibname).a
gammaAnalysisstamp     = $(bin)gammaAnalysis.stamp
gammaAnalysisshstamp   = $(bin)gammaAnalysis.shstamp

gammaAnalysis :: dirs  gammaAnalysisLIB
	$(echo) "gammaAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

gammaAnalysisLIB :: $(gammaAnalysislib) $(gammaAnalysisshstamp)
	$(echo) "gammaAnalysis : library ok"

$(gammaAnalysislib) :: $(bin)gammaAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(gammaAnalysislib) $?
	$(lib_silent) $(ranlib) $(gammaAnalysislib)
	$(lib_silent) cat /dev/null >$(gammaAnalysisstamp)

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

$(gammaAnalysislibname).$(shlibsuffix) :: $(gammaAnalysislib) requirements $(use_requirements) $(gammaAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" gammaAnalysis $(gammaAnalysis_shlibflags)

$(gammaAnalysisshstamp) :: $(gammaAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(gammaAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(gammaAnalysisshstamp) ; fi

gammaAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)gammaAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf gammaAnalysis_deps gammaAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
gammaAnalysisinstallname = $(library_prefix)gammaAnalysis$(library_suffix).$(shlibsuffix)

gammaAnalysis :: gammaAnalysisinstall

install :: gammaAnalysisinstall

gammaAnalysisinstall :: $(install_dir)/$(gammaAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(gammaAnalysisinstallname) :: $(bin)$(gammaAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(gammaAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##gammaAnalysisclean :: gammaAnalysisuninstall

uninstall :: gammaAnalysisuninstall

gammaAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(gammaAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),gammaAnalysisclean)

#$(bin)gammaAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)gammaAnalysis_dependencies.make : ../src/gammaAnalysis.cxx $(use_requirements) $(cmt_final_setup_gammaAnalysis)
	$(echo) "(gammaAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) gammaAnalysis -all_sources -out=$@ ../src/gammaAnalysis.cxx
endif

#$(gammaAnalysis_dependencies)

-include $(bin)gammaAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)gammaAnalysis_dependencies.make : $(gammaAnalysis_cxx_dependencies)

$(bin)$(binobj)gammaAnalysis.o : $(gammaAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)gammaAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(gammaAnalysis_pp_cppflags) $(lib_gammaAnalysis_pp_cppflags) $(gammaAnalysis_pp_cppflags) $(use_cppflags) $(gammaAnalysis_cppflags) $(lib_gammaAnalysis_cppflags) $(gammaAnalysis_cppflags) $(gammaAnalysis_cxx_cppflags)  $(src)gammaAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: gammaAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(gammaAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(gammaAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_gammaAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(gammaAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(gammaAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(gammaAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

gammaAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)gammaAnalysis$(library_suffix).a $(binobj)$(library_prefix)gammaAnalysis$(library_suffix).s? $(binobj)gammaAnalysis.stamp $(binobj)gammaAnalysis.shstamp
#-- end of cleanup_library ---------------

