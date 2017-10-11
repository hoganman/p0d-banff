#-- start of make_header -----------------

#====================================
#  Library antiNumuCCMultiPiAnalysis
#
#   Generated Mon Oct  9 06:43:09 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_antiNumuCCMultiPiAnalysis_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_antiNumuCCMultiPiAnalysis_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_antiNumuCCMultiPiAnalysis

antiNumuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCMultiPiAnalysis = /tmp/CMT_$(antiNumuCCMultiPiAnalysis_tag)_antiNumuCCMultiPiAnalysis.make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCMultiPiAnalysis = $(antiNumuCCMultiPiAnalysis_tag)_antiNumuCCMultiPiAnalysis.make
cmt_local_tagfile_antiNumuCCMultiPiAnalysis = $(bin)$(antiNumuCCMultiPiAnalysis_tag)_antiNumuCCMultiPiAnalysis.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

antiNumuCCMultiPiAnalysis_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_antiNumuCCMultiPiAnalysis = /tmp/CMT_$(antiNumuCCMultiPiAnalysis_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_antiNumuCCMultiPiAnalysis = $(antiNumuCCMultiPiAnalysis_tag).make
cmt_local_tagfile_antiNumuCCMultiPiAnalysis = $(bin)$(antiNumuCCMultiPiAnalysis_tag).make
endif

endif

-include $(cmt_local_tagfile_antiNumuCCMultiPiAnalysis)

ifdef cmt_antiNumuCCMultiPiAnalysis_has_target_tag

ifdef READONLY
cmt_final_setup_antiNumuCCMultiPiAnalysis = /tmp/CMT_antiNumuCCMultiPiAnalysis_antiNumuCCMultiPiAnalysissetup.make
cmt_local_antiNumuCCMultiPiAnalysis_makefile = /tmp/CMT_antiNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCMultiPiAnalysis = $(bin)antiNumuCCMultiPiAnalysis_antiNumuCCMultiPiAnalysissetup.make
cmt_local_antiNumuCCMultiPiAnalysis_makefile = $(bin)antiNumuCCMultiPiAnalysis.make
endif

else

ifdef READONLY
cmt_final_setup_antiNumuCCMultiPiAnalysis = /tmp/CMT_antiNumuCCMultiPiAnalysissetup.make
cmt_local_antiNumuCCMultiPiAnalysis_makefile = /tmp/CMT_antiNumuCCMultiPiAnalysis$(cmt_lock_pid).make
else
cmt_final_setup_antiNumuCCMultiPiAnalysis = $(bin)antiNumuCCMultiPiAnalysissetup.make
cmt_local_antiNumuCCMultiPiAnalysis_makefile = $(bin)antiNumuCCMultiPiAnalysis.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_antiNumuCCMultiPiAnalysissetup.make
else
cmt_final_setup = $(bin)antiNumuCCMultiPiAnalysissetup.make
endif

antiNumuCCMultiPiAnalysis ::


ifdef READONLY
antiNumuCCMultiPiAnalysis ::
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
	$(echo) 'antiNumuCCMultiPiAnalysis'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = antiNumuCCMultiPiAnalysis/
antiNumuCCMultiPiAnalysis::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

antiNumuCCMultiPiAnalysislibname   = $(bin)$(library_prefix)antiNumuCCMultiPiAnalysis$(library_suffix)
antiNumuCCMultiPiAnalysislib       = $(antiNumuCCMultiPiAnalysislibname).a
antiNumuCCMultiPiAnalysisstamp     = $(bin)antiNumuCCMultiPiAnalysis.stamp
antiNumuCCMultiPiAnalysisshstamp   = $(bin)antiNumuCCMultiPiAnalysis.shstamp

antiNumuCCMultiPiAnalysis :: dirs  antiNumuCCMultiPiAnalysisLIB
	$(echo) "antiNumuCCMultiPiAnalysis ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

antiNumuCCMultiPiAnalysisLIB :: $(antiNumuCCMultiPiAnalysislib) $(antiNumuCCMultiPiAnalysisshstamp)
	$(echo) "antiNumuCCMultiPiAnalysis : library ok"

$(antiNumuCCMultiPiAnalysislib) :: $(bin)antiNumuCCMultiPiAnalysis.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(antiNumuCCMultiPiAnalysislib) $?
	$(lib_silent) $(ranlib) $(antiNumuCCMultiPiAnalysislib)
	$(lib_silent) cat /dev/null >$(antiNumuCCMultiPiAnalysisstamp)

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

$(antiNumuCCMultiPiAnalysislibname).$(shlibsuffix) :: $(antiNumuCCMultiPiAnalysislib) requirements $(use_requirements) $(antiNumuCCMultiPiAnalysisstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" antiNumuCCMultiPiAnalysis $(antiNumuCCMultiPiAnalysis_shlibflags)

$(antiNumuCCMultiPiAnalysisshstamp) :: $(antiNumuCCMultiPiAnalysislibname).$(shlibsuffix)
	$(lib_silent) if test -f $(antiNumuCCMultiPiAnalysislibname).$(shlibsuffix) ; then cat /dev/null >$(antiNumuCCMultiPiAnalysisshstamp) ; fi

antiNumuCCMultiPiAnalysisclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)antiNumuCCMultiPiAnalysis.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf antiNumuCCMultiPiAnalysis_deps antiNumuCCMultiPiAnalysis_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
antiNumuCCMultiPiAnalysisinstallname = $(library_prefix)antiNumuCCMultiPiAnalysis$(library_suffix).$(shlibsuffix)

antiNumuCCMultiPiAnalysis :: antiNumuCCMultiPiAnalysisinstall

install :: antiNumuCCMultiPiAnalysisinstall

antiNumuCCMultiPiAnalysisinstall :: $(install_dir)/$(antiNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(antiNumuCCMultiPiAnalysisinstallname) :: $(bin)$(antiNumuCCMultiPiAnalysisinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##antiNumuCCMultiPiAnalysisclean :: antiNumuCCMultiPiAnalysisuninstall

uninstall :: antiNumuCCMultiPiAnalysisuninstall

antiNumuCCMultiPiAnalysisuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(antiNumuCCMultiPiAnalysisinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),antiNumuCCMultiPiAnalysisclean)

#$(bin)antiNumuCCMultiPiAnalysis_dependencies.make :: dirs

ifndef QUICK
$(bin)antiNumuCCMultiPiAnalysis_dependencies.make : ../src/antiNumuCCMultiPiAnalysis.cxx $(use_requirements) $(cmt_final_setup_antiNumuCCMultiPiAnalysis)
	$(echo) "(antiNumuCCMultiPiAnalysis.make) Rebuilding $@"; \
	  $(build_dependencies) antiNumuCCMultiPiAnalysis -all_sources -out=$@ ../src/antiNumuCCMultiPiAnalysis.cxx
endif

#$(antiNumuCCMultiPiAnalysis_dependencies)

-include $(bin)antiNumuCCMultiPiAnalysis_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)antiNumuCCMultiPiAnalysis_dependencies.make : $(antiNumuCCMultiPiAnalysis_cxx_dependencies)

$(bin)$(binobj)antiNumuCCMultiPiAnalysis.o : $(antiNumuCCMultiPiAnalysis_cxx_dependencies)
	$(cpp_echo) $(src)antiNumuCCMultiPiAnalysis.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(antiNumuCCMultiPiAnalysis_pp_cppflags) $(lib_antiNumuCCMultiPiAnalysis_pp_cppflags) $(antiNumuCCMultiPiAnalysis_pp_cppflags) $(use_cppflags) $(antiNumuCCMultiPiAnalysis_cppflags) $(lib_antiNumuCCMultiPiAnalysis_cppflags) $(antiNumuCCMultiPiAnalysis_cppflags) $(antiNumuCCMultiPiAnalysis_cxx_cppflags)  $(src)antiNumuCCMultiPiAnalysis.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: antiNumuCCMultiPiAnalysisclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(antiNumuCCMultiPiAnalysis.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(antiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_antiNumuCCMultiPiAnalysis)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(antiNumuCCMultiPiAnalysis.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

antiNumuCCMultiPiAnalysisclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)antiNumuCCMultiPiAnalysis$(library_suffix).a $(binobj)$(library_prefix)antiNumuCCMultiPiAnalysis$(library_suffix).s? $(binobj)antiNumuCCMultiPiAnalysis.stamp $(binobj)antiNumuCCMultiPiAnalysis.shstamp
#-- end of cleanup_library ---------------

