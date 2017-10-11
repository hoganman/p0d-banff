#-- start of make_header -----------------

#====================================
#  Library antiNueCCAnalysis
#
#   Generated Mon Oct  9 06:42:24 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_antiNueCCAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_antiNueCCAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_antiNueCCAnalysis

antiNueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNueCCAnalysis = /tmp/CMT_$(antiNueCCAnalysis_tag)_antiNueCCAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNueCCAnalysis = $(antiNueCCAnalysis_tag)_antiNueCCAnalysis.make
cmt_local_tagfile_antiNueCCAnalysis = $(bin)$(antiNueCCAnalysis_tag)_antiNueCCAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNueCCAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNueCCAnalysis = /tmp/CMT_$(antiNueCCAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNueCCAnalysis = $(antiNueCCAnalysis_tag).make
cmt_local_tagfile_antiNueCCAnalysis = $(bin)$(antiNueCCAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_antiNueCCAnalysis)

ifdef cmt_antiNueCCAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_antiNueCCAnalysis = /tmp/CMT_antiNueCCAnalysis_antiNueCCAnalysissetup.make
cmt_local_antiNueCCAnalysis_makefile = /tmp/CMT_antiNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNueCCAnalysis = $(bin)antiNueCCAnalysis_antiNueCCAnalysissetup.make
cmt_local_antiNueCCAnalysis_makefile = $(bin)antiNueCCAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_antiNueCCAnalysis = /tmp/CMT_antiNueCCAnalysissetup.make
cmt_local_antiNueCCAnalysis_makefile = /tmp/CMT_antiNueCCAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNueCCAnalysis = $(bin)antiNueCCAnalysissetup.make
cmt_local_antiNueCCAnalysis_makefile = $(bin)antiNueCCAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNueCCAnalysissetup.make
else
cmt_final_setup = $(bin)antiNueCCAnalysissetup.make
endif

antiNueCCAnalysis ::


ifdef READONLY
antiNueCCAnalysis ::
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
	$(echo) 'antiNueCCAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = antiNueCCAnalysis/
antiNueCCAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

antiNueCCAnalysislibname   = $(bin)$(library_prefix)antiNueCCAnalysis$(library_suffix)
antiNueCCAnalysislib       = $(antiNueCCAnalysislibname).a
antiNueCCAnalysisstamp     = $(bin)antiNueCCAnalysis.stamp
antiNueCCAnalysisshstamp   = $(bin)antiNueCCAnalysis.shstamp

antiNueCCAnalysis :: dirs  antiNueCCAnalysisLIB
	$(echo) "antiNueCCAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

antiNueCCAnalysisLIB :: $(antiNueCCAnalysislib) $(antiNueCCAnalysisshstamp)
	$(echo) "antiNueCCAnalysis : library ok"

$(antiNueCCAnalysislib) :: $(bin)antiNueCCAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(antiNueCCAnalysislib) $?
	$(lib_silent) $(ranlib) $(antiNueCCAnalysislib)
	$(lib_silent) cat /dev/null >$(antiNueCCAnalysisstamp)

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

$(antiNueCCAnalysislibname).$(shlibsuffix) :: $(antiNueCCAnalysislib) requirements $(use_requirements) $(antiNueCCAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" antiNueCCAnalysis $(antiNueCCAnalysis_shlibflags)

$(antiNueCCAnalysisshstamp) :: $(antiNueCCAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(antiNueCCAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(antiNueCCAnalysisshstamp) ; fi

antiNueCCAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)antiNueCCAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf antiNueCCAnalysis_deps antiNueCCAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
antiNueCCAnalysisinstallname = $(library_prefix)antiNueCCAnalysis$(library_suffix).$(shlibsuffix)

antiNueCCAnalysis :: antiNueCCAnalysisinstall

install :: antiNueCCAnalysisinstall

antiNueCCAnalysisinstall :: $(install_dir)/$(antiNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(antiNueCCAnalysisinstallname) :: $(bin)$(antiNueCCAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##antiNueCCAnalysisclean :: antiNueCCAnalysisuninstall

uninstall :: antiNueCCAnalysisuninstall

antiNueCCAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNueCCAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),antiNueCCAnalysisclean)

#$(bin)antiNueCCAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)antiNueCCAnalysis_dependencies.make : ../src/antiNueCCAnalysis.cxx $(use_requirements) $(cmt_final_setup_antiNueCCAnalysis)
	$(echo) "(antiNueCCAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) antiNueCCAnalysis -all_sources -out=$@ ../src/antiNueCCAnalysis.cxx
endif

#$(antiNueCCAnalysis_dependencies)

-include $(bin)antiNueCCAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)antiNueCCAnalysis_dependencies.make : $(antiNueCCAnalysis_cxx_dependencies)

$(bin)$(binobj)antiNueCCAnalysis.o : $(antiNueCCAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)antiNueCCAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNueCCAnalysis_pp_cppflags) $(lib_antiNueCCAnalysis_pp_cppflags) $(antiNueCCAnalysis_pp_cppflags) $(use_cppflags) $(antiNueCCAnalysis_cppflags) $(lib_antiNueCCAnalysis_cppflags) $(antiNueCCAnalysis_cppflags) $(antiNueCCAnalysis_cxx_cppflags)  $(src)antiNueCCAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: antiNueCCAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(antiNueCCAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(antiNueCCAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_antiNueCCAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNueCCAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNueCCAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNueCCAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

antiNueCCAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)antiNueCCAnalysis$(library_suffix).a $(binobj)$(library_prefix)antiNueCCAnalysis$(library_suffix).s? $(binobj)antiNueCCAnalysis.stamp $(binobj)antiNueCCAnalysis.shstamp
#-- end of cleanup_library ---------------

