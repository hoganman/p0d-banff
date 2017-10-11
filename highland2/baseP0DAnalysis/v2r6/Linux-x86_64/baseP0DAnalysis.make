#-- start of make_header -----------------

#====================================
#  Library baseP0DAnalysis
#
#   Generated Mon Oct  9 06:41:19 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_baseP0DAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_baseP0DAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_baseP0DAnalysis

baseP0DAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseP0DAnalysis = /tmp/CMT_$(baseP0DAnalysis_tag)_baseP0DAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseP0DAnalysis = $(baseP0DAnalysis_tag)_baseP0DAnalysis.make
cmt_local_tagfile_baseP0DAnalysis = $(bin)$(baseP0DAnalysis_tag)_baseP0DAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseP0DAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseP0DAnalysis = /tmp/CMT_$(baseP0DAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseP0DAnalysis = $(baseP0DAnalysis_tag).make
cmt_local_tagfile_baseP0DAnalysis = $(bin)$(baseP0DAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_baseP0DAnalysis)

ifdef cmt_baseP0DAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_baseP0DAnalysis = /tmp/CMT_baseP0DAnalysis_baseP0DAnalysissetup.make
cmt_local_baseP0DAnalysis_makefile = /tmp/CMT_baseP0DAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseP0DAnalysis = $(bin)baseP0DAnalysis_baseP0DAnalysissetup.make
cmt_local_baseP0DAnalysis_makefile = $(bin)baseP0DAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_baseP0DAnalysis = /tmp/CMT_baseP0DAnalysissetup.make
cmt_local_baseP0DAnalysis_makefile = /tmp/CMT_baseP0DAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseP0DAnalysis = $(bin)baseP0DAnalysissetup.make
cmt_local_baseP0DAnalysis_makefile = $(bin)baseP0DAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseP0DAnalysissetup.make
else
cmt_final_setup = $(bin)baseP0DAnalysissetup.make
endif

baseP0DAnalysis ::


ifdef READONLY
baseP0DAnalysis ::
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
	$(echo) 'baseP0DAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = baseP0DAnalysis/
baseP0DAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

baseP0DAnalysislibname   = $(bin)$(library_prefix)baseP0DAnalysis$(library_suffix)
baseP0DAnalysislib       = $(baseP0DAnalysislibname).a
baseP0DAnalysisstamp     = $(bin)baseP0DAnalysis.stamp
baseP0DAnalysisshstamp   = $(bin)baseP0DAnalysis.shstamp

baseP0DAnalysis :: dirs  baseP0DAnalysisLIB
	$(echo) "baseP0DAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

baseP0DAnalysisLIB :: $(baseP0DAnalysislib) $(baseP0DAnalysisshstamp)
	$(echo) "baseP0DAnalysis : library ok"

$(baseP0DAnalysislib) :: $(bin)baseP0DAnalysis.o $(bin)p0dExampleAnalysis.o $(bin)p0dExampleSelection.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(baseP0DAnalysislib) $?
	$(lib_silent) $(ranlib) $(baseP0DAnalysislib)
	$(lib_silent) cat /dev/null >$(baseP0DAnalysisstamp)

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

$(baseP0DAnalysislibname).$(shlibsuffix) :: $(baseP0DAnalysislib) requirements $(use_requirements) $(baseP0DAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" baseP0DAnalysis $(baseP0DAnalysis_shlibflags)

$(baseP0DAnalysisshstamp) :: $(baseP0DAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(baseP0DAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(baseP0DAnalysisshstamp) ; fi

baseP0DAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)baseP0DAnalysis.o $(bin)p0dExampleAnalysis.o $(bin)p0dExampleSelection.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf baseP0DAnalysis_deps baseP0DAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
baseP0DAnalysisinstallname = $(library_prefix)baseP0DAnalysis$(library_suffix).$(shlibsuffix)

baseP0DAnalysis :: baseP0DAnalysisinstall

install :: baseP0DAnalysisinstall

baseP0DAnalysisinstall :: $(install_dir)/$(baseP0DAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(baseP0DAnalysisinstallname) :: $(bin)$(baseP0DAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseP0DAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##baseP0DAnalysisclean :: baseP0DAnalysisuninstall

uninstall :: baseP0DAnalysisuninstall

baseP0DAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseP0DAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),baseP0DAnalysisclean)

#$(bin)baseP0DAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)baseP0DAnalysis_dependencies.make : ../src/baseP0DAnalysis.cxx ../src/p0dExampleAnalysis.cxx ../src/p0dExampleSelection.cxx $(use_requirements) $(cmt_final_setup_baseP0DAnalysis)
	$(echo) "(baseP0DAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) baseP0DAnalysis -all_sources -out=$@ ../src/baseP0DAnalysis.cxx ../src/p0dExampleAnalysis.cxx ../src/p0dExampleSelection.cxx
endif

#$(baseP0DAnalysis_dependencies)

-include $(bin)baseP0DAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)baseP0DAnalysis_dependencies.make : $(baseP0DAnalysis_cxx_dependencies)

$(bin)$(binobj)baseP0DAnalysis.o : $(baseP0DAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)baseP0DAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseP0DAnalysis_pp_cppflags) $(lib_baseP0DAnalysis_pp_cppflags) $(baseP0DAnalysis_pp_cppflags) $(use_cppflags) $(baseP0DAnalysis_cppflags) $(lib_baseP0DAnalysis_cppflags) $(baseP0DAnalysis_cppflags) $(baseP0DAnalysis_cxx_cppflags)  $(src)baseP0DAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)baseP0DAnalysis_dependencies.make : $(p0dExampleAnalysis_cxx_dependencies)

$(bin)$(binobj)p0dExampleAnalysis.o : $(p0dExampleAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)p0dExampleAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseP0DAnalysis_pp_cppflags) $(lib_baseP0DAnalysis_pp_cppflags) $(p0dExampleAnalysis_pp_cppflags) $(use_cppflags) $(baseP0DAnalysis_cppflags) $(lib_baseP0DAnalysis_cppflags) $(p0dExampleAnalysis_cppflags) $(p0dExampleAnalysis_cxx_cppflags)  $(src)p0dExampleAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)baseP0DAnalysis_dependencies.make : $(p0dExampleSelection_cxx_dependencies)

$(bin)$(binobj)p0dExampleSelection.o : $(p0dExampleSelection_cxx_dependencies)
	$(cpp_echo) $(src)p0dExampleSelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseP0DAnalysis_pp_cppflags) $(lib_baseP0DAnalysis_pp_cppflags) $(p0dExampleSelection_pp_cppflags) $(use_cppflags) $(baseP0DAnalysis_cppflags) $(lib_baseP0DAnalysis_cppflags) $(p0dExampleSelection_cppflags) $(p0dExampleSelection_cxx_cppflags)  $(src)p0dExampleSelection.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: baseP0DAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(baseP0DAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(baseP0DAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_baseP0DAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseP0DAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseP0DAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseP0DAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

baseP0DAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)baseP0DAnalysis$(library_suffix).a $(binobj)$(library_prefix)baseP0DAnalysis$(library_suffix).s? $(binobj)baseP0DAnalysis.stamp $(binobj)baseP0DAnalysis.shstamp
#-- end of cleanup_library ---------------

