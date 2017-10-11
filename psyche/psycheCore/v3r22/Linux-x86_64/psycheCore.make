#-- start of make_header -----------------

#====================================
#  Library psycheCore
#
#   Generated Mon Oct  9 06:37:32 2017  by mhogan
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_psycheCore_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_psycheCore_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_psycheCore

psycheCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheCore = /tmp/CMT_$(psycheCore_tag)_psycheCore.make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheCore = $(psycheCore_tag)_psycheCore.make
cmt_local_tagfile_psycheCore = $(bin)$(psycheCore_tag)_psycheCore.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

psycheCore_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_psycheCore = /tmp/CMT_$(psycheCore_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_psycheCore = $(psycheCore_tag).make
cmt_local_tagfile_psycheCore = $(bin)$(psycheCore_tag).make
endif

endif

-include $(cmt_local_tagfile_psycheCore)

ifdef cmt_psycheCore_has_target_tag

ifdef READONLY
cmt_final_setup_psycheCore = /tmp/CMT_psycheCore_psycheCoresetup.make
cmt_local_psycheCore_makefile = /tmp/CMT_psycheCore$(cmt_lock_pid).make
else
cmt_final_setup_psycheCore = $(bin)psycheCore_psycheCoresetup.make
cmt_local_psycheCore_makefile = $(bin)psycheCore.make
endif

else

ifdef READONLY
cmt_final_setup_psycheCore = /tmp/CMT_psycheCoresetup.make
cmt_local_psycheCore_makefile = /tmp/CMT_psycheCore$(cmt_lock_pid).make
else
cmt_final_setup_psycheCore = $(bin)psycheCoresetup.make
cmt_local_psycheCore_makefile = $(bin)psycheCore.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_psycheCoresetup.make
else
cmt_final_setup = $(bin)psycheCoresetup.make
endif

psycheCore ::


ifdef READONLY
psycheCore ::
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
	$(echo) 'psycheCore'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = psycheCore/
psycheCore::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

psycheCorelibname   = $(bin)$(library_prefix)psycheCore$(library_suffix)
psycheCorelib       = $(psycheCorelibname).a
psycheCorestamp     = $(bin)psycheCore.stamp
psycheCoreshstamp   = $(bin)psycheCore.shstamp

psycheCore :: dirs  psycheCoreLIB
	$(echo) "psycheCore ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

psycheCoreLIB :: $(psycheCorelib) $(psycheCoreshstamp)
	$(echo) "psycheCore : library ok"

$(psycheCorelib) :: $(bin)TreeManager.o $(bin)CoreDataClasses.o $(bin)UnitsParser.o $(bin)CoreUtils.o $(bin)WeightType.o $(bin)DataSample.o $(bin)EventVariationBase.o $(bin)EventVariationManager.o $(bin)EventWeightBase.o $(bin)EventWeightManager.o $(bin)Experiment.o $(bin)HEPConstants.o $(bin)HLClonesArray.o $(bin)Header.o $(bin)InputConverter.o $(bin)InputManager.o $(bin)Parameters.o $(bin)ParticleId.o $(bin)SelectionBase.o $(bin)SelectionManager.o $(bin)StepBase.o $(bin)SystBoxB.o $(bin)SystematicBase.o $(bin)SystematicCovariance.o $(bin)SystematicManager.o $(bin)ToyBoxB.o $(bin)ToyExperiment.o $(bin)ToyMaker.o $(bin)ToyVariations.o $(bin)TreeManager_Dict.o $(bin)CoreDataClasses_Dict.o $(bin)DataSample_Dict.o $(bin)Experiment_Dict.o $(bin)HLClonesArray_Dict.o $(bin)Header_Dict.o $(bin)SelectionBase_Dict.o $(bin)StepBase_Dict.o $(bin)SystematicBase_Dict.o
	$(lib_echo) "static library $@"
	$(lib_silent) $(ar) $(psycheCorelib) $?
	$(lib_silent) $(ranlib) $(psycheCorelib)
	$(lib_silent) cat /dev/null >$(psycheCorestamp)

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

$(psycheCorelibname).$(shlibsuffix) :: $(psycheCorelib) requirements $(use_requirements) $(psycheCorestamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin=$(bin) $(make_shlib) "$(tags)" psycheCore $(psycheCore_shlibflags)

$(psycheCoreshstamp) :: $(psycheCorelibname).$(shlibsuffix)
	$(lib_silent) if test -f $(psycheCorelibname).$(shlibsuffix) ; then cat /dev/null >$(psycheCoreshstamp) ; fi

psycheCoreclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) /bin/rm -f $(bin)TreeManager.o $(bin)CoreDataClasses.o $(bin)UnitsParser.o $(bin)CoreUtils.o $(bin)WeightType.o $(bin)DataSample.o $(bin)EventVariationBase.o $(bin)EventVariationManager.o $(bin)EventWeightBase.o $(bin)EventWeightManager.o $(bin)Experiment.o $(bin)HEPConstants.o $(bin)HLClonesArray.o $(bin)Header.o $(bin)InputConverter.o $(bin)InputManager.o $(bin)Parameters.o $(bin)ParticleId.o $(bin)SelectionBase.o $(bin)SelectionManager.o $(bin)StepBase.o $(bin)SystBoxB.o $(bin)SystematicBase.o $(bin)SystematicCovariance.o $(bin)SystematicManager.o $(bin)ToyBoxB.o $(bin)ToyExperiment.o $(bin)ToyMaker.o $(bin)ToyVariations.o $(bin)TreeManager_Dict.o $(bin)CoreDataClasses_Dict.o $(bin)DataSample_Dict.o $(bin)Experiment_Dict.o $(bin)HLClonesArray_Dict.o $(bin)Header_Dict.o $(bin)SelectionBase_Dict.o $(bin)StepBase_Dict.o $(bin)SystematicBase_Dict.o
	$(cleanup_silent) cd $(bin); /bin/rm -rf psycheCore_deps psycheCore_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
psycheCoreinstallname = $(library_prefix)psycheCore$(library_suffix).$(shlibsuffix)

psycheCore :: psycheCoreinstall

install :: psycheCoreinstall

psycheCoreinstall :: $(install_dir)/$(psycheCoreinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(psycheCoreinstallname) :: $(bin)$(psycheCoreinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheCoreinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##psycheCoreclean :: psycheCoreuninstall

uninstall :: psycheCoreuninstall

psycheCoreuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(psycheCoreinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),psycheCoreclean)

#$(bin)psycheCore_dependencies.make :: dirs

ifndef QUICK
$(bin)psycheCore_dependencies.make : ../src/TreeManager.cxx ../src/CoreDataClasses.cxx ../src/UnitsParser.cxx ../src/CoreUtils.cxx ../src/WeightType.cxx ../src/DataSample.cxx ../src/EventVariationBase.cxx ../src/EventVariationManager.cxx ../src/EventWeightBase.cxx ../src/EventWeightManager.cxx ../src/Experiment.cxx ../src/HEPConstants.cxx ../src/HLClonesArray.cxx ../src/Header.cxx ../src/InputConverter.cxx ../src/InputManager.cxx ../src/Parameters.cxx ../src/ParticleId.cxx ../src/SelectionBase.cxx ../src/SelectionManager.cxx ../src/StepBase.cxx ../src/SystBoxB.cxx ../src/SystematicBase.cxx ../src/SystematicCovariance.cxx ../src/SystematicManager.cxx ../src/ToyBoxB.cxx ../src/ToyExperiment.cxx ../src/ToyMaker.cxx ../src/ToyVariations.cxx ../dict/TreeManager_Dict.cxx ../dict/CoreDataClasses_Dict.cxx ../dict/DataSample_Dict.cxx ../dict/Experiment_Dict.cxx ../dict/HLClonesArray_Dict.cxx ../dict/Header_Dict.cxx ../dict/SelectionBase_Dict.cxx ../dict/StepBase_Dict.cxx ../dict/SystematicBase_Dict.cxx $(use_requirements) $(cmt_final_setup_psycheCore)
	$(echo) "(psycheCore.make) Rebuilding $@"; \
	  $(build_dependencies) psycheCore -all_sources -out=$@ ../src/TreeManager.cxx ../src/CoreDataClasses.cxx ../src/UnitsParser.cxx ../src/CoreUtils.cxx ../src/WeightType.cxx ../src/DataSample.cxx ../src/EventVariationBase.cxx ../src/EventVariationManager.cxx ../src/EventWeightBase.cxx ../src/EventWeightManager.cxx ../src/Experiment.cxx ../src/HEPConstants.cxx ../src/HLClonesArray.cxx ../src/Header.cxx ../src/InputConverter.cxx ../src/InputManager.cxx ../src/Parameters.cxx ../src/ParticleId.cxx ../src/SelectionBase.cxx ../src/SelectionManager.cxx ../src/StepBase.cxx ../src/SystBoxB.cxx ../src/SystematicBase.cxx ../src/SystematicCovariance.cxx ../src/SystematicManager.cxx ../src/ToyBoxB.cxx ../src/ToyExperiment.cxx ../src/ToyMaker.cxx ../src/ToyVariations.cxx ../dict/TreeManager_Dict.cxx ../dict/CoreDataClasses_Dict.cxx ../dict/DataSample_Dict.cxx ../dict/Experiment_Dict.cxx ../dict/HLClonesArray_Dict.cxx ../dict/Header_Dict.cxx ../dict/SelectionBase_Dict.cxx ../dict/StepBase_Dict.cxx ../dict/SystematicBase_Dict.cxx
endif

#$(psycheCore_dependencies)

-include $(bin)psycheCore_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(TreeManager_cxx_dependencies)

$(bin)$(binobj)TreeManager.o : $(TreeManager_cxx_dependencies)
	$(cpp_echo) $(src)TreeManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(TreeManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(TreeManager_cppflags) $(TreeManager_cxx_cppflags)  $(src)TreeManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(CoreDataClasses_cxx_dependencies)

$(bin)$(binobj)CoreDataClasses.o : $(CoreDataClasses_cxx_dependencies)
	$(cpp_echo) $(src)CoreDataClasses.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(CoreDataClasses_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(CoreDataClasses_cppflags) $(CoreDataClasses_cxx_cppflags)  $(src)CoreDataClasses.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(UnitsParser_cxx_dependencies)

$(bin)$(binobj)UnitsParser.o : $(UnitsParser_cxx_dependencies)
	$(cpp_echo) $(src)UnitsParser.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(UnitsParser_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(UnitsParser_cppflags) $(UnitsParser_cxx_cppflags)  $(src)UnitsParser.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(CoreUtils_cxx_dependencies)

$(bin)$(binobj)CoreUtils.o : $(CoreUtils_cxx_dependencies)
	$(cpp_echo) $(src)CoreUtils.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(CoreUtils_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(CoreUtils_cppflags) $(CoreUtils_cxx_cppflags)  $(src)CoreUtils.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(WeightType_cxx_dependencies)

$(bin)$(binobj)WeightType.o : $(WeightType_cxx_dependencies)
	$(cpp_echo) $(src)WeightType.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(WeightType_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(WeightType_cppflags) $(WeightType_cxx_cppflags)  $(src)WeightType.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(DataSample_cxx_dependencies)

$(bin)$(binobj)DataSample.o : $(DataSample_cxx_dependencies)
	$(cpp_echo) $(src)DataSample.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(DataSample_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(DataSample_cppflags) $(DataSample_cxx_cppflags)  $(src)DataSample.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(EventVariationBase_cxx_dependencies)

$(bin)$(binobj)EventVariationBase.o : $(EventVariationBase_cxx_dependencies)
	$(cpp_echo) $(src)EventVariationBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(EventVariationBase_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(EventVariationBase_cppflags) $(EventVariationBase_cxx_cppflags)  $(src)EventVariationBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(EventVariationManager_cxx_dependencies)

$(bin)$(binobj)EventVariationManager.o : $(EventVariationManager_cxx_dependencies)
	$(cpp_echo) $(src)EventVariationManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(EventVariationManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(EventVariationManager_cppflags) $(EventVariationManager_cxx_cppflags)  $(src)EventVariationManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(EventWeightBase_cxx_dependencies)

$(bin)$(binobj)EventWeightBase.o : $(EventWeightBase_cxx_dependencies)
	$(cpp_echo) $(src)EventWeightBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(EventWeightBase_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(EventWeightBase_cppflags) $(EventWeightBase_cxx_cppflags)  $(src)EventWeightBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(EventWeightManager_cxx_dependencies)

$(bin)$(binobj)EventWeightManager.o : $(EventWeightManager_cxx_dependencies)
	$(cpp_echo) $(src)EventWeightManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(EventWeightManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(EventWeightManager_cppflags) $(EventWeightManager_cxx_cppflags)  $(src)EventWeightManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(Experiment_cxx_dependencies)

$(bin)$(binobj)Experiment.o : $(Experiment_cxx_dependencies)
	$(cpp_echo) $(src)Experiment.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(Experiment_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(Experiment_cppflags) $(Experiment_cxx_cppflags)  $(src)Experiment.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(HEPConstants_cxx_dependencies)

$(bin)$(binobj)HEPConstants.o : $(HEPConstants_cxx_dependencies)
	$(cpp_echo) $(src)HEPConstants.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(HEPConstants_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(HEPConstants_cppflags) $(HEPConstants_cxx_cppflags)  $(src)HEPConstants.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(HLClonesArray_cxx_dependencies)

$(bin)$(binobj)HLClonesArray.o : $(HLClonesArray_cxx_dependencies)
	$(cpp_echo) $(src)HLClonesArray.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(HLClonesArray_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(HLClonesArray_cppflags) $(HLClonesArray_cxx_cppflags)  $(src)HLClonesArray.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(Header_cxx_dependencies)

$(bin)$(binobj)Header.o : $(Header_cxx_dependencies)
	$(cpp_echo) $(src)Header.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(Header_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(Header_cppflags) $(Header_cxx_cppflags)  $(src)Header.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(InputConverter_cxx_dependencies)

$(bin)$(binobj)InputConverter.o : $(InputConverter_cxx_dependencies)
	$(cpp_echo) $(src)InputConverter.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(InputConverter_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(InputConverter_cppflags) $(InputConverter_cxx_cppflags)  $(src)InputConverter.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(InputManager_cxx_dependencies)

$(bin)$(binobj)InputManager.o : $(InputManager_cxx_dependencies)
	$(cpp_echo) $(src)InputManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(InputManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(InputManager_cppflags) $(InputManager_cxx_cppflags)  $(src)InputManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(Parameters_cxx_dependencies)

$(bin)$(binobj)Parameters.o : $(Parameters_cxx_dependencies)
	$(cpp_echo) $(src)Parameters.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(Parameters_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(Parameters_cppflags) $(Parameters_cxx_cppflags)  $(src)Parameters.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(ParticleId_cxx_dependencies)

$(bin)$(binobj)ParticleId.o : $(ParticleId_cxx_dependencies)
	$(cpp_echo) $(src)ParticleId.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(ParticleId_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(ParticleId_cppflags) $(ParticleId_cxx_cppflags)  $(src)ParticleId.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SelectionBase_cxx_dependencies)

$(bin)$(binobj)SelectionBase.o : $(SelectionBase_cxx_dependencies)
	$(cpp_echo) $(src)SelectionBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SelectionBase_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SelectionBase_cppflags) $(SelectionBase_cxx_cppflags)  $(src)SelectionBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SelectionManager_cxx_dependencies)

$(bin)$(binobj)SelectionManager.o : $(SelectionManager_cxx_dependencies)
	$(cpp_echo) $(src)SelectionManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SelectionManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SelectionManager_cppflags) $(SelectionManager_cxx_cppflags)  $(src)SelectionManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(StepBase_cxx_dependencies)

$(bin)$(binobj)StepBase.o : $(StepBase_cxx_dependencies)
	$(cpp_echo) $(src)StepBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(StepBase_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(StepBase_cppflags) $(StepBase_cxx_cppflags)  $(src)StepBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SystBoxB_cxx_dependencies)

$(bin)$(binobj)SystBoxB.o : $(SystBoxB_cxx_dependencies)
	$(cpp_echo) $(src)SystBoxB.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SystBoxB_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SystBoxB_cppflags) $(SystBoxB_cxx_cppflags)  $(src)SystBoxB.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SystematicBase_cxx_dependencies)

$(bin)$(binobj)SystematicBase.o : $(SystematicBase_cxx_dependencies)
	$(cpp_echo) $(src)SystematicBase.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SystematicBase_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SystematicBase_cppflags) $(SystematicBase_cxx_cppflags)  $(src)SystematicBase.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SystematicCovariance_cxx_dependencies)

$(bin)$(binobj)SystematicCovariance.o : $(SystematicCovariance_cxx_dependencies)
	$(cpp_echo) $(src)SystematicCovariance.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SystematicCovariance_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SystematicCovariance_cppflags) $(SystematicCovariance_cxx_cppflags)  $(src)SystematicCovariance.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SystematicManager_cxx_dependencies)

$(bin)$(binobj)SystematicManager.o : $(SystematicManager_cxx_dependencies)
	$(cpp_echo) $(src)SystematicManager.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SystematicManager_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SystematicManager_cppflags) $(SystematicManager_cxx_cppflags)  $(src)SystematicManager.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(ToyBoxB_cxx_dependencies)

$(bin)$(binobj)ToyBoxB.o : $(ToyBoxB_cxx_dependencies)
	$(cpp_echo) $(src)ToyBoxB.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(ToyBoxB_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(ToyBoxB_cppflags) $(ToyBoxB_cxx_cppflags)  $(src)ToyBoxB.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(ToyExperiment_cxx_dependencies)

$(bin)$(binobj)ToyExperiment.o : $(ToyExperiment_cxx_dependencies)
	$(cpp_echo) $(src)ToyExperiment.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(ToyExperiment_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(ToyExperiment_cppflags) $(ToyExperiment_cxx_cppflags)  $(src)ToyExperiment.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(ToyMaker_cxx_dependencies)

$(bin)$(binobj)ToyMaker.o : $(ToyMaker_cxx_dependencies)
	$(cpp_echo) $(src)ToyMaker.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(ToyMaker_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(ToyMaker_cppflags) $(ToyMaker_cxx_cppflags)  $(src)ToyMaker.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(ToyVariations_cxx_dependencies)

$(bin)$(binobj)ToyVariations.o : $(ToyVariations_cxx_dependencies)
	$(cpp_echo) $(src)ToyVariations.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(ToyVariations_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(ToyVariations_cppflags) $(ToyVariations_cxx_cppflags)  $(src)ToyVariations.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(TreeManager_Dict_cxx_dependencies)

$(bin)$(binobj)TreeManager_Dict.o : $(TreeManager_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/TreeManager_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(TreeManager_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(TreeManager_Dict_cppflags) $(TreeManager_Dict_cxx_cppflags) -I../dict ../dict/TreeManager_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(CoreDataClasses_Dict_cxx_dependencies)

$(bin)$(binobj)CoreDataClasses_Dict.o : $(CoreDataClasses_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/CoreDataClasses_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(CoreDataClasses_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(CoreDataClasses_Dict_cppflags) $(CoreDataClasses_Dict_cxx_cppflags) -I../dict ../dict/CoreDataClasses_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(DataSample_Dict_cxx_dependencies)

$(bin)$(binobj)DataSample_Dict.o : $(DataSample_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/DataSample_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(DataSample_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(DataSample_Dict_cppflags) $(DataSample_Dict_cxx_cppflags) -I../dict ../dict/DataSample_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(Experiment_Dict_cxx_dependencies)

$(bin)$(binobj)Experiment_Dict.o : $(Experiment_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/Experiment_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(Experiment_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(Experiment_Dict_cppflags) $(Experiment_Dict_cxx_cppflags) -I../dict ../dict/Experiment_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(HLClonesArray_Dict_cxx_dependencies)

$(bin)$(binobj)HLClonesArray_Dict.o : $(HLClonesArray_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/HLClonesArray_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(HLClonesArray_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(HLClonesArray_Dict_cppflags) $(HLClonesArray_Dict_cxx_cppflags) -I../dict ../dict/HLClonesArray_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(Header_Dict_cxx_dependencies)

$(bin)$(binobj)Header_Dict.o : $(Header_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/Header_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(Header_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(Header_Dict_cppflags) $(Header_Dict_cxx_cppflags) -I../dict ../dict/Header_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SelectionBase_Dict_cxx_dependencies)

$(bin)$(binobj)SelectionBase_Dict.o : $(SelectionBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/SelectionBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SelectionBase_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SelectionBase_Dict_cppflags) $(SelectionBase_Dict_cxx_cppflags) -I../dict ../dict/SelectionBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(StepBase_Dict_cxx_dependencies)

$(bin)$(binobj)StepBase_Dict.o : $(StepBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/StepBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(StepBase_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(StepBase_Dict_cppflags) $(StepBase_Dict_cxx_cppflags) -I../dict ../dict/StepBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)psycheCore_dependencies.make : $(SystematicBase_Dict_cxx_dependencies)

$(bin)$(binobj)SystematicBase_Dict.o : $(SystematicBase_Dict_cxx_dependencies)
	$(cpp_echo) ../dict/SystematicBase_Dict.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(psycheCore_pp_cppflags) $(lib_psycheCore_pp_cppflags) $(SystematicBase_Dict_pp_cppflags) $(use_cppflags) $(psycheCore_cppflags) $(lib_psycheCore_cppflags) $(SystematicBase_Dict_cppflags) $(SystematicBase_Dict_cxx_cppflags) -I../dict ../dict/SystematicBase_Dict.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: psycheCoreclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(psycheCore.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(psycheCore.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_psycheCore)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheCore.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheCore.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(psycheCore.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

psycheCoreclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)psycheCore$(library_suffix).a $(binobj)$(library_prefix)psycheCore$(library_suffix).s? $(binobj)psycheCore.stamp $(binobj)psycheCore.shstamp
#-- end of cleanup_library ---------------

