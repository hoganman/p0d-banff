#-- start of make_header -----------------

#====================================
#  Library BANFF
#
#   Generated Wed Oct  4 12:22:02 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_BANFF_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_BANFF_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_BANFF

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_BANFF = /tmp/CMT_$(BANFF_tag)_BANFF.make$(cmt_lock_pid)
else
#cmt_local_tagfile_BANFF = $(BANFF_tag)_BANFF.make
cmt_local_tagfile_BANFF = $(bin)$(BANFF_tag)_BANFF.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

BANFF_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_BANFF = /tmp/CMT_$(BANFF_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_BANFF = $(BANFF_tag).make
cmt_local_tagfile_BANFF = $(bin)$(BANFF_tag).make
endif

endif

-include $(cmt_local_tagfile_BANFF)

ifdef cmt_BANFF_has_target_tag

ifdef READONLY
cmt_final_setup_BANFF = /tmp/CMT_BANFF_BANFFsetup.make
cmt_local_BANFF_makefile = /tmp/CMT_BANFF$(cmt_lock_pid).make
else
cmt_final_setup_BANFF = $(bin)BANFF_BANFFsetup.make
cmt_local_BANFF_makefile = $(bin)BANFF.make
endif

else

ifdef READONLY
cmt_final_setup_BANFF = /tmp/CMT_BANFFsetup.make
cmt_local_BANFF_makefile = /tmp/CMT_BANFF$(cmt_lock_pid).make
else
cmt_final_setup_BANFF = $(bin)BANFFsetup.make
cmt_local_BANFF_makefile = $(bin)BANFF.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_BANFFsetup.make
else
cmt_final_setup = $(bin)BANFFsetup.make
endif

BANFF ::


ifdef READONLY
BANFF ::
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
	$(echo) 'BANFF'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = BANFF/
BANFF::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

BANFFlibname   = $(bin)$(library_prefix)BANFF$(library_suffix)
BANFFlib       = $(BANFFlibname).a
BANFFstamp     = $(bin)BANFF.stamp
BANFFshstamp   = $(bin)BANFF.shstamp

BANFF :: dirs  BANFFLIB
	$(echo) "BANFF ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

BANFFLIB :: $(BANFFlib) $(BANFFshstamp)
	$(echo) "BANFF : library ok"

$(BANFFlib) :: $(bin)EigenDecomp.o $(bin)ThrowParms.o $(bin)BANFFEventBase.o $(bin)BANFFInterfaceBase.o $(bin)FitParameters.o $(bin)WeightFile.o $(bin)BANFFBinnedSampleUtils.o $(bin)BANFFBinnedSample.o $(bin)psycheInterface.o $(bin)psycheObservables.o $(bin)Minuit2LikelihoodFitFCN.o $(bin)Minuit2LikelihoodFit.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(BANFFlib) $?
	$(lib_silent) $(ranlib) $(BANFFlib)
	$(lib_silent) cat /dev/null >$(BANFFstamp)

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

$(BANFFlibname).$(shlibsuffix) :: $(BANFFlib) requirements $(use_requirements) $(BANFFstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" BANFF $(BANFF_shlibflags)

$(BANFFshstamp) :: $(BANFFlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(BANFFlibname).$(shlibsuffix) ; then cat /dev/null >$(BANFFshstamp) ; fi

BANFFclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)EigenDecomp.o $(bin)ThrowParms.o $(bin)BANFFEventBase.o $(bin)BANFFInterfaceBase.o $(bin)FitParameters.o $(bin)WeightFile.o $(bin)BANFFBinnedSampleUtils.o $(bin)BANFFBinnedSample.o $(bin)psycheInterface.o $(bin)psycheObservables.o $(bin)Minuit2LikelihoodFitFCN.o $(bin)Minuit2LikelihoodFit.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf BANFF_deps BANFF_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
BANFFinstallname = $(library_prefix)BANFF$(library_suffix).$(shlibsuffix)

BANFF :: BANFFinstall

install :: BANFFinstall

BANFFinstall :: $(install_dir)/$(BANFFinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(BANFFinstallname) :: $(bin)$(BANFFinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(BANFFinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##BANFFclean :: BANFFuninstall

uninstall :: BANFFuninstall

BANFFuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(BANFFinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),BANFFclean)

#$(bin)BANFF_dependencies.make :: dirs

ifndef QUICK
$(bin)BANFF_dependencies.make : ../src/EigenDecomp.cxx ../src/ThrowParms.cxx ../src/BANFFEventBase.cxx ../src/BANFFInterfaceBase.cxx ../src/FitParameters.cxx ../src/WeightFile.cxx ../src/BANFFSample/BANFFBinnedSampleUtils.cxx ../src/BANFFSample/BANFFBinnedSample.cxx ../src/psycheInterface/psycheInterface.cxx ../src/psycheInterface/psycheObservables.cxx ../src/Minuit2LikelihoodFit/Minuit2LikelihoodFitFCN.cxx ../src/Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx $(use_requirements) $(cmt_final_setup_BANFF)
	$(echo) "(BANFF.make) Rebuilding $@"; \
	  $(build_dependencies) BANFF -all_sources -out=$@ ../src/EigenDecomp.cxx ../src/ThrowParms.cxx ../src/BANFFEventBase.cxx ../src/BANFFInterfaceBase.cxx ../src/FitParameters.cxx ../src/WeightFile.cxx ../src/BANFFSample/BANFFBinnedSampleUtils.cxx ../src/BANFFSample/BANFFBinnedSample.cxx ../src/psycheInterface/psycheInterface.cxx ../src/psycheInterface/psycheObservables.cxx ../src/Minuit2LikelihoodFit/Minuit2LikelihoodFitFCN.cxx ../src/Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx
endif

#$(BANFF_dependencies)

-include $(bin)BANFF_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(EigenDecomp_cxx_dependencies)

$(bin)$(binobj)EigenDecomp.o : $(EigenDecomp_cxx_dependencies)
	$(cpp_echo) $(src)EigenDecomp.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(EigenDecomp_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(EigenDecomp_cppflags) $(EigenDecomp_cxx_cppflags)  $(src)EigenDecomp.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(ThrowParms_cxx_dependencies)

$(bin)$(binobj)ThrowParms.o : $(ThrowParms_cxx_dependencies)
	$(cpp_echo) $(src)ThrowParms.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(ThrowParms_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(ThrowParms_cppflags) $(ThrowParms_cxx_cppflags)  $(src)ThrowParms.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(BANFFEventBase_cxx_dependencies)

$(bin)$(binobj)BANFFEventBase.o : $(BANFFEventBase_cxx_dependencies)
	$(cpp_echo) $(src)BANFFEventBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(BANFFEventBase_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(BANFFEventBase_cppflags) $(BANFFEventBase_cxx_cppflags)  $(src)BANFFEventBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(BANFFInterfaceBase_cxx_dependencies)

$(bin)$(binobj)BANFFInterfaceBase.o : $(BANFFInterfaceBase_cxx_dependencies)
	$(cpp_echo) $(src)BANFFInterfaceBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(BANFFInterfaceBase_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(BANFFInterfaceBase_cppflags) $(BANFFInterfaceBase_cxx_cppflags)  $(src)BANFFInterfaceBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(FitParameters_cxx_dependencies)

$(bin)$(binobj)FitParameters.o : $(FitParameters_cxx_dependencies)
	$(cpp_echo) $(src)FitParameters.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(FitParameters_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(FitParameters_cppflags) $(FitParameters_cxx_cppflags)  $(src)FitParameters.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(WeightFile_cxx_dependencies)

$(bin)$(binobj)WeightFile.o : $(WeightFile_cxx_dependencies)
	$(cpp_echo) $(src)WeightFile.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(WeightFile_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(WeightFile_cppflags) $(WeightFile_cxx_cppflags)  $(src)WeightFile.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(BANFFBinnedSampleUtils_cxx_dependencies)

$(bin)$(binobj)BANFFBinnedSampleUtils.o : $(BANFFBinnedSampleUtils_cxx_dependencies)
	$(cpp_echo) $(src)BANFFSample/BANFFBinnedSampleUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(BANFFBinnedSampleUtils_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(BANFFBinnedSampleUtils_cppflags) $(BANFFBinnedSampleUtils_cxx_cppflags) -I../src/BANFFSample $(src)BANFFSample/BANFFBinnedSampleUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(BANFFBinnedSample_cxx_dependencies)

$(bin)$(binobj)BANFFBinnedSample.o : $(BANFFBinnedSample_cxx_dependencies)
	$(cpp_echo) $(src)BANFFSample/BANFFBinnedSample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(BANFFBinnedSample_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(BANFFBinnedSample_cppflags) $(BANFFBinnedSample_cxx_cppflags) -I../src/BANFFSample $(src)BANFFSample/BANFFBinnedSample.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(psycheInterface_cxx_dependencies)

$(bin)$(binobj)psycheInterface.o : $(psycheInterface_cxx_dependencies)
	$(cpp_echo) $(src)psycheInterface/psycheInterface.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(psycheInterface_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(psycheInterface_cppflags) $(psycheInterface_cxx_cppflags) -I../src/psycheInterface $(src)psycheInterface/psycheInterface.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(psycheObservables_cxx_dependencies)

$(bin)$(binobj)psycheObservables.o : $(psycheObservables_cxx_dependencies)
	$(cpp_echo) $(src)psycheInterface/psycheObservables.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(psycheObservables_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(psycheObservables_cppflags) $(psycheObservables_cxx_cppflags) -I../src/psycheInterface $(src)psycheInterface/psycheObservables.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(Minuit2LikelihoodFitFCN_cxx_dependencies)

$(bin)$(binobj)Minuit2LikelihoodFitFCN.o : $(Minuit2LikelihoodFitFCN_cxx_dependencies)
	$(cpp_echo) $(src)Minuit2LikelihoodFit/Minuit2LikelihoodFitFCN.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(Minuit2LikelihoodFitFCN_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(Minuit2LikelihoodFitFCN_cppflags) $(Minuit2LikelihoodFitFCN_cxx_cppflags) -I../src/Minuit2LikelihoodFit $(src)Minuit2LikelihoodFit/Minuit2LikelihoodFitFCN.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)BANFF_dependencies.make : $(Minuit2LikelihoodFit_cxx_dependencies)

$(bin)$(binobj)Minuit2LikelihoodFit.o : $(Minuit2LikelihoodFit_cxx_dependencies)
	$(cpp_echo) $(src)Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(BANFF_pp_cppflags) $(lib_BANFF_pp_cppflags) $(Minuit2LikelihoodFit_pp_cppflags) $(use_cppflags) $(BANFF_cppflags) $(lib_BANFF_cppflags) $(Minuit2LikelihoodFit_cppflags) $(Minuit2LikelihoodFit_cxx_cppflags) -I../src/Minuit2LikelihoodFit $(src)Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: BANFFclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(BANFF.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(BANFF.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_BANFF)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BANFF.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BANFF.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(BANFF.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

BANFFclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)BANFF$(library_suffix).a $(binobj)$(library_prefix)BANFF$(library_suffix).s? $(binobj)BANFF.stamp $(binobj)BANFF.shstamp
#-- end of cleanup_library ---------------

