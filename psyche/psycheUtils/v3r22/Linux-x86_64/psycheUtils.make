#-- start of make_header -----------------

#====================================
#  Library psycheUtils
#
#   Generated Mon Oct  9 06:39:21 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheUtils_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheUtils_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheUtils

psycheUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheUtils = /tmp/CMT_$(psycheUtils_tag)_psycheUtils.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheUtils = $(psycheUtils_tag)_psycheUtils.make
cmt_local_tagfile_psycheUtils = $(bin)$(psycheUtils_tag)_psycheUtils.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheUtils_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheUtils = /tmp/CMT_$(psycheUtils_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheUtils = $(psycheUtils_tag).make
cmt_local_tagfile_psycheUtils = $(bin)$(psycheUtils_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheUtils)

ifdef cmt_psycheUtils_has_target_tag

ifdef READONLY
cmt_final_setup_psycheUtils = /tmp/CMT_psycheUtils_psycheUtilssetup.make
cmt_local_psycheUtils_makefile = /tmp/CMT_psycheUtils$(cmt_lock_pid).make
else
cmt_final_setup_psycheUtils = $(bin)psycheUtils_psycheUtilssetup.make
cmt_local_psycheUtils_makefile = $(bin)psycheUtils.make
endif

else

ifdef READONLY
cmt_final_setup_psycheUtils = /tmp/CMT_psycheUtilssetup.make
cmt_local_psycheUtils_makefile = /tmp/CMT_psycheUtils$(cmt_lock_pid).make
else
cmt_final_setup_psycheUtils = $(bin)psycheUtilssetup.make
cmt_local_psycheUtils_makefile = $(bin)psycheUtils.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheUtilssetup.make
else
cmt_final_setup = $(bin)psycheUtilssetup.make
endif

psycheUtils ::


ifdef READONLY
psycheUtils ::
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
	$(echo) 'psycheUtils'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheUtils/
psycheUtils::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheUtilslibname   = $(bin)$(library_prefix)psycheUtils$(library_suffix)
psycheUtilslib       = $(psycheUtilslibname).a
psycheUtilsstamp     = $(bin)psycheUtils.stamp
psycheUtilsshstamp   = $(bin)psycheUtils.shstamp

psycheUtils :: dirs  psycheUtilsLIB
	$(echo) "psycheUtils ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheUtilsLIB :: $(psycheUtilslib) $(psycheUtilsshstamp)
	$(echo) "psycheUtils : library ok"

$(psycheUtilslib) :: $(bin)AnalysisUtils.o $(bin)BeamBunching.o $(bin)BinnedPDF.o $(bin)BinnedParams.o $(bin)Interaction.o $(bin)KinematicsUtils.o $(bin)NuDirUtils.o $(bin)SubDetUtils.o $(bin)SystematicUtils.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheUtilslib) $?
	$(lib_silent) $(ranlib) $(psycheUtilslib)
	$(lib_silent) cat /dev/null >$(psycheUtilsstamp)

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

$(psycheUtilslibname).$(shlibsuffix) :: $(psycheUtilslib) requirements $(use_requirements) $(psycheUtilsstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheUtils $(psycheUtils_shlibflags)

$(psycheUtilsshstamp) :: $(psycheUtilslibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheUtilslibname).$(shlibsuffix) ; then cat /dev/null >$(psycheUtilsshstamp) ; fi

psycheUtilsclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)AnalysisUtils.o $(bin)BeamBunching.o $(bin)BinnedPDF.o $(bin)BinnedParams.o $(bin)Interaction.o $(bin)KinematicsUtils.o $(bin)NuDirUtils.o $(bin)SubDetUtils.o $(bin)SystematicUtils.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheUtils_deps psycheUtils_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheUtilsinstallname = $(library_prefix)psycheUtils$(library_suffix).$(shlibsuffix)

psycheUtils :: psycheUtilsinstall

install :: psycheUtilsinstall

psycheUtilsinstall :: $(install_dir)/$(psycheUtilsinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheUtilsinstallname) :: $(bin)$(psycheUtilsinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheUtilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheUtilsclean :: psycheUtilsuninstall

uninstall :: psycheUtilsuninstall

psycheUtilsuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheUtilsinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheUtilsclean)

