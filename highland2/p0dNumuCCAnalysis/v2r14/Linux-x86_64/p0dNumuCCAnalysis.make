#-- start of make_header -----------------

#====================================
#  Library p0dNumuCCAnalysis
#
#   Generated Mon Oct  9 06:42:39 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_p0dNumuCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_p0dNumuCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_p0dNumuCCAnalysis

p0dNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_p0dNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_p0dNumuCCAnalysis = $(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make
cmt_local_tagfile_p0dNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag)_p0dNumuCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

p0dNumuCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_p0dNumuCCAnalysis = /tmp/CMT_$(p0dNumuCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_p0dNumuCCAnalysis = $(p0dNumuCCAnalysis_tag).make
cmt_local_tagfile_p0dNumuCCAnalysis = $(bin)$(p0dNumuCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_p0dNumuCCAnalysis)

ifdef cmt_p0dNumuCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_p0dNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysis_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = /tmp/CMT_p0dNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_p0dNumuCCAnalysis = $(bin)p0dNumuCCAnalysis_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = $(bin)p0dNumuCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_p0dNumuCCAnalysis = /tmp/CMT_p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = /tmp/CMT_p0dNumuCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_p0dNumuCCAnalysis = $(bin)p0dNumuCCAnalysissetup.make
cmt_local_p0dNumuCCAnalysis_makefile = $(bin)p0dNumuCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_p0dNumuCCAnalysissetup.make
else
cmt_final_setup = $(bin)p0dNumuCCAnalysissetup.make
endif

p0dNumuCCAnalysis ::


ifdef READONLY
p0dNumuCCAnalysis ::
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
	$(echo) 'p0dNumuCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = p0dNumuCCAnalysis/
p0dNumuCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

p0dNumuCCAnalysislibname   = $(bin)$(library_prefix)p0dNumuCCAnalysis$(library_suffix)
p0dNumuCCAnalysislib       = $(p0dNumuCCAnalysislibname).a
p0dNumuCCAnalysisstamp     = $(bin)p0dNumuCCAnalysis.stamp
p0dNumuCCAnalysisshstamp   = $(bin)p0dNumuCCAnalysis.shstamp

p0dNumuCCAnalysis :: dirs  p0dNumuCCAnalysisLIB
	$(echo) "p0dNumuCCAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

p0dNumuCCAnalysisLIB :: $(p0dNumuCCAnalysislib) $(p0dNumuCCAnalysisshstamp)
	$(echo) "p0dNumuCCAnalysis : library ok"

$(p0dNumuCCAnalysislib) :: $(bin)P0DCutUtils.o $(bin)P0dMassSystematics.o $(bin)P0dTrackerOOFVSystematics.o $(bin)p0dAnaUtils.o $(bin)p0dNumuCCAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(p0dNumuCCAnalysislib) $?
	$(lib_silent) $(ranlib) $(p0dNumuCCAnalysislib)
	$(lib_silent) cat /dev/null >$(p0dNumuCCAnalysisstamp)

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

$(p0dNumuCCAnalysislibname).$(shlibsuffix) :: $(p0dNumuCCAnalysislib) requirements $(use_requirements) $(p0dNumuCCAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" p0dNumuCCAnalysis $(p0dNumuCCAnalysis_shlibflags)

$(p0dNumuCCAnalysisshstamp) :: $(p0dNumuCCAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(p0dNumuCCAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(p0dNumuCCAnalysisshstamp) ; fi

p0dNumuCCAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)P0DCutUtils.o $(bin)P0dMassSystematics.o $(bin)P0dTrackerOOFVSystematics.o $(bin)p0dAnaUtils.o $(bin)p0dNumuCCAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf p0dNumuCCAnalysis_deps p0dNumuCCAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
p0dNumuCCAnalysisinstallname = $(library_prefix)p0dNumuCCAnalysis$(library_suffix).$(shlibsuffix)

p0dNumuCCAnalysis :: p0dNumuCCAnalysisinstall

install :: p0dNumuCCAnalysisinstall

p0dNumuCCAnalysisinstall :: $(install_dir)/$(p0dNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(p0dNumuCCAnalysisinstallname) :: $(bin)$(p0dNumuCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(p0dNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##p0dNumuCCAnalysisclean :: p0dNumuCCAnalysisuninstall

uninstall :: p0dNumuCCAnalysisuninstall

p0dNumuCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(p0dNumuCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),p0dNumuCCAnalysisclean)

#$(bin)p0dNumuCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)p0dNumuCCAnalysis_dependencies.make : ../src/P0DCutUtils.cxx ../src/P0dMassSystematics.cxx ../src/P0dTrackerOOFVSystematics.cxx ../src/p0dAnaUtils.cxx ../src/p0dNumuCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_p0dNumuCCAnalysis)
	$(echo) "(p0dNumuCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) p0dNumuCCAnalysis -all_sources -out=$@ ../src/P0DCutUtils.cxx ../src/P0dMassSystematics.cxx ../src/P0dTrackerOOFVSystematics.cxx ../src/p0dAnaUtils.cxx ../src/p0dNumuCCAnalysis.cxx
endif

#$(p0dNumuCCAnalysis_dependencies)

-include $(bin)p0dNumuCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCAnalysis_dependencies.make : $(P0DCutUtils_cxx_dependencies)

$(bin)$(binobj)P0DCutUtils.o : $(P0DCutUtils_cxx_dependencies)
	$(cpp_echo) $(src)P0DCutUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(lib_p0dNumuCCAnalysis_pp_cppflags) $(P0DCutUtils_pp_cppflags) $(use_cppflags) $(p0dNumuCCAnalysis_cppflags) $(lib_p0dNumuCCAnalysis_cppflags) $(P0DCutUtils_cppflags) $(P0DCutUtils_cxx_cppflags)  $(src)P0DCutUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCAnalysis_dependencies.make : $(P0dMassSystematics_cxx_dependencies)

$(bin)$(binobj)P0dMassSystematics.o : $(P0dMassSystematics_cxx_dependencies)
	$(cpp_echo) $(src)P0dMassSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(lib_p0dNumuCCAnalysis_pp_cppflags) $(P0dMassSystematics_pp_cppflags) $(use_cppflags) $(p0dNumuCCAnalysis_cppflags) $(lib_p0dNumuCCAnalysis_cppflags) $(P0dMassSystematics_cppflags) $(P0dMassSystematics_cxx_cppflags)  $(src)P0dMassSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCAnalysis_dependencies.make : $(P0dTrackerOOFVSystematics_cxx_dependencies)

$(bin)$(binobj)P0dTrackerOOFVSystematics.o : $(P0dTrackerOOFVSystematics_cxx_dependencies)
	$(cpp_echo) $(src)P0dTrackerOOFVSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(lib_p0dNumuCCAnalysis_pp_cppflags) $(P0dTrackerOOFVSystematics_pp_cppflags) $(use_cppflags) $(p0dNumuCCAnalysis_cppflags) $(lib_p0dNumuCCAnalysis_cppflags) $(P0dTrackerOOFVSystematics_cppflags) $(P0dTrackerOOFVSystematics_cxx_cppflags)  $(src)P0dTrackerOOFVSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCAnalysis_dependencies.make : $(p0dAnaUtils_cxx_dependencies)

$(bin)$(binobj)p0dAnaUtils.o : $(p0dAnaUtils_cxx_dependencies)
	$(cpp_echo) $(src)p0dAnaUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(lib_p0dNumuCCAnalysis_pp_cppflags) $(p0dAnaUtils_pp_cppflags) $(use_cppflags) $(p0dNumuCCAnalysis_cppflags) $(lib_p0dNumuCCAnalysis_cppflags) $(p0dAnaUtils_cppflags) $(p0dAnaUtils_cxx_cppflags)  $(src)p0dAnaUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCAnalysis_dependencies.make : $(p0dNumuCCAnalysis_cxx_dependencies)

$(bin)$(binobj)p0dNumuCCAnalysis.o : $(p0dNumuCCAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)p0dNumuCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(lib_p0dNumuCCAnalysis_pp_cppflags) $(p0dNumuCCAnalysis_pp_cppflags) $(use_cppflags) $(p0dNumuCCAnalysis_cppflags) $(lib_p0dNumuCCAnalysis_cppflags) $(p0dNumuCCAnalysis_cppflags) $(p0dNumuCCAnalysis_cxx_cppflags)  $(src)p0dNumuCCAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: p0dNumuCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(p0dNumuCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(p0dNumuCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_p0dNumuCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

p0dNumuCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)p0dNumuCCAnalysis$(library_suffix).a $(binobj)$(library_prefix)p0dNumuCCAnalysis$(library_suffix).s? $(binobj)p0dNumuCCAnalysis.stamp $(binobj)p0dNumuCCAnalysis.shstamp
#-- end of cleanup_library ---------------

