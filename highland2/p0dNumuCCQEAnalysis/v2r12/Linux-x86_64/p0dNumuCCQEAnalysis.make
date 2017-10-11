#-- start of make_header -----------------

#====================================
#  Library p0dNumuCCQEAnalysis
#
#   Generated Mon Oct  9 06:42:51 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_p0dNumuCCQEAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_p0dNumuCCQEAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_p0dNumuCCQEAnalysis

p0dNumuCCQEAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_p0dNumuCCQEAnalysis = /tmp/CMT_$(p0dNumuCCQEAnalysis_tag)_p0dNumuCCQEAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_p0dNumuCCQEAnalysis = $(p0dNumuCCQEAnalysis_tag)_p0dNumuCCQEAnalysis.make
cmt_local_tagfile_p0dNumuCCQEAnalysis = $(bin)$(p0dNumuCCQEAnalysis_tag)_p0dNumuCCQEAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

p0dNumuCCQEAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_p0dNumuCCQEAnalysis = /tmp/CMT_$(p0dNumuCCQEAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_p0dNumuCCQEAnalysis = $(p0dNumuCCQEAnalysis_tag).make
cmt_local_tagfile_p0dNumuCCQEAnalysis = $(bin)$(p0dNumuCCQEAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_p0dNumuCCQEAnalysis)

ifdef cmt_p0dNumuCCQEAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_p0dNumuCCQEAnalysis = /tmp/CMT_p0dNumuCCQEAnalysis_p0dNumuCCQEAnalysissetup.make
cmt_local_p0dNumuCCQEAnalysis_makefile = /tmp/CMT_p0dNumuCCQEAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_p0dNumuCCQEAnalysis = $(bin)p0dNumuCCQEAnalysis_p0dNumuCCQEAnalysissetup.make
cmt_local_p0dNumuCCQEAnalysis_makefile = $(bin)p0dNumuCCQEAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_p0dNumuCCQEAnalysis = /tmp/CMT_p0dNumuCCQEAnalysissetup.make
cmt_local_p0dNumuCCQEAnalysis_makefile = /tmp/CMT_p0dNumuCCQEAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_p0dNumuCCQEAnalysis = $(bin)p0dNumuCCQEAnalysissetup.make
cmt_local_p0dNumuCCQEAnalysis_makefile = $(bin)p0dNumuCCQEAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_p0dNumuCCQEAnalysissetup.make
else
cmt_final_setup = $(bin)p0dNumuCCQEAnalysissetup.make
endif

p0dNumuCCQEAnalysis ::


ifdef READONLY
p0dNumuCCQEAnalysis ::
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
	$(echo) 'p0dNumuCCQEAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = p0dNumuCCQEAnalysis/
p0dNumuCCQEAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

p0dNumuCCQEAnalysislibname   = $(bin)$(library_prefix)p0dNumuCCQEAnalysis$(library_suffix)
p0dNumuCCQEAnalysislib       = $(p0dNumuCCQEAnalysislibname).a
p0dNumuCCQEAnalysisstamp     = $(bin)p0dNumuCCQEAnalysis.stamp
p0dNumuCCQEAnalysisshstamp   = $(bin)p0dNumuCCQEAnalysis.shstamp

p0dNumuCCQEAnalysis :: dirs  p0dNumuCCQEAnalysisLIB
	$(echo) "p0dNumuCCQEAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

p0dNumuCCQEAnalysisLIB :: $(p0dNumuCCQEAnalysislib) $(p0dNumuCCQEAnalysisshstamp)
	$(echo) "p0dNumuCCQEAnalysis : library ok"

$(p0dNumuCCQEAnalysislib) :: $(bin)PileUpCorrection.o $(bin)p0dCCQEOOFVSystematics.o $(bin)p0dNumuCCQEAnalysis.o $(bin)p0dNumuCCQESelection.o $(bin)p0dPileUpCorrection.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(p0dNumuCCQEAnalysislib) $?
	$(lib_silent) $(ranlib) $(p0dNumuCCQEAnalysislib)
	$(lib_silent) cat /dev/null >$(p0dNumuCCQEAnalysisstamp)

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

$(p0dNumuCCQEAnalysislibname).$(shlibsuffix) :: $(p0dNumuCCQEAnalysislib) requirements $(use_requirements) $(p0dNumuCCQEAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" p0dNumuCCQEAnalysis $(p0dNumuCCQEAnalysis_shlibflags)

$(p0dNumuCCQEAnalysisshstamp) :: $(p0dNumuCCQEAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(p0dNumuCCQEAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(p0dNumuCCQEAnalysisshstamp) ; fi

p0dNumuCCQEAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)PileUpCorrection.o $(bin)p0dCCQEOOFVSystematics.o $(bin)p0dNumuCCQEAnalysis.o $(bin)p0dNumuCCQESelection.o $(bin)p0dPileUpCorrection.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf p0dNumuCCQEAnalysis_deps p0dNumuCCQEAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
p0dNumuCCQEAnalysisinstallname = $(library_prefix)p0dNumuCCQEAnalysis$(library_suffix).$(shlibsuffix)

p0dNumuCCQEAnalysis :: p0dNumuCCQEAnalysisinstall

install :: p0dNumuCCQEAnalysisinstall

p0dNumuCCQEAnalysisinstall :: $(install_dir)/$(p0dNumuCCQEAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(p0dNumuCCQEAnalysisinstallname) :: $(bin)$(p0dNumuCCQEAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(p0dNumuCCQEAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##p0dNumuCCQEAnalysisclean :: p0dNumuCCQEAnalysisuninstall

uninstall :: p0dNumuCCQEAnalysisuninstall

p0dNumuCCQEAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(p0dNumuCCQEAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),p0dNumuCCQEAnalysisclean)

#$(bin)p0dNumuCCQEAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)p0dNumuCCQEAnalysis_dependencies.make : ../src/PileUpCorrection.cxx ../src/p0dCCQEOOFVSystematics.cxx ../src/p0dNumuCCQEAnalysis.cxx ../src/p0dNumuCCQESelection.cxx ../src/p0dPileUpCorrection.cxx $(use_requirements) $(cmt_final_setup_p0dNumuCCQEAnalysis)
	$(echo) "(p0dNumuCCQEAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) p0dNumuCCQEAnalysis -all_sources -out=$@ ../src/PileUpCorrection.cxx ../src/p0dCCQEOOFVSystematics.cxx ../src/p0dNumuCCQEAnalysis.cxx ../src/p0dNumuCCQESelection.cxx ../src/p0dPileUpCorrection.cxx
endif

#$(p0dNumuCCQEAnalysis_dependencies)

-include $(bin)p0dNumuCCQEAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCQEAnalysis_dependencies.make : $(PileUpCorrection_cxx_dependencies)

$(bin)$(binobj)PileUpCorrection.o : $(PileUpCorrection_cxx_dependencies)
	$(cpp_echo) $(src)PileUpCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(lib_p0dNumuCCQEAnalysis_pp_cppflags) $(PileUpCorrection_pp_cppflags) $(use_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(lib_p0dNumuCCQEAnalysis_cppflags) $(PileUpCorrection_cppflags) $(PileUpCorrection_cxx_cppflags)  $(src)PileUpCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCQEAnalysis_dependencies.make : $(p0dCCQEOOFVSystematics_cxx_dependencies)

$(bin)$(binobj)p0dCCQEOOFVSystematics.o : $(p0dCCQEOOFVSystematics_cxx_dependencies)
	$(cpp_echo) $(src)p0dCCQEOOFVSystematics.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(lib_p0dNumuCCQEAnalysis_pp_cppflags) $(p0dCCQEOOFVSystematics_pp_cppflags) $(use_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(lib_p0dNumuCCQEAnalysis_cppflags) $(p0dCCQEOOFVSystematics_cppflags) $(p0dCCQEOOFVSystematics_cxx_cppflags)  $(src)p0dCCQEOOFVSystematics.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCQEAnalysis_dependencies.make : $(p0dNumuCCQEAnalysis_cxx_dependencies)

$(bin)$(binobj)p0dNumuCCQEAnalysis.o : $(p0dNumuCCQEAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)p0dNumuCCQEAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(lib_p0dNumuCCQEAnalysis_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(use_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(lib_p0dNumuCCQEAnalysis_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(p0dNumuCCQEAnalysis_cxx_cppflags)  $(src)p0dNumuCCQEAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCQEAnalysis_dependencies.make : $(p0dNumuCCQESelection_cxx_dependencies)

$(bin)$(binobj)p0dNumuCCQESelection.o : $(p0dNumuCCQESelection_cxx_dependencies)
	$(cpp_echo) $(src)p0dNumuCCQESelection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(lib_p0dNumuCCQEAnalysis_pp_cppflags) $(p0dNumuCCQESelection_pp_cppflags) $(use_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(lib_p0dNumuCCQEAnalysis_cppflags) $(p0dNumuCCQESelection_cppflags) $(p0dNumuCCQESelection_cxx_cppflags)  $(src)p0dNumuCCQESelection.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)p0dNumuCCQEAnalysis_dependencies.make : $(p0dPileUpCorrection_cxx_dependencies)

$(bin)$(binobj)p0dPileUpCorrection.o : $(p0dPileUpCorrection_cxx_dependencies)
	$(cpp_echo) $(src)p0dPileUpCorrection.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(p0dNumuCCQEAnalysis_pp_cppflags) $(lib_p0dNumuCCQEAnalysis_pp_cppflags) $(p0dPileUpCorrection_pp_cppflags) $(use_cppflags) $(p0dNumuCCQEAnalysis_cppflags) $(lib_p0dNumuCCQEAnalysis_cppflags) $(p0dPileUpCorrection_cppflags) $(p0dPileUpCorrection_cxx_cppflags)  $(src)p0dPileUpCorrection.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: p0dNumuCCQEAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(p0dNumuCCQEAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(p0dNumuCCQEAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_p0dNumuCCQEAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(p0dNumuCCQEAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

p0dNumuCCQEAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)p0dNumuCCQEAnalysis$(library_suffix).a $(binobj)$(library_prefix)p0dNumuCCQEAnalysis$(library_suffix).s? $(binobj)p0dNumuCCQEAnalysis.stamp $(binobj)p0dNumuCCQEAnalysis.shstamp
#-- end of cleanup_library ---------------