#$(bin)psycheUtils_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheUtils_dependencies.make : ../src/AnalysisUtils.cxx ../src/BeamBunching.cxx ../src/BinnedPDF.cxx ../src/BinnedParams.cxx ../src/Interaction.cxx ../src/KinematicsUtils.cxx ../src/NuDirUtils.cxx ../src/SubDetUtils.cxx ../src/SystematicUtils.cxx $(use_requirements) $(cmt_final_setup_psycheUtils)
	$(echo) "(psycheUtils.make) Rebuilding $@"; \
	  $(build_dependencies) psycheUtils -all_sources -out=$@ ../src/AnalysisUtils.cxx ../src/BeamBunching.cxx ../src/BinnedPDF.cxx ../src/BinnedParams.cxx ../src/Interaction.cxx ../src/KinematicsUtils.cxx ../src/NuDirUtils.cxx ../src/SubDetUtils.cxx ../src/SystematicUtils.cxx
endif

#$(psycheUtils_dependencies)

-include $(bin)psycheUtils_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(AnalysisUtils_cxx_dependencies)

$(bin)$(binobj)AnalysisUtils.o : $(AnalysisUtils_cxx_dependencies)
	$(cpp_echo) $(src)AnalysisUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(AnalysisUtils_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(AnalysisUtils_cppflags) $(AnalysisUtils_cxx_cppflags)  $(src)AnalysisUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(BeamBunching_cxx_dependencies)

$(bin)$(binobj)BeamBunching.o : $(BeamBunching_cxx_dependencies)
	$(cpp_echo) $(src)BeamBunching.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(BeamBunching_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(BeamBunching_cppflags) $(BeamBunching_cxx_cppflags)  $(src)BeamBunching.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(BinnedPDF_cxx_dependencies)

$(bin)$(binobj)BinnedPDF.o : $(BinnedPDF_cxx_dependencies)
	$(cpp_echo) $(src)BinnedPDF.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(BinnedPDF_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(BinnedPDF_cppflags) $(BinnedPDF_cxx_cppflags)  $(src)BinnedPDF.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(BinnedParams_cxx_dependencies)

$(bin)$(binobj)BinnedParams.o : $(BinnedParams_cxx_dependencies)
	$(cpp_echo) $(src)BinnedParams.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(BinnedParams_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(BinnedParams_cppflags) $(BinnedParams_cxx_cppflags)  $(src)BinnedParams.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(Interaction_cxx_dependencies)

$(bin)$(binobj)Interaction.o : $(Interaction_cxx_dependencies)
	$(cpp_echo) $(src)Interaction.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(Interaction_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(Interaction_cppflags) $(Interaction_cxx_cppflags)  $(src)Interaction.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(KinematicsUtils_cxx_dependencies)

$(bin)$(binobj)KinematicsUtils.o : $(KinematicsUtils_cxx_dependencies)
	$(cpp_echo) $(src)KinematicsUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(KinematicsUtils_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(KinematicsUtils_cppflags) $(KinematicsUtils_cxx_cppflags)  $(src)KinematicsUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(NuDirUtils_cxx_dependencies)

$(bin)$(binobj)NuDirUtils.o : $(NuDirUtils_cxx_dependencies)
	$(cpp_echo) $(src)NuDirUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(NuDirUtils_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(NuDirUtils_cppflags) $(NuDirUtils_cxx_cppflags)  $(src)NuDirUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(SubDetUtils_cxx_dependencies)

$(bin)$(binobj)SubDetUtils.o : $(SubDetUtils_cxx_dependencies)
	$(cpp_echo) $(src)SubDetUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(SubDetUtils_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(SubDetUtils_cppflags) $(SubDetUtils_cxx_cppflags)  $(src)SubDetUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheUtils_dependencies.make : $(SystematicUtils_cxx_dependencies)

$(bin)$(binobj)SystematicUtils.o : $(SystematicUtils_cxx_dependencies)
	$(cpp_echo) $(src)SystematicUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheUtils_pp_cppflags) $(lib_psycheUtils_pp_cppflags) $(SystematicUtils_pp_cppflags) $(use_cppflags) $(psycheUtils_cppflags) $(lib_psycheUtils_cppflags) $(SystematicUtils_cppflags) $(SystematicUtils_cxx_cppflags)  $(src)SystematicUtils.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheUtilsclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheUtils.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheUtils.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheUtils)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheUtils.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheUtils.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheUtils.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheUtilsclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheUtils$(library_suffix).a $(binobj)$(library_prefix)psycheUtils$(library_suffix).s? $(binobj)psycheUtils.stamp $(binobj)psycheUtils.shstamp
#-- end of cleanup_library ---------------

