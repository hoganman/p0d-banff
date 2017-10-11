#-- start of make_header -----------------

#====================================
#  Library baseAnalysis
#
#   Generated Mon Oct  9 06:41:05 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_baseAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_baseAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_baseAnalysis

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseAnalysis = /tmp/CMT_$(baseAnalysis_tag)_baseAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseAnalysis = $(baseAnalysis_tag)_baseAnalysis.make
cmt_local_tagfile_baseAnalysis = $(bin)$(baseAnalysis_tag)_baseAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

baseAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_baseAnalysis = /tmp/CMT_$(baseAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_baseAnalysis = $(baseAnalysis_tag).make
cmt_local_tagfile_baseAnalysis = $(bin)$(baseAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_baseAnalysis)

ifdef cmt_baseAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_baseAnalysis = /tmp/CMT_baseAnalysis_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = /tmp/CMT_baseAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseAnalysis = $(bin)baseAnalysis_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = $(bin)baseAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_baseAnalysis = /tmp/CMT_baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = /tmp/CMT_baseAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_baseAnalysis = $(bin)baseAnalysissetup.make
cmt_local_baseAnalysis_makefile = $(bin)baseAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_baseAnalysissetup.make
else
cmt_final_setup = $(bin)baseAnalysissetup.make
endif

baseAnalysis ::


ifdef READONLY
baseAnalysis ::
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
	$(echo) 'baseAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = baseAnalysis/
baseAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

baseAnalysislibname   = $(bin)$(library_prefix)baseAnalysis$(library_suffix)
baseAnalysislib       = $(baseAnalysislibname).a
baseAnalysisstamp     = $(bin)baseAnalysis.stamp
baseAnalysisshstamp   = $(bin)baseAnalysis.shstamp

baseAnalysis :: dirs  baseAnalysisLIB
	$(echo) "baseAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

baseAnalysisLIB :: $(baseAnalysislib) $(baseAnalysisshstamp)
	$(echo) "baseAnalysis : library ok"

$(baseAnalysislib) :: $(bin)baseAnalysis.o $(bin)baseToyMaker.o $(bin)DetectorForagerND280.o $(bin)EventPlotterND280.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(baseAnalysislib) $?
	$(lib_silent) $(ranlib) $(baseAnalysislib)
	$(lib_silent) cat /dev/null >$(baseAnalysisstamp)

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

$(baseAnalysislibname).$(shlibsuffix) :: $(baseAnalysislib) requirements $(use_requirements) $(baseAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" baseAnalysis $(baseAnalysis_shlibflags)

$(baseAnalysisshstamp) :: $(baseAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(baseAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(baseAnalysisshstamp) ; fi

baseAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)baseAnalysis.o $(bin)baseToyMaker.o $(bin)DetectorForagerND280.o $(bin)EventPlotterND280.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf baseAnalysis_deps baseAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
baseAnalysisinstallname = $(library_prefix)baseAnalysis$(library_suffix).$(shlibsuffix)

baseAnalysis :: baseAnalysisinstall

install :: baseAnalysisinstall

baseAnalysisinstall :: $(install_dir)/$(baseAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(baseAnalysisinstallname) :: $(bin)$(baseAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##baseAnalysisclean :: baseAnalysisuninstall

uninstall :: baseAnalysisuninstall

baseAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(baseAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),baseAnalysisclean)

#$(bin)baseAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)baseAnalysis_dependencies.make : ../src/baseAnalysis.cxx ../src/baseToyMaker.cxx ../src/EventDisplayND280/DetectorForagerND280.cxx ../src/EventDisplayND280/EventPlotterND280.cxx $(use_requirements) $(cmt_final_setup_baseAnalysis)
	$(echo) "(baseAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) baseAnalysis -all_sources -out=$@ ../src/baseAnalysis.cxx ../src/baseToyMaker.cxx ../src/EventDisplayND280/DetectorForagerND280.cxx ../src/EventDisplayND280/EventPlotterND280.cxx
endif

#$(baseAnalysis_dependencies)

-include $(bin)baseAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)baseAnalysis_dependencies.make : $(baseAnalysis_cxx_dependencies)

$(bin)$(binobj)baseAnalysis.o : $(baseAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)baseAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseAnalysis_pp_cppflags) $(lib_baseAnalysis_pp_cppflags) $(baseAnalysis_pp_cppflags) $(use_cppflags) $(baseAnalysis_cppflags) $(lib_baseAnalysis_cppflags) $(baseAnalysis_cppflags) $(baseAnalysis_cxx_cppflags)  $(src)baseAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)baseAnalysis_dependencies.make : $(baseToyMaker_cxx_dependencies)

$(bin)$(binobj)baseToyMaker.o : $(baseToyMaker_cxx_dependencies)
	$(cpp_echo) $(src)baseToyMaker.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseAnalysis_pp_cppflags) $(lib_baseAnalysis_pp_cppflags) $(baseToyMaker_pp_cppflags) $(use_cppflags) $(baseAnalysis_cppflags) $(lib_baseAnalysis_cppflags) $(baseToyMaker_cppflags) $(baseToyMaker_cxx_cppflags)  $(src)baseToyMaker.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)baseAnalysis_dependencies.make : $(DetectorForagerND280_cxx_dependencies)

$(bin)$(binobj)DetectorForagerND280.o : $(DetectorForagerND280_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplayND280/DetectorForagerND280.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseAnalysis_pp_cppflags) $(lib_baseAnalysis_pp_cppflags) $(DetectorForagerND280_pp_cppflags) $(use_cppflags) $(baseAnalysis_cppflags) $(lib_baseAnalysis_cppflags) $(DetectorForagerND280_cppflags) $(DetectorForagerND280_cxx_cppflags) -I../src/EventDisplayND280 $(src)EventDisplayND280/DetectorForagerND280.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)baseAnalysis_dependencies.make : $(EventPlotterND280_cxx_dependencies)

$(bin)$(binobj)EventPlotterND280.o : $(EventPlotterND280_cxx_dependencies)
	$(cpp_echo) $(src)EventDisplayND280/EventPlotterND280.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(baseAnalysis_pp_cppflags) $(lib_baseAnalysis_pp_cppflags) $(EventPlotterND280_pp_cppflags) $(use_cppflags) $(baseAnalysis_cppflags) $(lib_baseAnalysis_cppflags) $(EventPlotterND280_cppflags) $(EventPlotterND280_cxx_cppflags) -I../src/EventDisplayND280 $(src)EventDisplayND280/EventPlotterND280.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: baseAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(baseAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(baseAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_baseAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(baseAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

baseAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)baseAnalysis$(library_suffix).a $(binobj)$(library_prefix)baseAnalysis$(library_suffix).s? $(binobj)baseAnalysis.stamp $(binobj)baseAnalysis.shstamp
#-- end of cleanup_library ---------------

