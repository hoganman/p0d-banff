#-- start of make_header -----------------

#====================================
#  Library nueCCAnalysis
#
#   Generated Mon Oct  9 06:41:39 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_nueCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_nueCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_nueCCAnalysis

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_nueCCAnalysis = /tmp/CMT_$(nueCCAnalysis_tag)_nueCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_nueCCAnalysis = $(nueCCAnalysis_tag)_nueCCAnalysis.make
cmt_local_tagfile_nueCCAnalysis = $(bin)$(nueCCAnalysis_tag)_nueCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

nueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_nueCCAnalysis = /tmp/CMT_$(nueCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_nueCCAnalysis = $(nueCCAnalysis_tag).make
cmt_local_tagfile_nueCCAnalysis = $(bin)$(nueCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_nueCCAnalysis)

ifdef cmt_nueCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_nueCCAnalysis = /tmp/CMT_nueCCAnalysis_nueCCAnalysissetup.make
cmt_local_nueCCAnalysis_makefile = /tmp/CMT_nueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_nueCCAnalysis = $(bin)nueCCAnalysis_nueCCAnalysissetup.make
cmt_local_nueCCAnalysis_makefile = $(bin)nueCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_nueCCAnalysis = /tmp/CMT_nueCCAnalysissetup.make
cmt_local_nueCCAnalysis_makefile = /tmp/CMT_nueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_nueCCAnalysis = $(bin)nueCCAnalysissetup.make
cmt_local_nueCCAnalysis_makefile = $(bin)nueCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_nueCCAnalysissetup.make
else
cmt_final_setup = $(bin)nueCCAnalysissetup.make
endif

nueCCAnalysis ::


ifdef READONLY
nueCCAnalysis ::
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
	$(echo) 'nueCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = nueCCAnalysis/
nueCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

nueCCAnalysislibname   = $(bin)$(library_prefix)nueCCAnalysis$(library_suffix)
nueCCAnalysislib       = $(nueCCAnalysislibname).a
nueCCAnalysisstamp     = $(bin)nueCCAnalysis.stamp
nueCCAnalysisshstamp   = $(bin)nueCCAnalysis.shstamp

nueCCAnalysis :: dirs  nueCCAnalysisLIB
	$(echo) "nueCCAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

nueCCAnalysisLIB :: $(nueCCAnalysislib) $(nueCCAnalysisshstamp)
	$(echo) "nueCCAnalysis : library ok"

$(nueCCAnalysislib) :: $(bin)DrawingToolsNueCC.o $(bin)nueCCAnalysis.o $(bin)DrawingToolsNueCC_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(nueCCAnalysislib) $?
	$(lib_silent) $(ranlib) $(nueCCAnalysislib)
	$(lib_silent) cat /dev/null >$(nueCCAnalysisstamp)

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

$(nueCCAnalysislibname).$(shlibsuffix) :: $(nueCCAnalysislib) requirements $(use_requirements) $(nueCCAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" nueCCAnalysis $(nueCCAnalysis_shlibflags)

$(nueCCAnalysisshstamp) :: $(nueCCAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(nueCCAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(nueCCAnalysisshstamp) ; fi

nueCCAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)DrawingToolsNueCC.o $(bin)nueCCAnalysis.o $(bin)DrawingToolsNueCC_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf nueCCAnalysis_deps nueCCAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
nueCCAnalysisinstallname = $(library_prefix)nueCCAnalysis$(library_suffix).$(shlibsuffix)

nueCCAnalysis :: nueCCAnalysisinstall

install :: nueCCAnalysisinstall

nueCCAnalysisinstall :: $(install_dir)/$(nueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(nueCCAnalysisinstallname) :: $(bin)$(nueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(nueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##nueCCAnalysisclean :: nueCCAnalysisuninstall

uninstall :: nueCCAnalysisuninstall

nueCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(nueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),nueCCAnalysisclean)

#$(bin)nueCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)nueCCAnalysis_dependencies.make : ../src/DrawingToolsNueCC.cxx ../src/nueCCAnalysis.cxx ../dict/DrawingToolsNueCC_Dict.cxx $(use_requirements) $(cmt_final_setup_nueCCAnalysis)
	$(echo) "(nueCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) nueCCAnalysis -all_sources -out=$@ ../src/DrawingToolsNueCC.cxx ../src/nueCCAnalysis.cxx ../dict/DrawingToolsNueCC_Dict.cxx
endif

#$(nueCCAnalysis_dependencies)

-include $(bin)nueCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)nueCCAnalysis_dependencies.make : $(DrawingToolsNueCC_cxx_dependencies)

$(bin)$(binobj)DrawingToolsNueCC.o : $(DrawingToolsNueCC_cxx_dependencies)
	$(cpp_echo) $(src)DrawingToolsNueCC.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(nueCCAnalysis_pp_cppflags) $(lib_nueCCAnalysis_pp_cppflags) $(DrawingToolsNueCC_pp_cppflags) $(use_cppflags) $(nueCCAnalysis_cppflags) $(lib_nueCCAnalysis_cppflags) $(DrawingToolsNueCC_cppflags) $(DrawingToolsNueCC_cxx_cppflags)  $(src)DrawingToolsNueCC.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)nueCCAnalysis_dependencies.make : $(nueCCAnalysis_cxx_dependencies)

$(bin)$(binobj)nueCCAnalysis.o : $(nueCCAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)nueCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(nueCCAnalysis_pp_cppflags) $(lib_nueCCAnalysis_pp_cppflags) $(nueCCAnalysis_pp_cppflags) $(use_cppflags) $(nueCCAnalysis_cppflags) $(lib_nueCCAnalysis_cppflags) $(nueCCAnalysis_cppflags) $(nueCCAnalysis_cxx_cppflags)  $(src)nueCCAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)nueCCAnalysis_dependencies.make : $(DrawingToolsNueCC_Dict_cxx_dependencies)

$(bin)$(binobj)DrawingToolsNueCC_Dict.o : $(DrawingToolsNueCC_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DrawingToolsNueCC_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(nueCCAnalysis_pp_cppflags) $(lib_nueCCAnalysis_pp_cppflags) $(DrawingToolsNueCC_Dict_pp_cppflags) $(use_cppflags) $(nueCCAnalysis_cppflags) $(lib_nueCCAnalysis_cppflags) $(DrawingToolsNueCC_Dict_cppflags) $(DrawingToolsNueCC_Dict_cxx_cppflags) -I../dict ../dict/DrawingToolsNueCC_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: nueCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(nueCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(nueCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_nueCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(nueCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(nueCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(nueCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

nueCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)nueCCAnalysis$(library_suffix).a $(binobj)$(library_prefix)nueCCAnalysis$(library_suffix).s? $(binobj)nueCCAnalysis.stamp $(binobj)nueCCAnalysis.shstamp
#-- end of cleanup_library ---------------

